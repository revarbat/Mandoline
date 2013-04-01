AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

AM_CXXFLAGS = -I${srcdir}/../BGL

TESTS = \
	Test-G03-T001a.sh \
	Test-G03-T001b.sh \
	Test-G03-T001c.sh \
	Test-G03-T002a.sh \
	Test-G03-T003a.sh \
	Test-G03-T003b.sh \
	Test-G03-T003c.sh \
	Test-G03-T004a.sh \
	Test-G03-T004b.sh \
	Test-G03-T005a.sh \
	Test-G03-T006a.sh \
	Test-G03-T007a.sh \
	Test-G03-T007b.sh \
	Test-G03-T007c.sh \
	Test-G03-T007d.sh \
	Test-G03-T007e.sh \
	Test-G03-T007f.sh \
	Test-G03-T008a.sh \
	Test-G03-T009a.sh \
	Test-G03-T010a.sh \
	Test-G03-T010b.sh \
	Test-G03-T011a.sh \
	Test-G04-T001a.sh \
	Test-G04-T001b.sh \
	Test-G04-T001c.sh \
	Test-G04-T002a.sh \
	Test-G04-T002b.sh \
	Test-G04-T002c.sh \
	Test-G04-T002d.sh \
	Test-G05-T001a.sh \
	Test-G05-T001b.sh \
	Test-G05-T001c.sh \
	Test-G05-T001d.sh \
	Test-G05-T001e.sh \
	Test-G05-T001f.sh \
	Test-G05-T001g.sh \
	Test-G05-T002a.sh \
	Test-G05-T002b.sh \
	Test-G05-T002c.sh \
	Test-G05-T002d.sh \
	Test-G05-T002e.sh \
	Test-G05-T002f.sh

check_PROGRAMS = \
	G03_T001_path \
	G03_T002_path \
	G03_T003_path \
	G03_T004_path \
	G03_T005_path \
	G03_T006_path \
	G03_T007_path \
	G03_T008_path \
	G03_T009_path \
	G03_T010_path \
	G03_T011_path \
	G04_T001_simpleregion \
	G04_T002_simpleregion \
	G05_T001_compoundregion \
	G05_T002_compoundregion

CLEANFILES=result_svgs/*.svg logs/Log-*.txt
DISTCLEANFILES=Test-G*-T*.sh result_svgs logs

G03_T001_path_SOURCES = G03_T001_path.cc
G03_T001_path_LDADD = ../libBGL.la

G03_T002_path_SOURCES = G03_T002_path.cc
G03_T002_path_LDADD = ../libBGL.la

G03_T003_path_SOURCES = G03_T003_path.cc
G03_T003_path_LDADD = ../libBGL.la

G03_T004_path_SOURCES = G03_T004_path.cc
G03_T004_path_LDADD = ../libBGL.la

G03_T005_path_SOURCES = G03_T005_path.cc
G03_T005_path_LDADD = ../libBGL.la

G03_T006_path_SOURCES = G03_T006_path.cc
G03_T006_path_LDADD = ../libBGL.la

G03_T007_path_SOURCES = G03_T007_path.cc
G03_T007_path_LDADD = ../libBGL.la

G03_T008_path_SOURCES = G03_T008_path.cc
G03_T008_path_LDADD = ../libBGL.la

G03_T009_path_SOURCES = G03_T009_path.cc
G03_T009_path_LDADD = ../libBGL.la

G03_T010_path_SOURCES = G03_T010_path.cc
G03_T010_path_LDADD = ../libBGL.la

G03_T011_path_SOURCES = G03_T011_path.cc
G03_T011_path_LDADD = ../libBGL.la


G04_T001_simpleregion_SOURCES = G04_T001_simpleregion.cc
G04_T001_simpleregion_LDADD = ../libBGL.la

G04_T002_simpleregion_SOURCES = G04_T002_simpleregion.cc
G04_T002_simpleregion_LDADD = ../libBGL.la


G05_T001_compoundregion_SOURCES = G05_T001_compoundregion.cc
G05_T001_compoundregion_LDADD = ../libBGL.la

G05_T002_compoundregion_SOURCES = G05_T002_compoundregion.cc
G05_T002_compoundregion_LDADD = ../libBGL.la


# dist_noinst_SCRIPTS = autogen.sh

