Mandoline: A faster way to slice 3D models.
Copyright 2010 Belfry Software.  All rights reserved.

Licensing:
  Released under the BSD "2-clause" license.
  Check the LICENSE.txt file for full licensing info.

Description:
  Mandoline is a command-line program that slices 3D files into toolpaths for
  home-build thermoplastic-extrusion 3D printers like the RepRap or MakerBot.
  It can currently parse 3D input files in the .STL binary or ASCII formats.
  Output will be in G-Code or .S3G formatted files.  (Not yet implemented)

Compilation and Installation:
  UNIX, Linux and OS X:
      cd src
      ./configure
      make
      make install

  Windows:
    At this point, there is not yet a GUI wrapper for Mandoline.
    You can compile and use it from the CygWin environment, though.
    You'll need CygWin installed with gcc/g++ compilers and GNUMake.
      cd src
      ./configure
      make
    Then install the mandoline.exe file someplace useful.

    //TODO: verify that it compiles and works under Cygwin.

Calibration:
  Mandoline comes with reasonable starting defaults for slicing files for
  printing on a MakerBot CupcakeCNC with a Mk5 extruder.  Machines vary a
  lot, though, so you'll need to calibrate for your machine.

  //TODO: Come up with a good calibration scheme.

Usage:
  //TODO: document settings.

