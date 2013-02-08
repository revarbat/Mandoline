//
//  CoolOp.cc
//  Mandoline
//
//  Created by GM on 2011-01-05.
//  Copyright 2011 Belfry DevWorks. All rights reserved.
//

#include <BGL.hh>
#include "CoolOp.hh"



CoolOp::~CoolOp()
{
}



void CoolOp::main()
{
    if ( isCancelled ) return;
    if ( NULL == context ) return;
    if ( NULL == slice ) return;

    int mainTool = 0;

    double pathLen = 0.0;
    BGL::CompoundRegions::iterator cit;
    BGL::Paths::iterator pit;
    for (cit = slice->shells.begin(); cit != slice->shells.end(); cit++) {
        pathLen += cit->perimeterLength();
    }
    for (pit = slice->infill.begin(); pit != slice->infill.end(); pit++) {
        pathLen += pit->length();
    }

    // pathTime is in seconds.
    double pathTime = pathLen / (context->standardFeedRate(mainTool) / 60.0);
    if (pathTime > 0 && pathTime < context->minLayerTime) {
        slice->speedMult = pathTime / context->minLayerTime;
    }

    slice->state = COOLED;

    if ( isCancelled ) return;
}

// vim: set ts=4 sw=4 nowrap expandtab: settings

