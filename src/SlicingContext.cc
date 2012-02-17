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
#include <errno.h>
#include <sys/stat.h>

// default constructor
SlicingContext::SlicingContext()
{
    layerThickness       = DEFAULT_LAYER_THICKNESS;
    widthOverHeightRatio = DEFAULT_WIDTH_OVER_HEIGHT;
    infillDensity        = DEFAULT_INFILL_DENSITY;
    perimeterShells      = DEFAULT_PERIMETER_SHELLS;
    flatShells           = DEFAULT_FLAT_SHELLS;
    raftLayers           = DEFAULT_RAFT_LAYERS;
    raftOutset           = DEFAULT_RAFT_OUTSET;
    minLayerTime         = DEFAULT_MIN_LAYER_TIME;
    infillStyle          = DEFAULT_INFILL_STYLE;
    platformTemp         = DEFAULT_PLATFORM_TEMP;
    supportTool          = DEFAULT_SUPPORT_TOOL;

    for (int tool = 0; tool < MAX_TOOLS; tool++) {
        filamentFeedRate[tool]  = DEFAULT_FILAMENT_FEED_RATE;
        filamentDiameter[tool]  = DEFAULT_FILAMENT_DIAMETER;
        driveGearDiameter[tool] = DEFAULT_DRIVE_GEAR_DIAMETER;
        nozzleDiameter[tool]    = DEFAULT_NOZZLE_DIAMETER;
        extruderTemp[tool]      = DEFAULT_EXTRUDER_TEMP;
        retractionRate[tool]    = DEFAULT_RETRACTION_RATE;
        retractionTime[tool]    = DEFAULT_RETRACTION_TIME;
        pushBackTime[tool]      = DEFAULT_PUSH_BACK_TIME;
        xAxisOffset[tool]       = DEFAULT_X_AXIS_OFFSET;
        yAxisOffset[tool]       = DEFAULT_Y_AXIS_OFFSET;
        materialFudge[tool]     = DEFAULT_MATERIAL_FUDGE;
    }
    
    processedLayers = 0;
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

    struct stat statBuf;
    if (stat(fileName, &statBuf)) {
        if (errno == ENOENT) {
            return;
        }
    }

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
        while (isalnum(*ptr) || *ptr == '.' || *ptr == '-') ptr++;

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

        bool foundSetting = false;
        // Extruder settings
        for (int tool = 0; tool < MAX_TOOLS; tool++) {
            if (!strncasecmp(optname, "filamentFeedRate", 16) && optname[16] == ('0'+tool)) {
                filamentFeedRate[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "driveGearDiameter", 17) && optname[17] == ('0'+tool)) {
                driveGearDiameter[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "extruderTemp", 12) && optname[12] == ('0'+tool)) {
                extruderTemp[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "nozzleDiameter", 14) && optname[14] == ('0'+tool)) {
                nozzleDiameter[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "filamentDiameter", 16) && optname[16] == ('0'+tool)) {
                filamentDiameter[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "retractionRate", 14) && optname[14] == ('0'+tool)) {
                retractionRate[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "retractionTime", 14) && optname[14] == ('0'+tool)) {
                retractionTime[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "pushBackTime", 12) && optname[12] == ('0'+tool)) {
                pushBackTime[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "xAxisOffset", 11) && optname[11] == ('0'+tool)) {
                xAxisOffset[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "yAxisOffset", 11) && optname[11] == ('0'+tool)) {
                yAxisOffset[tool] = strtod(value, &ptr);
                foundSetting = true;
            } else if (!strncasecmp(optname, "materialFudge", 13) && optname[13] == ('0'+tool)) {
                materialFudge[tool] = strtod(value, &ptr);
                foundSetting = true;
            }
        }
        if (!strcasecmp(optname, "supportTool")) {
            supportTool = strtol(value, &ptr, 10);
            foundSetting = true;

        // Machine settings
        } else if (!strcasecmp(optname, "platformTemp")) {
            platformTemp = strtod(value, &ptr);
            foundSetting = true;
        } else if (!strcasecmp(optname, "minLayerTime")) {
            minLayerTime = strtod(value, &ptr);
            foundSetting = true;

        // Build settings
        } else if (!strcasecmp(optname, "layerThickness")) {
            layerThickness = strtod(value, &ptr);
            foundSetting = true;
        } else if (!strcasecmp(optname, "widthOverHeightRatio")) {
            widthOverHeightRatio = strtod(value, &ptr);
            foundSetting = true;
        } else if (!strcasecmp(optname, "infillDensity")) {
            infillDensity = strtod(value, &ptr);
            foundSetting = true;
        } else if (!strcasecmp(optname, "perimeterShells")) {
            perimeterShells = strtol(value, &ptr, 10);
            foundSetting = true;
        } else if (!strcasecmp(optname, "flatShells")) {
            flatShells = strtol(value, &ptr, 10);
            foundSetting = true;
        } else if (!strcasecmp(optname, "raftLayers")) {
            raftLayers = strtol(value, &ptr, 10);
            foundSetting = true;
        } else if (!strcasecmp(optname, "raftOutset")) {
            raftOutset = strtod(value, &ptr);
            foundSetting = true;
        } else if (!strcasecmp(optname, "infillStyle")) {
            if (!strcasecmp(value,"none")) {
                infillStyle = INFILL_NONE;
            } else if (!strcasecmp(value,"lines")) {
                infillStyle = INFILL_LINES;
            } else if (!strcasecmp(value,"rectangular")) {
                infillStyle = INFILL_RECTANGULAR;
            } else if (!strcasecmp(value,"hexagonal")) {
                infillStyle = INFILL_HEXAGONAL;
            } else {
                cerr << "Bad value '" << value
                     << "' on line " << linenum
                     << " of " << fileName << endl;
                cerr << "Value should be one of 'none', 'lines', 'rectangular', or 'hexagonal'." << endl;
                continue;
            }
            foundSetting = true;
        }

        if (!foundSetting) {
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

    for (int tool = 0; tool < MAX_TOOLS; tool++) {
        fout.precision(2);
        fout << "# Rate to feed the filament, in RPM, for Tool" << tool << endl;
        fout << "filamentFeedRate" << tool << ": " << filamentFeedRate[tool] << endl;
        fout << endl;

        fout.precision(3);
        fout << "# Diameter of the filament, in millimeters, for Tool" << tool << endl;
        fout << "filamentDiameter" << tool << ": " << filamentDiameter[tool] << endl;
        fout << endl;

        fout.precision(2);
        fout << "# Diameter of the filament drive gear, in millimeters, for Tool" << tool << endl;
        fout << "driveGearDiameter" << tool << ": " << driveGearDiameter[tool] << endl;
        fout << endl;

        fout.precision(2);
        fout << "# Diameter of the nozzle hole, in millimeters, for Tool" << tool << endl;
        fout << "nozzleDiameter" << tool << ": " << nozzleDiameter[tool] << endl;
        fout << endl;

        fout.precision(0);
        fout << "# Temperature of the extruder, in celsius, for Tool" << tool << endl;
        fout << "extruderTemp" << tool << ": " << extruderTemp[tool] << endl;
        fout << endl;

        fout.precision(0);
        fout << "# Speed to retract filament, in RPM, for Tool" << tool << endl;
        fout << "retractionRate" << tool << ": " << retractionRate[tool] << endl;
        fout << endl;

        fout.precision(0);
        fout << "# Time to retract filament, in milliseconds, for Tool" << tool << endl;
        fout << "retractionTime" << tool << ": " << retractionTime[tool] << endl;
        fout << endl;

        fout.precision(0);
        fout << "# Time to push back filament, in milliseconds, for Tool" << tool << endl;
        fout << "pushBackTime" << tool << ": " << pushBackTime[tool] << endl;
        fout << endl;

        fout.precision(3);
        fout << "# Nozzle offset from center X, in millimeters, for Tool" << tool << endl;
        fout << "xAxisOffset" << tool << ": " << xAxisOffset[tool] << endl;
        fout << endl;

        fout.precision(3);
        fout << "# Nozzle offset from center Y, in millimeters, for Tool" << tool << endl;
        fout << "yAxisOffset" << tool << ": " << yAxisOffset[tool] << endl;
        fout << endl;

        fout.precision(3);
        fout << "# Material shrinkage fudge factor ratio, for Tool" << tool << endl;
        fout << "materialFudge" << tool << ": " << materialFudge[tool] << endl;
        fout << endl;

        fout << endl;
        fout << endl;
    }

    fout << "# The tool to use when printing rafts and supports." << endl;
    fout << "supportTool: " << supportTool << endl;
    fout << endl;

    fout.precision(3);
    fout << "# Layer thickness, in millimeters." << endl;
    fout << "layerThickness: " << layerThickness << endl;
    fout << endl;

    fout.precision(3);
    fout << "# The ratio of the width to the height of the extrusion." << endl;
    fout << "widthOverHeightRatio: " << widthOverHeightRatio << endl;
    fout << endl;

    fout.precision(3);
    fout << "# The density ratio to be used when infilling." << endl;
    fout << "infillDensity: " << infillDensity << endl;
    fout << endl;

    fout << "# The number of perimeter shell layers to use." << endl;
    fout << "perimeterShells: " << perimeterShells << endl;
    fout << endl;

    fout << "# The number of layers used to seal tops and bottoms." << endl;
    fout << "flatShells: " << flatShells << endl;
    fout << endl;

    fout << "# Number of raft layers to set down. 0=No raft. >1 adds interface layers." << endl;
    fout << "raftLayers: " << raftLayers << endl;
    fout << endl;

    fout.precision(2);
    fout << "# How far to outset raft, in millimeters." << endl;
    fout << "raftOutset: " << raftOutset << endl;
    fout << endl;

    fout.precision(1);
    fout << "# Minimum time to spend on a layer. Slows down printing of small layers." << endl;
    fout << "minLayerTime: " << minLayerTime << endl;
    fout << endl;

    fout << "# Infill pattern style. One of 'none', 'lines', 'rectangular', or 'hexagonal'." << endl;
    fout << "infillStyle: ";
    switch (infillStyle) {
      case INFILL_NONE:
        fout << "none" << endl;
        break;
      case INFILL_LINES:
        fout << "lines" << endl;
        break;
      case INFILL_RECTANGULAR:
        fout << "rectangular" << endl;
        break;
      case INFILL_HEXAGONAL:
        fout << "hexagonal" << endl;
        break;
    }
    fout << endl;

    fout.close();
}




double SlicingContext::standardFeedRate(int tool)
{
    double filamentRadius = 0.5f * filamentDiameter[tool];
    double filamentFeedMmPerMin = filamentFeedRate[tool] * driveGearDiameter[tool] * M_PI;
    double extrusionMm3PerMin = filamentFeedMmPerMin * M_PI * filamentRadius * filamentRadius;
    double extrusionWidthMm = widthOverHeightRatio * layerThickness;
    double extrusionSpeedMmPerMin = extrusionMm3PerMin / (extrusionWidthMm * layerThickness);
    return extrusionSpeedMmPerMin * materialFudge[tool];
}



double SlicingContext::standardFeedRate(int tool, double layerThick)
{
    double filamentRadius = 0.5f * filamentDiameter[tool];
    double filamentFeedMmPerMin = filamentFeedRate[tool] * driveGearDiameter[tool] * M_PI;
    double extrusionMm3PerMin = filamentFeedMmPerMin * M_PI * filamentRadius * filamentRadius;
    double extrusionWidthMm = widthOverHeightRatio * layerThick;
    double extrusionSpeedMmPerMin = extrusionMm3PerMin / (extrusionWidthMm * layerThick);
    return extrusionSpeedMmPerMin * materialFudge[tool];
}



double SlicingContext::feedRateForWidth(int tool, double extrusionWidthMm)
{
    double filamentRadius = 0.5f * filamentDiameter[tool];
    double filamentFeedMmPerMin = filamentFeedRate[tool] * driveGearDiameter[tool] * M_PI;
    double extrusionMm3PerMin = filamentFeedMmPerMin * M_PI * filamentRadius * filamentRadius;
    double extrusionSpeedMmPerMin = extrusionMm3PerMin / (extrusionWidthMm * layerThickness);
    return extrusionSpeedMmPerMin * materialFudge[tool];
}



double SlicingContext::feedRateForWidth(int tool, double extrusionWidthMm, double layerThick)
{
    double filamentRadius = 0.5f * filamentDiameter[tool];
    double filamentFeedMmPerMin = filamentFeedRate[tool] * driveGearDiameter[tool] * M_PI;
    double extrusionMm3PerMin = filamentFeedMmPerMin * M_PI * filamentRadius * filamentRadius;
    double extrusionSpeedMmPerMin = extrusionMm3PerMin / (extrusionWidthMm * layerThick);
    return extrusionSpeedMmPerMin * materialFudge[tool];
}



double SlicingContext::standardExtrusionWidth()
{
    return layerThickness * widthOverHeightRatio;
}



double SlicingContext::standardExtrusionWidth(double layerThick)
{
    return layerThick * widthOverHeightRatio;
}



double SlicingContext::ratioForWidth(double extrusionWidth)
{
    return extrusionWidth / layerThickness;
}



double SlicingContext::ratioForWidth(double extrusionWidth, double layerThick)
{
    return extrusionWidth / layerThick;
}



CarvedSlice* SlicingContext::allocSlice(double Z)
{
    CarvedSlice *slice = NULL;
    slices.push_back(CarvedSlice(layerThickness));
    slice = &slices.back();
    slice->zLayer = Z;
    return slice;
}


// vim: set ts=4 sw=4 nowrap expandtab: settings

