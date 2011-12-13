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
#include "SvgDumpOp.hh"
#include "PathFinderOp.hh"
#include "GCodeExportOp.hh"

static string inFileName  = "";
static string material    = "ABS";
static double scaling      = 1.0f;
static double rotation     = 0.0f;
static bool   doCenter     = true;
static double onlyAtZ      = -1.0;
static bool   doDumpSVG    = false;
static int    threadcount  = DEFAULT_WORKER_THREADS;

enum ExportTypes {
    NONE,
    GCODE,
    S3G
} exportType = GCODE;



void usage(const char* arg0, SlicingContext& ctx)
{
    fprintf(stderr, "Usage: %s [OPTIONS] FILE\n", arg0);
    fprintf(stderr, "Or   : %s -m MATERIAL [OPTIONS] FILE\n", arg0);
    fprintf(stderr, "\t[-m STRING]   Extruded material. (default ABS)\n");
    fprintf(stderr, "\t[-f FLOAT]    Filament diameter. (default %.1f mm)\n", ctx.filamentDiameter);
    fprintf(stderr, "\t[-F FLOAT]    Filament feedrate. (default %.3f mm/s)\n", ctx.filamentFeedRate);
    fprintf(stderr, "\t[-i FLOAT]    Infill density. (default %.2f)\n", ctx.infillDensity);
    fprintf(stderr, "\t[-l FLOAT]    Slicing layer thickness. (default %.2f mm)\n", ctx.layerThickness);
    fprintf(stderr, "\t[-p INT]      Number of perimeter shell layers. (default %d)\n", ctx.perimeterShells);
    fprintf(stderr, "\t[-w FLOAT]    Extrusion width over thickness ratio. (default %.2f)\n", ctx.widthOverHeightRatio);
    fprintf(stderr, "\t[-c]          DON'T center model on platform before slicing.\n");
    fprintf(stderr, "\t[-s FLOAT]    Scale model.  (default %.4gx)\n", scaling);
    fprintf(stderr, "\t[-r FLOAT]    Rotate model about Z.  (default %.4g deg)\n", rotation);
    fprintf(stderr, "\t[-z FLOAT]    Slice model only at the given Z level.\n");
    fprintf(stderr, "\t[-d PREFIX]   Dump layers to SVG files with names like PREFIX-12.34.svg.\n");
    fprintf(stderr, "\t[-t INT]      Number of threads to slice with. (default %d)\n", threadcount);
    exit(-1);
}



int main (int argc, char * const argv[])
{
    Stopwatch stopwatch;
    stopwatch.start();
    
    SlicingContext ctx;
    char buf[512];

    int ch;
    const char *progName = argv[0];
    const char * shortopts = "?cd:f:F:hi:l:m:o:p:r:s:t:w:Z:";
    static struct option longopts[] = {
	{"material", required_argument, NULL, 'm'},
	{"diameter", required_argument, NULL, 'f'},
	{"feedrate", required_argument, NULL, 'F'},
	{"infill", required_argument, NULL, 'i'},
	{"layer", required_argument, NULL, 'l'},
	{"shells", required_argument, NULL, 'p'},
	{"ratio", required_argument, NULL, 'w'},
	{"nocenter", required_argument, NULL, 'c'},
	{"scale", required_argument, NULL, 's'},
	{"rotatex", required_argument, NULL, 'r'},
	{"onlyatz", required_argument, NULL, 'Z'},
	{"dumpprefix", required_argument, NULL, 'd'},
	{"threads", required_argument, NULL, 't'},
	{0, 0, 0, 0}
    };
    
    // We need to get the command-line argument for Material first.
    while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        if (ch == 'm') {
	    material = optarg;
	}
    }

    // Load saved settings for the given material, if any.
    snprintf(buf, sizeof(buf), "~/.mandoline/materials/%.128s.conf", material.c_str());
    ctx.loadDefaultsFromFile(buf);

    // Process remainder of command-line arguments.
    opterr = optind = 1;

#if HAVE_DECL_OPTRESET
    optreset = 1;
#endif

    while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        switch (ch) {
        case 'c':
            doCenter = false;
            break;
        case 'd':
            doDumpSVG = true;
            ctx.dumpPrefix = optarg;
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
            rotation = atof(optarg);
            break;
        case 's':
            scaling = atof(optarg);
            break;
        case 't': {
            int cnt = atoi(optarg);
            if (cnt < 1) {
                fprintf(stderr, "Error: Thread count cannot be less than 1.\n");
            }
            threadcount = cnt;
        }
            break;
        case 'Z':
            onlyAtZ = atof(optarg);
            break;
        case 'm':
            // We already parsed this one out.  Ignore.
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
        usage(progName, ctx);
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
    double z = ctx.layerThickness/2.0f;
    double topZ = mesh.maxZ;
    if (onlyAtZ > 0.0f) {
        z = onlyAtZ;
        topZ = onlyAtZ + ctx.layerThickness / 2.0f;
    }

    printf("Extrusion Width=%.4g\n", ctx.standardExtrusionWidth());
    
    // Set up Operations Queue and threadpool.
    OpQueue opQ;
    opQ.setMaxConcurrentOperationCount(threadcount);

    // Carve model to find layer outlines
    while (z < topZ) {
        CarveOp* op = new CarveOp(&ctx, ctx.allocSlice(z), z);
	opQ.addOperation(op);
        z += ctx.layerThickness;
    }
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Carved");
    
    map<double,CarvedSlice>::iterator it;

    // Inset each level's carved region
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        InsetOp* op = new InsetOp(&ctx, &(*it).second, (*it).first);
	opQ.addOperation(op);
    }
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Inset");
    
    // Infill each level's carved region
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        InfillOp* op = new InfillOp(&ctx, &(*it).second, (*it).first);
	opQ.addOperation(op);
    }
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Infilled");
    
    // Optionally dump to SVG
    if (doDumpSVG) {
	for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
	    SvgDumpOp* op = new SvgDumpOp(&ctx, &(*it).second, (*it).first);
	    opQ.addOperation(op);
        }
	opQ.waitUntilAllOperationsAreFinished();
        stopwatch.checkpoint("Dumped to SVG");
    }

    // Find optimized path.
    PathFinderOp* op = new PathFinderOp(&ctx);
    opQ.addOperation(op);
    opQ.waitUntilAllOperationsAreFinished();
    stopwatch.checkpoint("Path Optimized");
    
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
    
    return 0;
}

