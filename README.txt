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


Usage:
Mandoline is a lightning-fast command-line program that slices 3D files into toolpaths for home-build thermoplastic-extrusion 3D printers like the RepRap or MakerBot. It can currently parse 3D input files in .STL binary or ASCII formats. Output will be in G-Code or .S3G formatted files. (Not yet implemented)

At the moment it doesn't yet output toolpaths of any sort, so it's far less than useful. It's an alpha state progress demo, really.

You can dump SVG files for each layer, for debugging purposes. What is implemented is the basic carve and interior infill. And that's buggy. It won't do multiple shells and insets and rafts and comb yet.

What is implemented, though, is extremely fast. I mean OMGWTFBBQ fast. Don't take my word for it. Download the code and compile it, and slice a complex STL file with it. If comparing it to another slicer, compare the Carve and Infill times.

Usage:
mandoline [OPTIONS] FILE.stl
Options include:

  -i FLOAT    Infill density.  From 0.0 to 1.0.
  -l FLOAT    Layer thickness. (mm)
  -f FLOAT    Filament diameter. (mm)
  -F FLOAT    Filament feedrate. (mm/sec)
  -r FLOAT    Width/Thickness ratio.
  -p INT      Number of perimeter shells. (Not implemented yet)
  -s FLOAT    Scale model by factor.
  -r FLOAT    Rotate by X degrees around Z axis.
  -t INT      Number of Threads in threadpool.
  -d STRING   For every layer, saves an SVG file with the given prefix.



TODO: 
From Revar on 2010-12-30:
Things that are implemented:
 Multithreading.  Uses a thread pool with configurable thread count.
 Simple geometry code.
 CompoundRegion constructive boolean geometry.
 STL reading.  (ASCII and binary)
 Carving.  Slices the STL into CompoundRegions.
 Interior Infill.  Generates rectangular infill paths of appropriate density.
 SVG Dumping.  Generates an SVG file per layer to show what is going on.

Things to do:
Come up with a good calibration scheme.
 Debug the CompoundRegion constructive boolean geometry code.
 Raft/Raftless.
 Inset.  Write the Inset code to inset CompoundRegions, so we can make shells.
 Support.  I want to have it handle multiple tool heads, or same-material support structures.
 Bridging/Surface Infill.  Look ahead and behind to determine what areas need 100% infill.
 PathFinder.  (with Comb) Join and optimize the isolated paths into contiguous tool paths.
 Export. I'd like it to generate either GCode or .s3g files.

When the Inset code is done, and the CompoundRegion union/difference/intersection code is debugged, most of the rest of the to-do list becomes simpler.  Inset obviously needs inset code.  Surface Infill/Bridging uses the boolean geometry code to figure out what needs surfacing.

I've been concentrating on the geometry library and the infill code so far.  If someone wants to take on the Comb, PathFinder and Export subsystems, that'd be awesome.
