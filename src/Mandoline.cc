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
    fprintf(stderr, "\t--filament-diam FLOAT\n");
    fprintf(stderr, "\t            Filament diameter. (default %.1f mm)\n", ctx.filamentDiameter);
    fprintf(stderr, "\t-F FLOAT\n");
    fprintf(stderr, "\t--filament-feed-rate FLOAT\n");
    fprintf(stderr, "\t            Filament feedrate. (default %.3f RPM)\n", ctx.filamentFeedRate);
    fprintf(stderr, "\t-d FLOAT\n");
    fprintf(stderr, "\t--drive-gear-diam FLOAT\n");
    fprintf(stderr, "\t            Drive gear diameter. (default %.1f mm)\n", ctx.driveGearDiameter);
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
    fprintf(stderr, "\t--width-over-thickness FLOAT\n");
    fprintf(stderr, "\t            Extrusion width over thickness ratio. (default %.2f)\n", ctx.widthOverHeightRatio);
    fprintf(stderr, "\t--flat-shells INT\n");
    fprintf(stderr, "\t            Number of layers to infill solidly on top and bottom. (default %d)\n", ctx.flatShells);
    fprintf(stderr, "\t--raft-outset FLOAT\n");
    fprintf(stderr, "\t            Millimeters to outset raft around model. (default %.1f).\n", ctx.raftOutset);
    fprintf(stderr, "\t--min-layer-time FLOAT\n");
    fprintf(stderr, "\t            Minimum seconds per layer. (default %.1f).\n", ctx.minLayerTime);
    fprintf(stderr, "\t--shrinkage-ratio FLOAT\n");
    fprintf(stderr, "\t            Ratio of how much the part will shrink when cooled. (default %.3f).\n", ctx.shrinkageRatio);
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


#define OPT_RAFT_OUTSET       1001
#define OPT_MIN_LAYER_TIME    1002
#define OPT_FLAT_SHELLS       1003
#define OPT_SHRINKAGE_RATIO   1004
#define OPT_WRITE_DEFAULTS    1005
#define OPT_WRITE_MATERIAL    1006
#define OPT_NO_CENTER         1007
#define OPT_THREADS           1008
#define OPT_DUMP_PREFIX       1009
#define OPT_SCALE             1010
#define OPT_ROTATE            1011


int main (int argc, char * const argv[])
{
    Stopwatch stopwatch;
    stopwatch.start();
    
    SlicingContext ctx;
    char buf[512];

    int ch;
    const char *progName = argv[0];
    const char * shortopts = "?d:f:F:hi:l:m:p:r:w:";
    static struct option longopts[] = {
	{"material",           required_argument, NULL, 'm'},
	{"drive-gear-diam",    required_argument, NULL, 'd'},
	{"filament-diam",      required_argument, NULL, 'f'},
	{"filament-feed-rate", required_argument, NULL, 'F'},
	{"infill",             required_argument, NULL, 'i'},
	{"layer-thickness",    required_argument, NULL, 'l'},
	{"perimater-shells",   required_argument, NULL, 'p'},
	{"width-over-height",  required_argument, NULL, 'w'},
	{"raft-layers",        required_argument, NULL, 'r'},
	{"scale",              required_argument, NULL, OPT_SCALE},
	{"rotate",             required_argument, NULL, OPT_ROTATE},
	{"dump-prefix",        required_argument, NULL, OPT_DUMP_PREFIX},
	{"threads",            required_argument, NULL, OPT_THREADS},
	{"raft-outset",        required_argument, NULL, OPT_RAFT_OUTSET},
	{"min-layer-time",     required_argument, NULL, OPT_MIN_LAYER_TIME},
	{"flat-shells",        required_argument, NULL, OPT_FLAT_SHELLS},
	{"shrinkage-ratio",    required_argument, NULL, OPT_SHRINKAGE_RATIO},
	{"no-center",          no_argument,       NULL, OPT_NO_CENTER},
	{"write-defaults",     no_argument,       NULL, OPT_WRITE_DEFAULTS},
	{"write-material",     required_argument, NULL, OPT_WRITE_MATERIAL},
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

        case 'd':
            ctx.driveGearDiameter = atof(optarg);
            break;

        case 'f':
            ctx.filamentDiameter = atof(optarg);
            break;

        case 'F':
            ctx.filamentFeedRate = atof(optarg);
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

        case OPT_ROTATE:
            rotation = atof(optarg);
            break;

        case OPT_SCALE:
            scaling = atof(optarg);
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

        case OPT_SHRINKAGE_RATIO:
            ctx.shrinkageRatio = atoi(optarg);
            break;

        case OPT_NO_CENTER:
            doCenter = false;
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

