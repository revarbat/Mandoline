//
//  SlicingContext.cc
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//

#include "SlicingContext.hh"
#include "Defaults.hh"
#include <iostream>
#include <fstream>

// default constructor
SlicingContext::SlicingContext()
{
    filamentFeedRate     = DEFAULT_FILAMENT_FEED_RATE;
    filamentDiameter     = DEFAULT_FILAMENT_DIAMETER;
    driveGearDiameter    = DEFAULT_DRIVE_GEAR_DIAMETER;
    layerThickness       = DEFAULT_LAYER_THICKNESS;
    widthOverHeightRatio = DEFAULT_WIDTH_OVER_HEIGHT;
    shrinkageRatio       = DEFAULT_SHRINKAGE_RATIO;
    infillDensity        = DEFAULT_INFILL_DENSITY;
    perimeterShells      = DEFAULT_PERIMETER_SHELLS;
    flatShells           = DEFAULT_FLAT_SHELLS;
    raftLayers           = DEFAULT_RAFT_LAYERS;
    raftOutset           = DEFAULT_RAFT_OUTSET;
    minLayerTime         = DEFAULT_MIN_LAYER_TIME;
    
    calculateSvgOffsets();
}



void SlicingContext::calculateSvgOffsets()
{
    // Calculate offsets for SVG dumps
    svgWidth  = 40 + mesh.maxX - mesh.minX;
    svgHeight = 40 + mesh.maxY - mesh.minY;
    svgXOff   = 20 - mesh.minX;
    svgYOff   = 20 - mesh.minY;
}



void SlicingContext::loadSettingsFromFile(const char *fileName)
{
    ifstream fin(fileName);
    char buf[1024];
    char *ptr;
    char *optname;
    char *value;

    if (!fin.is_open()) {
        cerr << "Can't open file for reading: " << fileName << endl;
        return;
    }

    int linenum = 0;
    while (fin.good()) {
        fin.getline(buf,sizeof(buf));
	linenum++;

	// Skip leading spaces.
	for (ptr = buf; isspace(*ptr); ptr++);
	optname = ptr;

	// If line is empty, or first non-space char is '#', skip.
	if (!*optname || *optname == '#') {
	    continue;
	}

	// Find end of option name.
	while (isalpha(*ptr) || isdigit(*ptr)) ptr++;
	value = ptr;

	// Skip any space after option name.
	while (isspace(*ptr)) ptr++;

	// If the next char is not ':', print error.  Skip.
	if (*ptr != ':') {
	    cerr << "Bad option syntax at line " << linenum
	         << " of " << fileName << endl;
	    continue;
	}
	ptr++;

	// Null terminate the option name.
	*value = '\0';

	// Skip any space before option value.
	while (isspace(*ptr)) ptr++;

	// remember start of value string
	value = ptr;

	// Find end of value string.
	while (isalnum(*ptr) || *ptr == '.') ptr++;

	// Null out spaces after value string.
	while (isspace(*ptr)) {
	    *ptr++ = '\0';
	}

	// If there's any trailing non-whitespace chars, print an error. Skip.
	if (*ptr != '\0') {
	    cerr << "Cruft at end of line " << linenum
	         << " of " << fileName << endl;
	    continue;
	}

	if (!strcasecmp(optname, "filamentFeedRate")) {
	    filamentFeedRate = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "filamentDiameter")) {
	    filamentDiameter = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "driveGearDiameter")) {
	    driveGearDiameter = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "layerThickness")) {
	    layerThickness = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "widthOverHeightRatio")) {
	    widthOverHeightRatio = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "shrinkageRatio")) {
	    shrinkageRatio = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "infillDensity")) {
	    infillDensity = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "perimeterShells")) {
	    perimeterShells = strtol(value, &ptr, 10);
	} else if (!strcasecmp(optname, "flatShells")) {
	    flatShells = strtol(value, &ptr, 10);
	} else if (!strcasecmp(optname, "raftLayers")) {
	    raftLayers = strtol(value, &ptr, 10);
	} else if (!strcasecmp(optname, "raftOutset")) {
	    raftOutset = strtod(value, &ptr);
	} else if (!strcasecmp(optname, "minLayerTime")) {
	    minLayerTime = strtod(value, &ptr);
	} else {
	    cerr << "Bad option '" << optname
	         << "' on line " << linenum
	         << " of " << fileName << endl;
	    continue;
	}

	if (*ptr != '\0') {
	    cerr << "Cruft at end of line " << linenum
	         << " of " << fileName << endl;
	    continue;
	}
    }

    fin.close();
}




void SlicingContext::saveSettingsToFile(const char *fileName)
{
    ofstream fout(fileName);

    if (!fout.is_open()) {
        cerr << "Can't open file for writing: " << fileName << endl;
        return;
    }

    fout.setf(ios::fixed, ios::floatfield);

    fout.precision(2);
    fout << "# Rate to feed the filament, in RPM." << endl;
    fout << "filamentFeedRate: " << filamentFeedRate     << endl;
    fout << endl;

    fout.precision(3);
    fout << "# Diameter of the filament stock in millimeters." << endl;
    fout << "filamentDiameter: " << filamentDiameter     << endl;
    fout << endl;

    fout.precision(2);
    fout << "# Diameter of the gear used to drive the filament, in millimeters." << endl;
    fout << "driveGearDiameter: " << driveGearDiameter     << endl;
    fout << endl;

    fout.precision(3);
    fout << "# Layer thickness, in millimeters." << endl;
    fout << "layerThickness: " << layerThickness       << endl;
    fout << endl;

    fout.precision(3);
    fout << "# The ratio of the width to the height of the extrusion." << endl;
    fout << "widthOverHeightRatio: " << widthOverHeightRatio << endl;
    fout << endl;

    fout.precision(3);
    fout << "# The ratio that the part is expected to shrink by when it cools." << endl;
    fout << "shrinkageRatio: " << shrinkageRatio       << endl;
    fout << endl;

    fout.precision(3);
    fout << "# The density ratio to be used when infilling." << endl;
    fout << "infillDensity: " << infillDensity        << endl;
    fout << endl;

    fout << "# The number of perimeter shell layers to use." << endl;
    fout << "perimeterShells: " << perimeterShells      << endl;
    fout << endl;

    fout << "# The number of layers used to seal tops and bottoms." << endl;
    fout << "flatShells: " << flatShells           << endl;
    fout << endl;

    fout << "# Number of raft layers to set down. 0=No raft. >1 adds interface layers." << endl;
    fout << "raftLayers: " << raftLayers           << endl;
    fout << endl;

    fout.precision(2);
    fout << "# How far to outset raft, in millimeters." << endl;
    fout << "raftOutset: " << raftOutset           << endl;
    fout << endl;

    fout.precision(1);
    fout << "# Minimum time to spend on a layer. Slows down printing of small layers." << endl;
    fout << "minLayerTime: " << minLayerTime         << endl;
    fout << endl;

    fout.close();
}




double SlicingContext::standardFeedRate()
{
    double filamentRadius = 0.5f * filamentDiameter;
    double filamentFeedMmPerSec = (filamentFeedRate/60)*driveGearDiameter*M_PI;
    return (4*filamentFeedMmPerSec*filamentRadius*filamentRadius) / (widthOverHeightRatio*layerThickness*layerThickness);
}



double SlicingContext::standardExtrusionWidth()
{
    return layerThickness * widthOverHeightRatio;
}



double SlicingContext::ratioForWidth(double extrusionWidth)
{
    return extrusionWidth / layerThickness;
}



double SlicingContext::feedRateForWidth(double extrusionWidth)
{
    double filamentRadius = 0.5f * filamentDiameter;
    double filamentFeedMmPerSec = (filamentFeedRate/60)*driveGearDiameter*M_PI;
    return (4*filamentFeedMmPerSec*filamentRadius*filamentRadius) / (extrusionWidth*layerThickness);
}



CarvedSlice* SlicingContext::allocSlice(double Z)
{
    CarvedSlice *slice = NULL;
    slices.push_back(CarvedSlice());
    slice = &slices.back();
    slice->zLayer = Z;
    return slice;
}



