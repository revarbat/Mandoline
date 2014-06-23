Mandoline: A faster way to slice 3D models.
Copyright 2010-2014 Belfry Software.  All rights reserved.

Licensing:
  Released under the BSD "2-clause" license.
  Check the LICENSE.txt file for full licensing info.

Description:
  Mandoline is a command-line program that slices 3D files into toolpaths for
  home-build thermoplastic-extrusion 3D printers like the RepRap or MakerBot.
  It can currently parse 3D input files in the .STL binary or ASCII formats.
  Output is in G-Code formatted files.

Compilation and Installation:
  UNIX, Linux and OS X:
    cd src
    ./configure
    make
    sudo make install

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
  printing on a MakerBot Thingomatic with a Mk7 extruder.  Machines vary a
  lot, though, so you'll need to calibrate for your machine.

Usage:
  mandoline [OPTIONS] FILE
  mandoline -m MATERIAL [OPTIONS] FILE

Options include:
    -m STRING   Extruded material. (default ABS)
    -f FLOAT    Filament diameter. (default 3.0 mm)
    -F FLOAT    Filament feedrate. (default 0.689 mm/s)
    -i FLOAT    Infill density. (default 0.20)
    -l FLOAT    Slicing layer thickness. (default 0.36 mm)
    -p INT      Number of perimeter shell layers. (default 2)
    -r INT      Number of Raft layers. (default 2)
    -w FLOAT    Extrusion width over thickness ratio. (default 1.75)
    -c          DON'T center model on platform before slicing.
    -S FLOAT    Scale model.  (default 1x)
    -R FLOAT    Rotate model about Z.  (default 0 deg)
    -d PREFIX   Dump layers to SVG files with names like PREFIX-12.34.svg.
    -t INT      Number of threads to slice with. (default 8)

Debugging:
  You can dump SVG files for each layer, for debugging purposes. What is
  implemented is the basic carve and interior infill. And that's buggy.
  It won't do cooling or comb or filament retraction yet.

