# OwlProgram
Dynamically loaded OWL program.

OwlProgram is the patch build environment for the OWL Pedal, OWL Modular, Magus, Wizard and Alchemist products from Rebel Technology. For details see https://www.rebeltech.org (moved from http://hoxtonowl.com).

It is used to build, run and store patches written in PD, FAUST, Max Gen and C++.

# Instructions

## Prerequisites
* gcc arm compiler (to make patch binary) [[1]](#ref1)
* FirmwareSender (to make sysex and run) [[2]](#ref2)
* emcc (to make web) [[3]](#ref3)
* faust2owl (to compile FAUST patches) [[4]](#ref4)
* The heavy hvcc compiler (to compile PD patches) [[5]](#ref5)

* On Windows, you'll need a MAKE utility [[6]](#ref6).  You'll also need to open common.mk and point TOOLROOT to your gcc installation directory, using a path string without spaces (such as using 8.3 filenames).

## Preparing the environment
This has been done on a Ubuntu server 18.04

### Base tools

    $ sudo apt install build-essential
    $ sudo apt install make gcc-arm-none-eabi
    $ git clone https://github.com/pingdynasty/OwlProgram

edit OwlProgram/common.mk and point TOOLROOT to where arm-none-eabi binaries are installed

    TOOLROOT ?= /usr/bin/ 

to compile puredata patches you need hvcc

    $ sudo apt install python python-enum34 python-jinja2 python-nose2
    $ git clone https://github.com/enzienaudio/hvcc OwlProgram/Tools/hvcc
    
compile FirmwareSender 

FirmwareSender makes possible to use `run` and `store` make targets or by 
invoking it directly to run/store a compiled patch to the device using sysex 
codes. 

    $ sudo apt install libasound2-dev libcurl4-openssl-dev pkg-config
    $ git clone https://github.com/pingdynasty/FirmwareSender
    $ cd FirmwareSender/Builds/Linux
    $ make
    $ cp build/FirmwareSender ../../../OwlProgram/Tools
    $ cd ../../../
   
## Make targets
* make patch: build patch binary
* make sysex: package binary as sysex
* make run: upload patch to attached OWL
* make store: upload and save to attached OWL
* make web: build Javascript patch
* make clean: remove intermediary and target files
* make realclean: remove all (library+patch) intermediary and target files
* make size: show binary size metrics and large object summary
* make map: build map file (Build/patch.map)
* make as: build assembly file (Build/patch.s)
* make help: print target information
* make docs: generate HTML documentation

All files for a patch must be copied in the `PatchSource` directory, take care to put only files related to the one patch you want to compile here.

Make sure to do a `make clean` before compiling a new patch, or add `clean` to your make target, otherwise the previous patch name will be retained.

## Make options
* PATCHNAME: specify name of patch, e.g. SimpleDelay
* PATCHCLASS: name of patch class, e.g. SimpleDelayPatch
* PATCHFILE: name of main patch file, e.g. SimpleDelayPatch.hpp
* PATCHIN: number of input channels, default 2
* PATCHOUT: number of output channels, default 2
* SLOT: user program slot to store patch in, default 0
* TARGET: changes the output prefix, default 'patch'
* PLATFORM: Alchemist, Wizard, Prism, Magus, Player, default 'OWL'

If you follow the convention of SimpleDelay then you don't have to specify `PATCHCLASS` and `PATCHFILE`, they will be deduced from `PATCHNAME`.
t
Note that when storing user programs, the current OWL firmware has four user defined patches, numbered 37 to 40. These correspond to slot number 0 to 3.

## Building C++ patches
First copy all patch files to `PatchSource` folder, then issue the appropriate make command.

Example: Compile and run the TestTone patch, defined in file `PatchSource/TestTonePatch.hpp` as class `TestTonePatch`:
`make PATCHNAME=TestTone run`

Example: Compile and run in browser
`make PATCHNAME=TestTone web`
Then open `Build/web/patch.html`

## Building FAUST patches
To compile and run a FAUST patch
* copy .dsp file and dependencies into `PatchSource`, e.g. `LowShelf.dsp`
* `make FAUST=LowShelf run`

Note: assign OWL parameters with slider metadata: `[OWL:A]`, `[OWL:B]` et c. For example:
```gain = vslider("gain[OWL:C]", 1,0,1,0.1);```. Assign push button with e.g. ```gate = button("gate[OWL:Push]");```

## Building Pure Data patches

* put your PD patch file (e.g. `Foo.pd`) into `PatchSource`
* `make HEAVY=Foo run`
   
Note: assign OWL parameters with PD receivers called `Channel-A`, `Channel-B`, etc.

## Building Max Gen patches
Requires the `.cpp` and `.h` files of a Gen patch generated by Max Gen.

To compile and run a Gen patch called `Foo`:
* copy `Foo.cpp` and `Foo.h` into `PatchSource`
* `make GEN=Foo run`

Note: use OWL parameters in Gen with parameter names: `A`, `B`, `C`, `D`, `Exp`, and `Push`.

## Using FirmwareSender

If you prefer to build the patch first and send it later to the device you can 
do it like this from the main directory of OwlProgram (this will store in slot 6)

    Tools/FirmwareSender -in ./Build/patch.bin -out "OWL-MIDI*" -store 6

# Examples

Compile the puredata file owl_hypersaw.pd[[7]](#ref7) for magus platform:

    make HEAVY=owl_hypersaw PLATFORM=Magus clean patch


Compile puredata file owl_hypersaw.pd for magus platform and send to device to be run immediately:

    make HEAVY=owl_hypersaw PLATFORM=Magus clean run

Compile KickBox[[8]](#ref8) C++ patch for for magus platform:

    make PATCHNAME=KickBox PLATFORM=Magus clean patch
    

# References
<a name="ref1">[1]</a> https://developer.arm.com/open-source/gnu-toolchain/gnu-rm

<a name="ref2">[2]</a> https://github.com/pingdynasty/FirmwareSender/releases

<a name="ref3">[3]</a> http://emscripten.org

<a name="ref4">[4]</a> http://faust.grame.fr/

<a name="ref5">[5]</a> https://github.com/enzienaudio/hvcc

<a name="ref6">[6]</a> http://sourceforge.net/projects/mingw/

<a name="ref7">[7]</a> https://www.rebeltech.org/patch-library/patch/4_saw

<a name="ref8">[8]</a> https://github.com/marsus/MyPatches (AudioDisplay.hpp, BassDrum.hpp, Cymbal.hpp, Drum.hpp, KickBoxPatch.hpp, Oscillators.hpp, Sequence.h, SynthVoice.hpp, bjorklund.h)

