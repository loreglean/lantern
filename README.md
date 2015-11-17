Lantern is a cross-plaftorm 3D software renderer written for educational purposes. It depends of multiple libraries (full list is given below) but it doesn't use them for rendering (except using FreeType library for getting glyphs bitmaps), only for common tasks (such as presenting color buffer on a screen, handling input, loading textures from disk etc.). The aim is to create feature-full renderer from scratch, only using ability to set texture's pixel color.

Note that because it doesn't use GPU, it is much slower comparing to DirectX or OpenGL, and, obviously, is not intended for using in real projects (unless you're targeting users without video cards!).

###Implemented features

* Loading .obj files
* Rasterization using three different algorithms: inversed slope, traversal (and its subtypes: aabb, backtracking, zigzag), homogeneous
* Programmable vertex and pixel shaders
* Perspective-correct attributes interpolation
* Texture mapping
* Alpha-blending
* Truetype fonts rendering

###Dependencies

* SDL2 - used for creating windows, copying resulting texture data to a screen, handling input and system events
* SDL2_Image - used for loading images files
* FreeType - used for loading truetype fonts, calculating their metrics and rendering glyphs bitmaps
* Google C++ Tests - used for testing

###Building

Lantern uses CMake as its build system.

Simple scripts were created to simplify building process a little, though (output goes to `<lantern>/build` folder):
* `build_vs14.bat` - creates VS solution for Visual Studio 2015
* `build_vs12.bat` - creates VS solution for Visual Studio 2013
* `build_mingw_make.bat` - creates MinGW makefiles
* `build_make.sh` - creates Linux makefiles
* `build_xcode.sh` - creates XCode project

On Linux and MacOS you probably have to `chmod +x build_*.sh` before running them.

You also have to have all the dependencies and tools installed (obviously). As an example for Ubuntu, here are the commands for installing all of them (just skip what you don't need):
 * `sudo apt-get install cmake`
 * `sudo apt-get install g++`
 * `sudo apt-get install libsdl2-dev`
 * `sudo apt-get install libsdl2-image-dev`
 * `sudo apt-get install libfreetype6-dev`
 * `sudo apt-get install libgtest-dev`

On Windows you have to specify a few environment variables for dependencies so that CMake's `find_package` will be able to find them:
 * `SDL2DIR` - path to SDL2 development library
 * `SDL2IMAGEDIR` - path to SDL2_Image development library
 * `FREETYPEDIR` - path to a folder containing FreeType's headers and library
 * `GTEST_ROOT` - path to Google Tests source code folder (required only if you're going to build tests target)

Note for Windows FreeType library: if you're building it by yourself, make sure that output library's name is `freetype2.lib` and not `freetype26.lib` (that's what bundled FindFreeType.cmake looks for). It's also assumed for now that FreeType is compiled as a static library, otherwise you'll have to copy dll to resulting folder by yourself (or alter CMakeLists.txt a little)

###Known issues

* If you're facing linking problems in SDL2main library on VS 2015, you can recompile SDL2 by yourself using VS 2015, or just download SDL2 build bot package here: https://buildbot.libsdl.org/sdl-builds/sdl-visualstudio/

* Lantern fails loading resources when running from a directory different than the one where the executable is
