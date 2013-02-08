//
//  Defaults.hh
//  Mandoline
//
//  Created by GM on 11/24/10.
//  Copyright 2010 Belfry DevWorks. All rights reserved.
//


#define DEFAULT_FILAMENT_FEED_RATE    2.00    /* rev/sec */
#define DEFAULT_FILAMENT_DIAMETER     1.75    /* mm */
#define DEFAULT_NOZZLE_DIAMETER       0.4     /* mm */
#define DEFAULT_DRIVE_GEAR_DIAMETER  10.58    /* mm */
#define DEFAULT_EXTRUDER_TEMP       220.0     /* Tool0 head temperature in C. */
#define DEFAULT_RETRACTION_RATE      30.0     /* RPM */
#define DEFAULT_RETRACTION_TIME      15       /* milliseconds */
#define DEFAULT_PUSH_BACK_TIME       16       /* milliseconds */
#define DEFAULT_X_AXIS_OFFSET         0.0     /* mm */
#define DEFAULT_Y_AXIS_OFFSET         0.0     /* mm */
#define DEFAULT_MATERIAL_FUDGE        0.85    /* material shrinkage fudge factor */

#define DEFAULT_LAYER_THICKNESS       0.30    /* mm */
#define DEFAULT_WIDTH_OVER_HEIGHT     1.75    /* Width of extruded filament, divided by height */
#define DEFAULT_INFILL_DENSITY        0.25    /* Density of infill pattern.  1.0 = solid.  0.0 = hollow. */
#define DEFAULT_PERIMETER_SHELLS      2       /* Number of layers in vertical shell walls */
#define DEFAULT_FLAT_SHELLS           3       /* Number of layers in horizontal shell walls */
#define DEFAULT_RAFT_LAYERS           3       /* Number of layers in raft under model. */
#define DEFAULT_RAFT_OUTSET           5.0     /* mm to outset the raft. */
#define DEFAULT_MIN_LAYER_TIME       20.0     /* Minimum seconds per layer. */
#define DEFAULT_PLATFORM_TEMP       110.0     /* Build platform temperature in C. */
#define DEFAULT_INFILL_STYLE  INFILL_RECTANGULAR   /* Infill pattern. */
#define DEFAULT_HBP_TOOL              0       /* Tool that heats the build platform. */
#define DEFAULT_MAIN_TOOL             0       /* Tool used to print the main object. */
#define DEFAULT_SUPPORT_TOOL          0       /* Tool used to print supports. */
#define DEFAULT_WORKER_THREADS        8       /* Number of threads to slice with. */

// vim: set ts=4 sw=4 nowrap expandtab: settings

