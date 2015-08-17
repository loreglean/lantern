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

Simple scripts were created to simplify building process, though (output goes to `<lantern>/build` folder):
* `build_vs14.bat` - creates VS solution for Visual Studio 2015
* `build_vs12.bat` - creates VS solution for Visual Studio 2013
* `build_mingw_make.bat` - creates MinGW makefiles
* `build_make.sh` - creates Linux makefiles
* `build_xcode.sh` - creates XCode project

On Windows you have to specify a few environment variables for dependencies:
 * `SDL2DIR` - path to SDL2 development library
 * `SDL2IMAGEDIR` - path to SDL2_Image development library
 * `FREETYPEDIR` - path to a folder containing FreeType's headers and library
 * `GTEST_ROOT` - path to Google Tests source code folder (required only if you're going to build tests target)

###Known issues

* If you're facing linking problems in SDL2main library on VS 2015, you can recompile SDL2 by yourself using VS 2015, or just download SDL2 build bot package here: https://buildbot.libsdl.org/sdl-builds/sdl-visualstudio/

* Lantern fails loading resources when running from a directory different than the one where the executable is
