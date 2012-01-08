#include <unistd.h>
#include <getopt.h>
#include <BGL.hh>
#include "Defaults.hh"
#include "Stopwatch.hh"
#include "SlicingContext.hh"
#include "Operation.hh"
#include "OpQueue.hh"
#include "CarveOp.hh"
#include "InfillOp.hh"
#include "InsetOp.hh"
#include "RaftOp.hh"
#include "PathFinderOp.hh"
#include "CoolOp.hh"
#include "SvgDumpOp.hh"
#include "GCodeExportOp.hh"

static string inFileName  = "";
static string material    = "ABS";
static double scaling      = 1.0f;
static double rotation     = 0.0f;
static bool   doCenter     = true;
static bool   doDumpSVG    = false;
static int    threadcount  = DEFAULT_WORKER_THREADS;

enum ExportTypes {
    NONE,
    GCODE,
    S3G
} exportType = GCODE;



void usage(const char* arg0, SlicingContext& ctx)
{
    fprintf(stderr, "Usage: %s [OPTIONS] [FILE]\n", arg0);
    fprintf(stderr, "Or   : %s -m MATERIAL [OPTIONS] [FILE]\n", arg0);
    fprintf(stderr, "\t-m STRING\n");
    fprintf(stderr, "\t--material STRING\n");
    fprintf(stderr, "\t            Loads settings for given material. (default ABS)\n");
    fprintf(stderr, "\t-f FLOAT\n");
    fprintf(stderr, "\t--tool0-filament-diam FLOAT\n");
    fprintf(stderr, "\t            Tool0 filament diameter. (default %.2f mm)\n", ctx.filamentDiameter[0]);
    fprintf(stderr, "\t-F FLOAT\n");
    fprintf(stderr, "\t--tool0-feed-rate FLOAT\n");
    fprintf(stderr, "\t            Tool0 filament feedrate. (default %.3f RPM)\n", ctx.filamentFeedRate[0]);
    fprintf(stderr, "\t-d FLOAT\n");
    fprintf(stderr, "\t--tool0-gear-diam FLOAT\n");
    fprintf(stderr, "\t            Tool0 drive gear diameter. (default %.2f mm)\n", ctx.driveGearDiameter[0]);
    fprintf(stderr, "\t-n FLOAT\n");
    fprintf(stderr, "\t--tool0-nozzle-diam FLOAT\n");
    fprintf(stderr, "\t            Tool0 extruder nozzle hole diameter. (default %.2f mm)\n", ctx.nozzleDiameter[0]);
    fprintf(stderr, "\t-t FLOAT\n");
    fprintf(stderr, "\t--tool0-temp FLOAT\n");
    fprintf(stderr, "\t            Tool0 extruder temperature. (default %.1f mm)\n", ctx.extruderTemp[0]);
    fprintf(stderr, "\t-i FLOAT\n");
    fprintf(stderr, "\t--infill FLOAT\n");
    fprintf(stderr, "\t            Infill density. (default %.2f)\n", ctx.infillDensity);
    fprintf(stderr, "\t-l FLOAT\n");
    fprintf(stderr, "\t--layer-thickness FLOAT\n");
    fprintf(stderr, "\t            Slicing layer thickness. (default %.2f mm)\n", ctx.layerThickness);
    fprintf(stderr, "\t-p INT\n");
    fprintf(stderr, "\t--perimeter-shells INT\n");
    fprintf(stderr, "\t            Number of perimeter shell layers. (default %d)\n", ctx.perimeterShells);
    fprintf(stderr, "\t-r INT\n");
    fprintf(stderr, "\t--raft-layers INT\n");
    fprintf(stderr, "\t            Number of Raft layers. (default %d)\n", ctx.raftLayers);
    fprintf(stderr, "\t-w FLOAT\n");
    fprintf(stderr, "\t--width-over-height FLOAT\n");
    fprintf(stderr, "\t            Extrusion width over height ratio. (default %.2f)\n", ctx.widthOverHeightRatio);
    fprintf(stderr, "\t--support-tool INT\n");
    fprintf(stderr, "\t            Tool to use when printing rafts and supports. (default %d)\n", ctx.supportTool);
    fprintf(stderr, "\t--tool0-x-offset FLOAT\n");
    fprintf(stderr, "\t            Tool0 nozzle X-axis offset. (default %.3f mm)\n", ctx.xAxisOffset[0]);
    fprintf(stderr, "\t--tool0-y-offset FLOAT\n");
    fprintf(stderr, "\t            Tool0 nozzle Y-axis offset. (default %.3f mm)\n", ctx.yAxisOffset[0]);
    fprintf(stderr, "\t--tool0-retract-rate FLOAT\n");
    fprintf(stderr, "\t            Tool0 retraction rate. (default %.3f RPM)\n", ctx.retractionRate[0]);
    fprintf(stderr, "\t--tool0-retract-time FLOAT\n");
    fprintf(stderr, "\t            Tool0 retraction time. (default %.0f ms)\n", ctx.retractionTime[0]);
    fprintf(stderr, "\t--tool0-pushback-time FLOAT\n");
    fprintf(stderr, "\t            Tool0 pushback time. (default %.0f ms)\n", ctx.pushBackTime[0]);
    fprintf(stderr, "\t--tool0-material-fudge FLOAT\n");
    fprintf(stderr, "\t            Tool0 material shrinkage fudge factor. (default %.0f ms)\n", ctx.materialFudge[0]);
    fprintf(stderr, "\t--tool1-filament-diam FLOAT\n");
    fprintf(stderr, "\t            Tool1 filament diameter. (default %.2f mm)\n", ctx.filamentDiameter[1]);
    fprintf(stderr, "\t--tool1-feed-rate FLOAT\n");
    fprintf(stderr, "\t            Tool1 filament feedrate. (default %.3f RPM)\n", ctx.filamentFeedRate[1]);
    fprintf(stderr, "\t--tool1-gear-diam FLOAT\n");
    fprintf(stderr, "\t            Tool1 drive gear diameter. (default %.2f mm)\n", ctx.driveGearDiameter[1]);
    fprintf(stderr, "\t--tool1-nozzle-diam FLOAT\n");
    fprintf(stderr, "\t            Tool1 extruder nozzle hole diameter. (default %.2f mm)\n", ctx.nozzleDiameter[1]);
    fprintf(stderr, "\t--tool1-temp FLOAT\n");
    fprintf(stderr, "\t            Tool1 extruder temperature. (default %.1f mm)\n", ctx.extruderTemp[1]);
    fprintf(stderr, "\t--tool1-x-offset FLOAT\n");
    fprintf(stderr, "\t            Tool1 nozzle X-axis offset. (default %.3f mm)\n", ctx.xAxisOffset[1]);
    fprintf(stderr, "\t--tool1-y-offset FLOAT\n");
    fprintf(stderr, "\t            Tool1 nozzle Y-axis offset. (default %.3f mm)\n", ctx.yAxisOffset[1]);
    fprintf(stderr, "\t--tool1-retract-rate FLOAT\n");
    fprintf(stderr, "\t            Tool1 retraction rate. (default %.3f RPM)\n", ctx.retractionRate[1]);
    fprintf(stderr, "\t--tool1-retract-time FLOAT\n");
    fprintf(stderr, "\t            Tool1 retraction time. (default %.0f ms)\n", ctx.retractionTime[1]);
    fprintf(stderr, "\t--tool1-pushback-time FLOAT\n");
    fprintf(stderr, "\t            Tool1 pushback time. (default %.0f ms)\n", ctx.pushBackTime[1]);
    fprintf(stderr, "\t--tool1-material-fudge FLOAT\n");
    fprintf(stderr, "\t            Tool1 material shrinkage fudge factor. (default %.0f ms)\n", ctx.materialFudge[1]);
    fprintf(stderr, "\t--flat-shells INT\n");
    fprintf(stderr, "\t            Number of layers to infill solidly on top and bottom. (default %d)\n", ctx.flatShells);
    fprintf(stderr, "\t--raft-outset FLOAT\n");
    fprintf(stderr, "\t            Millimeters to outset raft around model. (default %.1f).\n", ctx.raftOutset);
    fprintf(stderr, "\t--min-layer-time FLOAT\n");
    fprintf(stderr, "\t            Minimum seconds per layer. (default %.1f).\n", ctx.minLayerTime);
    fprintf(stderr, "\t--infill-none\n");
    fprintf(stderr, "\t            Don't create infill structures.\n");
    fprintf(stderr, "\t--infill-lines\n");
    fprintf(stderr, "\t            Use randomized lines infill pattern.\n");
    fprintf(stderr, "\t--infill-rectangular\n");
    fprintf(stderr, "\t            Use rectangular infill patterns.\n");
    fprintf(stderr, "\t--infill-hexagonal\n");
    fprintf(stderr, "\t            Use hexagonal infill patterns.\n");
    fprintf(stderr, "\t--no-center\n");
    fprintf(stderr, "\t            DON'T center model on platform before slicing.\n");
    fprintf(stderr, "\t--scale FLOAT\n");
    fprintf(stderr, "\t            Scale model.  (default %.4gx)\n", scaling);
    fprintf(stderr, "\t--rotate FLOAT\n");
    fprintf(stderr, "\t            Rotate model about Z axis.  (default %.4g deg)\n", rotation);
    fprintf(stderr, "\t--threads INT\n");
    fprintf(stderr, "\t            Number of threads to slice with. (default %d)\n", threadcount);
    fprintf(stderr, "\t--dump-prefix PREFIX\n");
    fprintf(stderr, "\t            Dump layers to SVG files with names like PREFIX-12.34.svg.\n");
    fprintf(stderr, "\t--write-defaults\n");
    fprintf(stderr, "\t            Writes out global settings, as modified, to the defaults file.\n");
    fprintf(stderr, "\t--write-material STRING\n");
    fprintf(stderr, "\t            Writes out material settings, as modified, to the given material file.\n");
    exit(-1);
}


#define OPT_RAFT_OUTSET           1001
#define OPT_MIN_LAYER_TIME        1002
#define OPT_FLAT_SHELLS           1003
#define OPT_WRITE_DEFAULTS        1005
#define OPT_WRITE_MATERIAL        1006
#define OPT_NO_CENTER             1007
#define OPT_THREADS               1008
#define OPT_DUMP_PREFIX           1009
#define OPT_SCALE                 1010
#define OPT_ROTATE                1011
#define OPT_TOOL1_FEED_RATE       1012
#define OPT_TOOL1_FILAMENT_DIAM   1013
#define OPT_TOOL1_GEAR_DIAM       1014
#define OPT_TOOL1_NOZZLE_DIAM     1015
#define OPT_TOOL1_TEMP            1016
#define OPT_SUPPORT_TOOL          1017
#define OPT_TOOL1_X_OFFSET        1018
#define OPT_TOOL1_Y_OFFSET        1019
#define OPT_TOOL0_X_OFFSET        1020
#define OPT_TOOL0_Y_OFFSET        1021
#define OPT_TOOL0_RETRACT_RATE    1022
#define OPT_TOOL0_RETRACT_TIME    1023
#define OPT_TOOL0_PUSHBACK_TIME   1024
#define OPT_TOOL1_RETRACT_RATE    1025
#define OPT_TOOL1_RETRACT_TIME    1026
#define OPT_TOOL1_PUSHBACK_TIME   1027
#define OPT_TOOL0_MATERIAL_FUDGE  1028
#define OPT_TOOL1_MATERIAL_FUDGE  1029
#define OPT_INFILL_NONE           1030
#define OPT_INFILL_LINES          1031
#define OPT_INFILL_RECTANGULAR    1032
#define OPT_INFILL_HEXAGONAL      1033


int main (int argc, char * const argv[])
{
    Stopwatch stopwatch;
    stopwatch.start();
    
    SlicingContext ctx;
    char buf[512];

    int ch;
    const char *progName = argv[0];
    const char * shortopts = "?d:f:F:hi:l:m:n:p:r:w:";
    static struct option longopts[] = {
	{"material",             required_argument, NULL, 'm'},
	{"tool0-feed-rate",      required_argument, NULL, 'F'},
	{"tool0-filament-diam",  required_argument, NULL, 'f'},
	{"tool0-gear-diam",      required_argument, NULL, 'd'},
	{"tool0-nozzle-diam",    required_argument, NULL, 'n'},
	{"tool0-temp",           required_argument, NULL, 't'},
	{"tool0-x-offset",       required_argument, NULL, OPT_TOOL0_X_OFFSET},
	{"tool0-y-offset",       required_argument, NULL, OPT_TOOL0_Y_OFFSET},
	{"tool0-retract-rate",   required_argument, NULL, OPT_TOOL0_RETRACT_RATE},
	{"tool0-retract-time",   required_argument, NULL, OPT_TOOL0_RETRACT_TIME},
	{"tool0-pushback-time",  required_argument, NULL, OPT_TOOL0_PUSHBACK_TIME},
	{"tool0-material-fudge", required_argument, NULL, OPT_TOOL0_MATERIAL_FUDGE},
	{"tool1-feed-rate",      required_argument, NULL, OPT_TOOL1_FEED_RATE},
	{"tool1-filament-diam",  required_argument, NULL, OPT_TOOL1_FILAMENT_DIAM},
	{"tool1-gear-diam",      required_argument, NULL, OPT_TOOL1_GEAR_DIAM},
	{"tool1-nozzle-diam",    required_argument, NULL, OPT_TOOL1_NOZZLE_DIAM},
	{"tool1-temp",           required_argument, NULL, OPT_TOOL1_TEMP},
	{"tool1-x-offset",       required_argument, NULL, OPT_TOOL1_X_OFFSET},
	{"tool1-y-offset",       required_argument, NULL, OPT_TOOL1_Y_OFFSET},
	{"tool1-retract-rate",   required_argument, NULL, OPT_TOOL1_RETRACT_RATE},
	{"tool1-retract-time",   required_argument, NULL, OPT_TOOL1_RETRACT_TIME},
	{"tool1-pushback-time",  required_argument, NULL, OPT_TOOL1_PUSHBACK_TIME},
	{"tool1-material-fudge", required_argument, NULL, OPT_TOOL1_MATERIAL_FUDGE},
	{"support-tool",         required_argument, NULL, OPT_SUPPORT_TOOL},
	{"infill",               required_argument, NULL, 'i'},
	{"layer-thickness",      required_argument, NULL, 'l'},
	{"perimeter-shells",     required_argument, NULL, 'p'},
	{"width-over-height",    required_argument, NULL, 'w'},
	{"raft-layers",          required_argument, NULL, 'r'},
	{"platform-temp",        required_argument, NULL, 'T'},
	{"infill-none",          no_argument,       NULL, OPT_INFILL_NONE},
	{"infill-lines",         no_argument,       NULL, OPT_INFILL_LINES},
	{"infill-rectangular",   no_argument,       NULL, OPT_INFILL_RECTANGULAR},
	{"infill-hexagonal",     no_argument,       NULL, OPT_INFILL_HEXAGONAL},
	{"scale",                required_argument, NULL, OPT_SCALE},
	{"rotate",               required_argument, NULL, OPT_ROTATE},
	{"dump-prefix",          required_argument, NULL, OPT_DUMP_PREFIX},
	{"threads",              required_argument, NULL, OPT_THREADS},
	{"raft-outset",          required_argument, NULL, OPT_RAFT_OUTSET},
	{"min-layer-time",       required_argument, NULL, OPT_MIN_LAYER_TIME},
	{"flat-shells",          required_argument, NULL, OPT_FLAT_SHELLS},
	{"no-center",            no_argument,       NULL, OPT_NO_CENTER},
	{"write-defaults",       no_argument,       NULL, OPT_WRITE_DEFAULTS},
	{"write-material",       required_argument, NULL, OPT_WRITE_MATERIAL},
	{0, 0, 0, 0}
    };
    
    // We need to get the command-line argument for Material first.
    while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        if (ch == 'm') {
	    material = optarg;
	}
    }

    const char *homeDir = getenv("HOME");
    if (!homeDir) {
        homeDir = "";
    }

    // Load default settings.
    // TODO: vary filename path per OS platform.
    snprintf(buf, sizeof(buf), "%s/.mandoline/defaults.conf", homeDir);
    ctx.loadSettingsFromFile(buf);

    // Load saved settings for the given material, if any.
    // TODO: vary filename path per OS platform.
    snprintf(buf, sizeof(buf), "%s/.mandoline/materials/%.128s.conf", homeDir, material.c_str());
    ctx.loadSettingsFromFile(buf);

    // Reset getopt_long so we can process the remainder of
    // the command-line arguments.
    opterr = optind = 1;
#if HAVE_DECL_OPTRESET
    optreset = 1;
#endif

    while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        switch (ch) {
        case 'm':
            // We already parsed this one out.  Ignore.
            break;

        case 'F':
            ctx.filamentFeedRate[0] = atof(optarg);
            break;

        case 'f':
            ctx.filamentDiameter[0] = atof(optarg);
            break;

        case 'd':
            ctx.driveGearDiameter[0] = atof(optarg);
            break;

        case 'n':
            ctx.nozzleDiameter[0] = atof(optarg);
            break;

        case 't':
            ctx.extruderTemp[0] = atof(optarg);
            break;

        case 'T':
            ctx.platformTemp = atof(optarg);
            break;

        case 'i':
            ctx.infillDensity = atof(optarg);
            break;

        case 'l':
            ctx.layerThickness = atof(optarg);
            break;

        case 'p':
            ctx.perimeterShells = atoi(optarg);
            break;

        case 'w':
            ctx.widthOverHeightRatio = atof(optarg);
            break;

        case 'r':
            ctx.raftLayers = atoi(optarg);
            break;

        case OPT_TOOL0_X_OFFSET:
            ctx.xAxisOffset[0] = atof(optarg);
            break;

        case OPT_TOOL0_Y_OFFSET:
            ctx.yAxisOffset[0] = atof(optarg);
            break;

        case OPT_TOOL0_RETRACT_RATE:
            ctx.retractionRate[0] = atof(optarg);
            break;

        case OPT_TOOL0_RETRACT_TIME:
            ctx.retractionTime[0] = atof(optarg);
            break;

        case OPT_TOOL0_PUSHBACK_TIME:
            ctx.pushBackTime[0] = atof(optarg);
            break;

        case OPT_TOOL0_MATERIAL_FUDGE:
            ctx.materialFudge[0] = atof(optarg);
            break;

        case OPT_TOOL1_FEED_RATE:
            ctx.filamentFeedRate[1] = atof(optarg);
            break;

        case OPT_TOOL1_FILAMENT_DIAM:
            ctx.filamentDiameter[1] = atof(optarg);
            break;

        case OPT_TOOL1_GEAR_DIAM:
            ctx.driveGearDiameter[1] = atof(optarg);
            break;

        case OPT_TOOL1_NOZZLE_DIAM:
            ctx.nozzleDiameter[1] = atof(optarg);
            break;

        case OPT_TOOL1_TEMP:
            ctx.extruderTemp[1] = atof(optarg);
            break;

        case OPT_TOOL1_X_OFFSET:
            ctx.xAxisOffset[1] = atof(optarg);
            break;

        case OPT_TOOL1_Y_OFFSET:
            ctx.yAxisOffset[1] = atof(optarg);
            break;

        case OPT_TOOL1_RETRACT_RATE:
            ctx.retractionRate[1] = atof(optarg);
            break;

        case OPT_TOOL1_RETRACT_TIME:
            ctx.retractionTime[1] = atof(optarg);
            break;

        case OPT_TOOL1_PUSHBACK_TIME:
            ctx.pushBackTime[1] = atof(optarg);
            break;

        case OPT_TOOL1_MATERIAL_FUDGE:
            ctx.materialFudge[1] = atof(optarg);
            break;

        case OPT_RAFT_OUTSET:
            ctx.raftOutset = atof(optarg);
            break;

        case OPT_MIN_LAYER_TIME:
            ctx.minLayerTime = atof(optarg);
            break;

        case OPT_FLAT_SHELLS:
            ctx.flatShells = atoi(optarg);
            break;

        case OPT_INFILL_NONE:
            ctx.infillStyle = INFILL_NONE;
            break;

        case OPT_INFILL_LINES:
            ctx.infillStyle = INFILL_LINES;
            break;

        case OPT_INFILL_RECTANGULAR:
            ctx.infillStyle = INFILL_RECTANGULAR;
            break;

        case OPT_INFILL_HEXAGONAL:
            ctx.infillStyle = INFILL_HEXAGONAL;
            break;

        case OPT_SUPPORT_TOOL:
            ctx.supportTool = atoi(optarg);
            break;

        case OPT_NO_CENTER:
            doCenter = false;
            break;

        case OPT_ROTATE:
            rotation = atof(optarg);
            break;

        case OPT_SCALE:
            scaling = atof(optarg);
            break;

        case OPT_THREADS:
	    {
		int cnt = atoi(optarg);
		if (cnt < 1) {
		    fprintf(stderr, "Error: Thread count cannot be less than 1.\n");
		}
		threadcount = cnt;
	    }
            break;

        case OPT_DUMP_PREFIX:
            doDumpSVG = true;
            ctx.dumpPrefix = optarg;
            break;

        case OPT_WRITE_DEFAULTS:
	    // TODO: vary filename path per OS platform.
	    snprintf(buf, sizeof(buf), "%s/.mandoline/defaults.conf", homeDir);
	    ctx.saveSettingsToFile(buf);
	    cerr << "Wrote defaults to " << buf << endl;
            break;

        case OPT_WRITE_MATERIAL:
	    // TODO: vary filename path per OS platform.
	    snprintf(buf, sizeof(buf), "%s/.mandoline/materials/%.128s.conf", homeDir, optarg);
	    ctx.saveSettingsToFile(buf);
	    cerr << "Wrote material settings to " << buf << endl;
            break;

        case '?':
        case 'h':
        default:
            usage(progName, ctx);
        }
    }
    argc -= optind;
    argv += optind;
    if (argc > 0) {
        inFileName = string(argv[0]);
    }
    if (inFileName.length() < 1) {
	cerr << "No file to slice.  Exiting." << endl;
	exit(0);
    }
    
    // Load the model from the file.
    BGL::Mesh3d &mesh = ctx.mesh;
    mesh.loadFromSTLFile(inFileName.c_str());
    printf("Found %d faces.\n", mesh.size());
    stopwatch.checkpoint("Model loaded from file");
    printf("Model Bounds = (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f)\n", mesh.minX, mesh.minY, mesh.minZ, mesh.maxX, mesh.maxY, mesh.maxZ);
    
    // Scale the model if requested.
    if (scaling != 1.0f) {
        printf("Scaling model by %.4gx\n", scaling);
        mesh.scale(scaling);
    }

    // Rotate the model if requested.
    if (rotation != 0.0f) {
        printf("Rotating model by %.4g degrees\n", rotation);
        mesh.rotateZ(rotation*M_PI/180.0f);
    }

    // Center the model.
    if (doCenter) {
        printf("Centering model on X=0, Y=0  Placing bottom at Z=0.\n");
        mesh.translateToCenterOfPlatform();
    }    
    
    if (scaling != 1.0f || rotation != 0.0f || doCenter) {
	printf("New Bounds = (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f)\n", mesh.minX, mesh.minY, mesh.minZ, mesh.maxX, mesh.maxY, mesh.maxZ);
        stopwatch.checkpoint("Transformed");
    }
    
    // Recalculate SVG offsets.
    ctx.calculateSvgOffsets();
    
    // Calculate first and last layer Zs
    printf("Layer Thickness=%.4g\n", ctx.layerThickness);
    double halfLayer = ctx.layerThickness/2.0;
    double z = halfLayer;
    double topZ = mesh.maxZ;

    printf("Extrusion Width=%.4g\n", ctx.standardExtrusionWidth());
    
    // Set up Operations Queue and threadpool.
    OpQueue opQ;
    opQ.setMaxConcurrentOperationCount(threadcount);

    // Carve model to find layer outlines
    while (z < topZ) {
        CarveOp* op = new CarveOp(&ctx, ctx.allocSlice(z+halfLayer), z);
	opQ.addOperation(op);
        z += ctx.layerThickness;
    }
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Carved");
    
    list<CarvedSlice>::iterator it;

    // Inset each level's carved region
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        InsetOp* op = new InsetOp(&ctx, &(*it));
	opQ.addOperation(op);
    }
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Inset");
    
    // Infill each level's carved region
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        InfillOp* op = new InfillOp(&ctx, &(*it));
	opQ.addOperation(op);
    }
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Infilled");
    
    // Create the raft. This isn't easily parallelizable.
    if (ctx.raftLayers > 0) {
	RaftOp* raftOp = new RaftOp(&ctx);
	opQ.addOperation(raftOp);
	opQ.waitUntilAllOperationsAreFinished();
	delete raftOp;
	stopwatch.checkpoint("Rafted");
    }
    
    // Find optimized path.
    PathFinderOp* pathOp = new PathFinderOp(&ctx);
    opQ.addOperation(pathOp);
    opQ.waitUntilAllOperationsAreFinished();
    delete pathOp;
    stopwatch.checkpoint("Path Optimized");
    
    // Figure out the speed to let each layer cool down.
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        CoolOp* op = new CoolOp(&ctx, &(*it));
	opQ.addOperation(op);
    }
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Cooled");
    
    // Optionally dump to SVG
    if (doDumpSVG) {
	for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
	    SvgDumpOp* op = new SvgDumpOp(&ctx, &(*it));
	    opQ.addOperation(op);
        }
	opQ.waitUntilAllOperationsAreFinished();
        stopwatch.checkpoint("Dumped to SVG");
    }

    // Export toolpaths to apropriate output format.
    Operation* expOp = NULL;
    switch (exportType) {
    case GCODE:
        expOp = new GCodeExportOp(&ctx);
        break;
    default:
        break;
    }
    if (expOp) {
	opQ.addOperation(expOp);
	opQ.waitUntilAllOperationsAreFinished();
	stopwatch.checkpoint("Exported");
    }

    // All done!
    stopwatch.finish();
    
    exit(0);
    return 0;
}

