//
//  RaftOp.cc
//  Mandoline
//
//  Created by GM on 2011-12-30
//  Copyright 2011 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "RaftOp.hh"
#include "SlicingContext.hh"
#include "CarvedSlice.hh"



RaftOp::~RaftOp()
{
}



void RaftOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;

    CarvedSlice *baseSlice = new CarvedSlice();

    // So that we get a raft shape that doesn't have small inefficient
    // holes, we outset by twice the raft outset, then inset back to the
    // correct final size.  This removes the silly holes. Note that an outset
    // is the same as a negative inset.

    // Until we add support for support material, we'll use the first layer's
    // perimeter as the initial shape to base the raft on.
    BGL::CompoundRegion tempOutline;
    context->slices.begin()->perimeter.inset(-2.0*fabs(context->raftOutset), tempOutline);
    tempOutline.inset(context->raftOutset, baseSlice->perimeter);
    baseSlice->infillMask = baseSlice->perimeter;

    if ( isCancelled ) return;

    double minX = context->mesh.minX - context->raftOutset;;
    double maxX = context->mesh.maxX + context->raftOutset;;
    double minY = context->mesh.minY - context->raftOutset;;
    double maxY = context->mesh.maxY + context->raftOutset;;
    double offset = context->standardExtrusionWidth() * 2.0;

    // Build up list of base raft lines.
    BGL::Lines raftLines;
    for (double raftY = minY; raftY < maxY; raftY += offset) {
	BGL::Line tempLn(BGL::Point(minX, raftY), BGL::Point(maxX, raftY));
	baseSlice->perimeter.containedSegmentsOfLine(tempLn, raftLines);
    }

    // Convert lines to simple paths for later path optimization.
    BGL::Lines::iterator it;
    BGL::Paths tempPaths;
    for (it = raftLines.begin(); it != raftLines.end(); it++) {
	it->extrusionWidth = context->standardExtrusionWidth();
        tempPaths.push_back(BGL::Path(*it));
    }
    baseSlice->perimeter.joinSubPathsInside(offset*3.0, tempPaths, baseSlice->infill);
    baseSlice->state = RAFTED;

    if ( isCancelled ) return;

    CarvedSlice *ifaceSlice = new CarvedSlice();
    ifaceSlice->perimeter = baseSlice->perimeter;
    ifaceSlice->infillMask = baseSlice->perimeter;

    // Build up list of interface raft lines.
    BGL::Lines ifaceLines;
    for (double ifaceX = minX; ifaceX < maxX; ifaceX += offset) {
	BGL::Line tempLn(BGL::Point(ifaceX, minY), BGL::Point(ifaceX, maxY));
	ifaceSlice->perimeter.containedSegmentsOfLine(tempLn, ifaceLines);
    }

    // Convert lines to simple paths for later path optimization.
    BGL::Paths tempPaths2;
    for (it = ifaceLines.begin(); it != ifaceLines.end(); it++) {
	it->extrusionWidth = context->standardExtrusionWidth();
        tempPaths2.push_back(BGL::Path(*it));
    }
    ifaceSlice->perimeter.joinSubPathsInside(offset*3.0, tempPaths2, ifaceSlice->infill);
    ifaceSlice->state = RAFTED;

    // Add a copy of the interface layer for each raft layer above the base.
    for (int i = 1; i < context->raftLayers; i++) {
	context->slices.push_front(*ifaceSlice);
    }
    // Add the base raft layer to the bottom.
    context->slices.push_front(*baseSlice);
    
    // Adjust Z up for all layers.
    double Z = context->layerThickness;
    list<CarvedSlice>::iterator sit;
    for (sit = context->slices.begin(); sit != context->slices.end(); sit++) {
	sit->zLayer = Z;
        Z += context->layerThickness;
    }
}


