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

    context->processedLayers = 0;
    double nozzleDiam = context->nozzleDiameter[context->supportTool];

    // Raft base is extruded wider and slower for better platform adhesion.
    double baseExtWidth = nozzleDiam * 3.0;

    // Interface lines are extruded small and fast for easier part removal
    double ifaceExtWidth = nozzleDiam * 0.85;

    CarvedSlice *baseSlice = new CarvedSlice(nozzleDiam*1.5);
    double offset = baseExtWidth * 1.5; // provides space between base lines.

    // So that we get a raft shape that doesn't have small inefficient
    // holes, we outset by twice the raft outset, then inset back to the
    // correct final size.  This removes the silly holes. Note that an outset
    // is the same as a negative inset.

    // Until we add support for support material, we'll use the first layer's
    // perimeter as the initial shape to base the raft on.
    BGL::CompoundRegion tempOutline;
    double baseOutset = fabs(context->raftOutset) + fabs(offset);
    context->slices.begin()->perimeter.inset(-2.0*baseOutset, tempOutline);
    tempOutline.inset(baseOutset, baseSlice->perimeter);
    baseSlice->infillMask = baseSlice->perimeter;

    if ( isCancelled ) return;

    double minX = context->mesh.minX - baseOutset;
    double maxX = context->mesh.maxX + baseOutset;
    double minY = context->mesh.minY - baseOutset;
    double maxY = context->mesh.maxY + baseOutset;

    // Build up list of base raft lines.
    BGL::Lines raftLines;
    for (double raftX = minX; raftX < maxX; raftX += offset) {
        BGL::Line tempLn(BGL::Point(raftX, minY), BGL::Point(raftX, maxY));
        baseSlice->perimeter.containedSegmentsOfLine(tempLn, raftLines);
    }

    // Convert lines to simple paths for later path optimization.
    BGL::Lines::iterator it;
    BGL::Paths tempPaths;
    for (it = raftLines.begin(); it != raftLines.end(); it++) {
        it->extrusionWidth = baseExtWidth;;
        tempPaths.push_back(BGL::Path(*it));
    }
    baseSlice->perimeter.joinSubPathsInside(context->raftOutset, context->raftOutset, tempPaths, baseSlice->infill);
    //baseSlice->speedMult = 1.0; // Move slower to stick to platform better.
    baseSlice->isRaft = true;
    baseSlice->state = RAFTED;
    context->processedLayers++;

    if ( isCancelled ) return;

    CarvedSlice *ifaceSlice = new CarvedSlice(nozzleDiam*1.5);

    // Inset slightly more so interface layer isn't slopping off the sides of the base layer.
    baseSlice->perimeter.inset(baseExtWidth, ifaceSlice->perimeter);
    ifaceSlice->infillMask = ifaceSlice->perimeter;

    // Build up list of interface raft lines.
    offset = ifaceExtWidth * 2.5; // provides space between lines.
    BGL::Lines ifaceLines;
    for (double ifaceY = minY; ifaceY < maxY; ifaceY += offset) {
        BGL::Line tempLn(BGL::Point(minX, ifaceY), BGL::Point(maxX, ifaceY));
        ifaceSlice->perimeter.containedSegmentsOfLine(tempLn, ifaceLines);
    }

    // Convert lines to simple paths for later path optimization.
    BGL::Paths tempPaths2;
    for (it = ifaceLines.begin(); it != ifaceLines.end(); it++) {
        it->extrusionWidth = ifaceExtWidth;
        tempPaths2.push_back(BGL::Path(*it));
    }
    ifaceSlice->perimeter.joinSubPathsInside(context->raftOutset, context->raftOutset, tempPaths2, ifaceSlice->infill);
    ifaceSlice->isRaft = true;
    ifaceSlice->state = RAFTED;

    // Add a copy of the interface layer for each raft layer above the base.
    for (int i = 1; i < context->raftLayers; i++) {
        context->slices.push_front(*ifaceSlice);
        context->processedLayers++;
    }
    // Add the base raft layer to the bottom.
    context->slices.push_front(*baseSlice);
    
    // Adjust Z up for all layers.
    double Z = 0.0;
    list<CarvedSlice>::iterator sit;
    for (sit = context->slices.begin(); sit != context->slices.end(); sit++) {
        Z += sit->layerThickness;
        sit->zLayer = Z;
    }
}

// vim: set ts=4 sw=4 nowrap expandtab: settings

