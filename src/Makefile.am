AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

SUBDIRS=BGL
AM_CXXFLAGS = -I${srcdir}/BGL

bin_PROGRAMS = mandoline
mandoline_SOURCES = \
	Mandoline.cc \
	CarveOp.cc CarveOp.hh \
	CarvedSlice.cc CarvedSlice.hh \
	CoolOp.cc CoolOp.hh \
	Defaults.hh \
	GCodeExportOp.cc GCodeExportOp.hh \
	InfillOp.cc InfillOp.hh \
	InsetOp.cc InsetOp.hh \
	OpQueue.cc OpQueue.hh \
	OpThread.cc OpThread.hh \
	Operation.hh \
	FindOverhangsOp.cc FindOverhangsOp.hh \
	PathFinderOp.cc PathFinderOp.hh \
	RaftOp.cc RaftOp.hh \
	SlicingContext.cc SlicingContext.hh \
	Stopwatch.cc Stopwatch.hh \
	SvgDumpOp.cc SvgDumpOp.hh
mandoline_LDADD = BGL/libBGL.la

# dist_noinst_SCRIPTS = autogen.sh

