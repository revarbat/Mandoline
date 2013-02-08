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
#include "FindOverhangsOp.hh"
#include "PathFinderOp.hh"
#include "CoolOp.hh"
#include "SvgDumpOp.hh"
#include "GCodeExportOp.hh"

static string inFileName   = "";
static string outFile      = "";
static string material     = "ABS";
static double scaling      = 1.0f;
static double rotation     = 0.0f;
static bool   doCenter     = true;
static bool   doDumpSVG    = false;
static bool   doProgress   = true;
static int    threadcount  = DEFAULT_WORKER_THREADS;

const int PROGRESS_POLL_MICROS = 250000;


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
    fprintf(stderr, "\t--infill-density FLOAT\n");
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
    fprintf(stderr, "\t-o FILENAME\n");
    fprintf(stderr, "\t--outfile FILENAME\n");
    fprintf(stderr, "\t            Writes the resulting output to the given file. (default <INFILE>.gcode)\n");
    fprintf(stderr, "\t-P\n");
    fprintf(stderr, "\t--no-progress\n");
    fprintf(stderr, "\t            Don't show percentage progress per slicing stage.\n");
    fprintf(stderr, "\t--hbp-tool INT\n");
    fprintf(stderr, "\t            Tool that heats the build platform . (default %d)\n", ctx.hbpTool);
    fprintf(stderr, "\t--main-tool INT\n");
    fprintf(stderr, "\t            Tool to use when printing the main object. (default %d)\n", ctx.mainTool);
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


typedef enum {
    OPT_MATERIAL            = 'm',
    OPT_HELP                = 'h',
    OPT_INFILL_DENSITY      = 'i',
    OPT_LAYER_THICKNESS     = 'l',
    OPT_OUTFILE             = 'o',
    OPT_PERIMETER_SHELLS    = 'p',
    OPT_NO_PROGRESS         = 'P',
    OPT_PLATFORM_TEMP       = 'T',
    OPT_RAFT_LAYERS         = 'r',
    OPT_TOOL0_FEED_RATE     = 'F',
    OPT_TOOL0_FILAMENT_DIAM = 'f',
    OPT_TOOL0_GEAR_DIAM     = 'd',
    OPT_TOOL0_NOZZLE_DIAM   = 'n',
    OPT_TOOL0_TEMP          = 't',
    OPT_WIDTH_OVER_HEIGHT   = 'w',

    OPT_DUMP_PREFIX = 1000,
    OPT_FLAT_SHELLS,
    OPT_INFILL_HEXAGONAL,
    OPT_INFILL_LINES,
    OPT_INFILL_NONE,
    OPT_INFILL_RECTANGULAR,
    OPT_MIN_LAYER_TIME,
    OPT_NO_CENTER,
    OPT_RAFT_OUTSET,
    OPT_ROTATE,
    OPT_SCALE,
    OPT_MAIN_TOOL,
    OPT_HBP_TOOL,
    OPT_SUPPORT_TOOL,
    OPT_THREADS,
    OPT_TOOL0_MATERIAL_FUDGE,
    OPT_TOOL0_PUSHBACK_TIME,
    OPT_TOOL0_RETRACT_RATE,
    OPT_TOOL0_RETRACT_TIME,
    OPT_TOOL0_X_OFFSET,
    OPT_TOOL0_Y_OFFSET,
    OPT_TOOL1_FEED_RATE,
    OPT_TOOL1_FILAMENT_DIAM,
    OPT_TOOL1_GEAR_DIAM,
    OPT_TOOL1_MATERIAL_FUDGE,
    OPT_TOOL1_NOZZLE_DIAM,
    OPT_TOOL1_PUSHBACK_TIME,
    OPT_TOOL1_RETRACT_RATE,
    OPT_TOOL1_RETRACT_TIME,
    OPT_TOOL1_TEMP,
    OPT_TOOL1_X_OFFSET,
    OPT_TOOL1_Y_OFFSET,
    OPT_WRITE_DEFAULTS,
    OPT_WRITE_MATERIAL,
} OptionValues;



void showProgressUntilDone(const char* opName, OpQueue &opQ, int expectedLayers)
{
    const char *bar = "==================================================";
    int remainingLayers;
    if (doProgress) {
        for (;;) {
            if (opQ.waitUntilAllOperationsAreFinishedOrTimeout(PROGRESS_POLL_MICROS) != ETIMEDOUT) {
                break;
            }
            remainingLayers = opQ.operationsRemaining();
            double pctDone = (expectedLayers - remainingLayers) * 100.0 / expectedLayers;
            printf("\r%-12s |%-50.*s| %5.1f%%", opName, (int)(pctDone/2.0+0.5), bar, pctDone);
            fflush(stdout);
        }
        printf("\r%78s\r", "");
    }
    opQ.waitUntilAllOperationsAreFinished();
}



void showProcessedProgressUntilDone(const char* opName, SlicingContext &ctx, OpQueue &opQ, int expectedLayers)
{
    const char *bar = "==================================================";
    if (doProgress) {
        for (;;) {
            if (opQ.waitUntilAllOperationsAreFinishedOrTimeout(PROGRESS_POLL_MICROS) != ETIMEDOUT) {
                break;
            }
            double pctDone = ctx.processedLayers * 100.0 / expectedLayers;
            printf("\r%-12s |%-50.*s| %5.1f%%", opName, (int)(pctDone/2.0+0.5), bar, pctDone);
            fflush(stdout);
        }
        printf("\r%78s\r", "");
    }
    opQ.waitUntilAllOperationsAreFinished();
}



int main (int argc, char * const argv[])
{
    Stopwatch stopwatch;
    stopwatch.start();
    
    SlicingContext ctx;
    string outFileName;
    char buf[512];

    enum ImportTypes {
        IMPORT_NONE,
        IMPORT_STL,
        IMPORT_OBJ
    } importType = IMPORT_NONE;

    enum ExportTypes {
        EXPORT_NONE,
        EXPORT_GCODE,
        EXPORT_S3G
    } exportType = EXPORT_GCODE;

    int ch;
    const char *progName = argv[0];
    const char * shortopts = "?d:f:F:hi:l:m:n:o:p:Pr:t:T:w:";
    static struct option longopts[] = {
        {"material",             required_argument, NULL, OPT_MATERIAL},
        {"dump-prefix",          required_argument, NULL, OPT_DUMP_PREFIX},
        {"flat-shells",          required_argument, NULL, OPT_FLAT_SHELLS},
        {"hbp-tool",             required_argument, NULL, OPT_HBP_TOOL},
        {"help",                 no_argument,       NULL, OPT_HELP},
        {"infill-density",       required_argument, NULL, OPT_INFILL_DENSITY},
        {"infill-hexagonal",     no_argument,       NULL, OPT_INFILL_HEXAGONAL},
        {"infill-lines",         no_argument,       NULL, OPT_INFILL_LINES},
        {"infill-none",          no_argument,       NULL, OPT_INFILL_NONE},
        {"infill-rectangular",   no_argument,       NULL, OPT_INFILL_RECTANGULAR},
        {"layer-thickness",      required_argument, NULL, OPT_LAYER_THICKNESS},
        {"main-tool",            required_argument, NULL, OPT_MAIN_TOOL},
        {"min-layer-time",       required_argument, NULL, OPT_MIN_LAYER_TIME},
        {"no-center",            no_argument,       NULL, OPT_NO_CENTER},
        {"outfile",              required_argument, NULL, OPT_OUTFILE},
        {"perimeter-shells",     required_argument, NULL, OPT_PERIMETER_SHELLS},
        {"platform-temp",        required_argument, NULL, OPT_PLATFORM_TEMP},
        {"no-progress",          no_argument,       NULL, OPT_NO_PROGRESS},
        {"raft-layers",          required_argument, NULL, OPT_RAFT_LAYERS},
        {"raft-outset",          required_argument, NULL, OPT_RAFT_OUTSET},
        {"rotate",               required_argument, NULL, OPT_ROTATE},
        {"scale",                required_argument, NULL, OPT_SCALE},
        {"support-tool",         required_argument, NULL, OPT_SUPPORT_TOOL},
        {"threads",              required_argument, NULL, OPT_THREADS},
        {"tool0-feed-rate",      required_argument, NULL, OPT_TOOL0_FEED_RATE},
        {"tool0-filament-diam",  required_argument, NULL, OPT_TOOL0_FILAMENT_DIAM},
        {"tool0-gear-diam",      required_argument, NULL, OPT_TOOL0_GEAR_DIAM},
        {"tool0-material-fudge", required_argument, NULL, OPT_TOOL0_MATERIAL_FUDGE},
        {"tool0-nozzle-diam",    required_argument, NULL, OPT_TOOL0_NOZZLE_DIAM},
        {"tool0-pushback-time",  required_argument, NULL, OPT_TOOL0_PUSHBACK_TIME},
        {"tool0-retract-rate",   required_argument, NULL, OPT_TOOL0_RETRACT_RATE},
        {"tool0-retract-time",   required_argument, NULL, OPT_TOOL0_RETRACT_TIME},
        {"tool0-temp",           required_argument, NULL, OPT_TOOL0_TEMP},
        {"tool0-x-offset",       required_argument, NULL, OPT_TOOL0_X_OFFSET},
        {"tool0-y-offset",       required_argument, NULL, OPT_TOOL0_Y_OFFSET},
        {"tool1-feed-rate",      required_argument, NULL, OPT_TOOL1_FEED_RATE},
        {"tool1-filament-diam",  required_argument, NULL, OPT_TOOL1_FILAMENT_DIAM},
        {"tool1-gear-diam",      required_argument, NULL, OPT_TOOL1_GEAR_DIAM},
        {"tool1-material-fudge", required_argument, NULL, OPT_TOOL1_MATERIAL_FUDGE},
        {"tool1-nozzle-diam",    required_argument, NULL, OPT_TOOL1_NOZZLE_DIAM},
        {"tool1-pushback-time",  required_argument, NULL, OPT_TOOL1_PUSHBACK_TIME},
        {"tool1-retract-rate",   required_argument, NULL, OPT_TOOL1_RETRACT_RATE},
        {"tool1-retract-time",   required_argument, NULL, OPT_TOOL1_RETRACT_TIME},
        {"tool1-temp",           required_argument, NULL, OPT_TOOL1_TEMP},
        {"tool1-x-offset",       required_argument, NULL, OPT_TOOL1_X_OFFSET},
        {"tool1-y-offset",       required_argument, NULL, OPT_TOOL1_Y_OFFSET},
        {"width-over-height",    required_argument, NULL, OPT_WIDTH_OVER_HEIGHT},
        {"write-defaults",       no_argument,       NULL, OPT_WRITE_DEFAULTS},
        {"write-material",       required_argument, NULL, OPT_WRITE_MATERIAL},
        {0, 0, 0, 0}
    };
    
    // We need to get the command-line argument for Material first.
    while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        if (ch == OPT_MATERIAL) {
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
        case OPT_MATERIAL:
            // We already parsed this one out.  Ignore.
            break;

        case OPT_OUTFILE:
            outFile = string(optarg);
            break;

        case OPT_DUMP_PREFIX:
            doDumpSVG = true;
            ctx.dumpPrefix = optarg;
            break;

        case OPT_FLAT_SHELLS:
            ctx.flatShells = atoi(optarg);
            break;

        case OPT_INFILL_DENSITY:
            ctx.infillDensity = atof(optarg);
            break;

        case OPT_INFILL_HEXAGONAL:
            ctx.infillStyle = INFILL_HEXAGONAL;
            break;

        case OPT_INFILL_LINES:
            ctx.infillStyle = INFILL_LINES;
            break;

        case OPT_INFILL_NONE:
            ctx.infillStyle = INFILL_NONE;
            break;

        case OPT_INFILL_RECTANGULAR:
            ctx.infillStyle = INFILL_RECTANGULAR;
            break;

        case OPT_LAYER_THICKNESS:
            ctx.layerThickness = atof(optarg);
            break;

        case OPT_MIN_LAYER_TIME:
            ctx.minLayerTime = atof(optarg);
            break;

        case OPT_NO_CENTER:
            doCenter = false;
            break;

        case OPT_PERIMETER_SHELLS:
            ctx.perimeterShells = atoi(optarg);
            break;

        case OPT_NO_PROGRESS:
            doProgress = false;
            break;

        case OPT_PLATFORM_TEMP:
            ctx.platformTemp = atof(optarg);
            break;

        case OPT_RAFT_LAYERS:
            ctx.raftLayers = atoi(optarg);
            break;

        case OPT_RAFT_OUTSET:
            ctx.raftOutset = atof(optarg);
            break;

        case OPT_ROTATE:
            rotation = atof(optarg);
            break;

        case OPT_SCALE:
            scaling = atof(optarg);
            break;

        case OPT_MAIN_TOOL:
            ctx.mainTool = atoi(optarg);
            break;

        case OPT_HBP_TOOL:
            ctx.hbpTool = atoi(optarg);
            break;

        case OPT_SUPPORT_TOOL:
            ctx.supportTool = atoi(optarg);
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

        case OPT_TOOL0_FEED_RATE:
            ctx.filamentFeedRate[0] = atof(optarg);
            break;

        case OPT_TOOL0_FILAMENT_DIAM:
            ctx.filamentDiameter[0] = atof(optarg);
            break;

        case OPT_TOOL0_GEAR_DIAM:
            ctx.driveGearDiameter[0] = atof(optarg);
            break;

        case OPT_TOOL0_MATERIAL_FUDGE:
            ctx.materialFudge[0] = atof(optarg);
            break;

        case OPT_TOOL0_NOZZLE_DIAM:
            ctx.nozzleDiameter[0] = atof(optarg);
            break;

        case OPT_TOOL0_PUSHBACK_TIME:
            ctx.pushBackTime[0] = atof(optarg);
            break;

        case OPT_TOOL0_RETRACT_RATE:
            ctx.retractionRate[0] = atof(optarg);
            break;

        case OPT_TOOL0_RETRACT_TIME:
            ctx.retractionTime[0] = atof(optarg);
            break;

        case OPT_TOOL0_TEMP:
            ctx.extruderTemp[0] = atof(optarg);
            break;

        case OPT_TOOL0_X_OFFSET:
            ctx.xAxisOffset[0] = atof(optarg);
            break;

        case OPT_TOOL0_Y_OFFSET:
            ctx.yAxisOffset[0] = atof(optarg);
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

        case OPT_TOOL1_MATERIAL_FUDGE:
            ctx.materialFudge[1] = atof(optarg);
            break;

        case OPT_TOOL1_NOZZLE_DIAM:
            ctx.nozzleDiameter[1] = atof(optarg);
            break;

        case OPT_TOOL1_PUSHBACK_TIME:
            ctx.pushBackTime[1] = atof(optarg);
            break;

        case OPT_TOOL1_RETRACT_RATE:
            ctx.retractionRate[1] = atof(optarg);
            break;

        case OPT_TOOL1_RETRACT_TIME:
            ctx.retractionTime[1] = atof(optarg);
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

        case OPT_WIDTH_OVER_HEIGHT:
            ctx.widthOverHeightRatio = atof(optarg);
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
        case OPT_HELP:
        default:
            usage(progName, ctx);
        }
    }
    argc -= optind;
    argv += optind;
    if (argc > 0) {
        inFileName = string(argv[0]);
    }

    size_t iflen = inFileName.length();
    size_t endlen = iflen;
    if (iflen > 4) {
        if (!inFileName.substr(iflen-4,4).compare(".stl") || !inFileName.substr(iflen-4,4).compare(".STL")) {
            importType = IMPORT_STL;
            endlen -= 4;
        } else if (!inFileName.substr(iflen-4,4).compare(".obj") || !inFileName.substr(iflen-4,4).compare(".OBJ")) {
            importType = IMPORT_OBJ;
            endlen -= 4;
        }
    }

    if (inFileName.length() < 1) {
        // cerr << "No file to slice.  Exiting." << endl;
        exit(0);
    } else if (importType == IMPORT_NONE) {
        cerr << "Unrecognized input file format.  Exiting." << endl;
        exit(0);
    }

    if (outFile.length() > 0) {
        outFileName = outFile;
    } else {
        outFileName = inFileName.substr(0,endlen);
    }
    
    // Load the model from the file.
    BGL::Mesh3d &mesh = ctx.mesh;
    switch (importType) {
      case IMPORT_STL:
        mesh.loadFromSTLFile(inFileName.c_str());
        break;
      case IMPORT_OBJ:
        mesh.loadFromOBJFile(inFileName.c_str());
        break;
      case IMPORT_NONE:
      default:
        break;
    }
    stopwatch.checkpoint("Model loaded from file");
    printf("  Found %d faces.\n", mesh.size());
    
    // Scale the model if requested.
    if (scaling != 1.0f) {
        printf("  Scaling model by %.4gx.\n", scaling);
        mesh.scale(scaling);
    }

    // Rotate the model if requested.
    if (rotation != 0.0f) {
        printf("  Rotating model by %.4g degrees.\n", rotation);
        mesh.rotateZ(rotation*M_PI/180.0f);
    }

    // Center the model.
    if (doCenter) {
        printf("  Centering model on X=0, Y=0  Placing bottom at Z=0.\n");
        mesh.translateToCenterOfPlatform();
    }    
    
    if (scaling != 1.0f || rotation != 0.0f || doCenter) {
        stopwatch.checkpoint("Transformed");
    }
    printf("  Final Bounds = (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f)\n", mesh.minX, mesh.minY, mesh.minZ, mesh.maxX, mesh.maxY, mesh.maxZ);
    
    // Recalculate SVG offsets.
    ctx.calculateSvgOffsets();
    
    // Calculate first and last layer Zs
    double halfLayer = ctx.layerThickness/2.0;
    double z = halfLayer;
    double topZ = mesh.maxZ;

    printf("  Layer Thickness=%.4g\n", ctx.layerThickness);
    printf("  Extrusion Width=%.4g\n", ctx.standardExtrusionWidth());
    
    // Set up Operations Queue and threadpool.
    OpQueue opQ;
    opQ.setMaxConcurrentOperationCount(threadcount);

    int expectedLayers = topZ / ctx.layerThickness + 0.5;

    // Carve model to find layer outlines
    while (z < topZ) {
        CarveOp* op = new CarveOp(&ctx, ctx.allocSlice(z+halfLayer), z);
        opQ.addOperation(op);
        z += ctx.layerThickness;
    }
    showProgressUntilDone("Carving", opQ, expectedLayers);
    stopwatch.checkpoint("Carved");
    printf("  Layer Count=%d\n", (int)ctx.slices.size());
    
    list<CarvedSlice>::iterator it;

    // Inset each level's carved region
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        InsetOp* op = new InsetOp(&ctx, &(*it));
        opQ.addOperation(op);
    }
    showProgressUntilDone("Insetting", opQ, expectedLayers);
    stopwatch.checkpoint("Inset");
    
    // Infill each level's carved region
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        InfillOp* op = new InfillOp(&ctx, &(*it));
        opQ.addOperation(op);
    }
    showProgressUntilDone("Infilling", opQ, expectedLayers);
    stopwatch.checkpoint("Infilled");
    
    // Create the raft. This isn't easily parallelizable.
    if (ctx.raftLayers > 0) {
        RaftOp* raftOp = new RaftOp(&ctx);
        opQ.addOperation(raftOp);
        showProcessedProgressUntilDone("Rafting", ctx, opQ, ctx.raftLayers);
        delete raftOp;
        stopwatch.checkpoint("Rafted");
        expectedLayers += ctx.raftLayers;
    }
    
    // Find overhangs
    FindOverhangsOp* overhangsOp = new FindOverhangsOp(&ctx);
    opQ.addOperation(overhangsOp);
    showProcessedProgressUntilDone("Finding Overhangs", ctx, opQ, expectedLayers);
    delete overhangsOp;
    stopwatch.checkpoint("Found Overhangs");
    
    // Find optimized path.
    PathFinderOp* pathOp = new PathFinderOp(&ctx);
    opQ.addOperation(pathOp);
    showProcessedProgressUntilDone("Path Optimizing", ctx, opQ, expectedLayers);
    delete pathOp;
    stopwatch.checkpoint("Optimizing");
    
    // Figure out the speed to let each layer cool down.
    for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
        CoolOp* op = new CoolOp(&ctx, &(*it));
        opQ.addOperation(op);
    }
    showProgressUntilDone("Cooling", opQ, expectedLayers);
    stopwatch.checkpoint("Cooled");
    
    // Optionally dump to SVG
    if (doDumpSVG) {
        for (it = ctx.slices.begin(); it != ctx.slices.end(); it++) {
            SvgDumpOp* op = new SvgDumpOp(&ctx, &(*it));
            opQ.addOperation(op);
        }
        showProgressUntilDone("DumpSVG", opQ, expectedLayers);
        stopwatch.checkpoint("Dumped to SVG");
    }

    // Export toolpaths to apropriate output format.
    Operation* expOp = NULL;
    switch (exportType) {
    case EXPORT_GCODE:
        if (inFileName.substr(iflen-6,6).compare(".gcode") != 0) {
            outFileName.append(".gcode");
        }
        expOp = new GCodeExportOp(&ctx, outFileName);
        break;
    default:
        break;
    }
    if (expOp) {
        opQ.addOperation(expOp);
        showProcessedProgressUntilDone("Exporting", ctx, opQ, expectedLayers);
        delete expOp;
        stopwatch.checkpoint("Exported");
    }

    // All done!
    stopwatch.finish();
    
    exit(0);
    return 0;
}
// vim: set ts=4 sw=4 nowrap expandtab: settings

