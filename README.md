lantern
=======

Software renderer written for educational purposes. Uses SDL2 for rendering and Google C++ Testing Framework for tests.

###Building on Windows (MinGW)
* Download SDL ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php)), select MinGW version of development library). Download SDL2Image ([https://www.libsdl.org/projects/SDL_image/](https://www.libsdl.org/projects/SDL_image/)). If you want to build tests you must also download Google C++ Testing Framework source code ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open cmd.exe, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```set SDL2DIR=<path to SDL2>```. Note that it should be either i686-w64-mingw32 (for 32-bit) or x86_64-w64-mingw32 (for 64-bit) folder
* Run: ```set SDL2IMAGEDIR=<path to SDL2>```
* Run: ```set GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G "MinGW Makefiles" ..``` to generate makefile
* Run: ```mingw32-make lantern``` to build library
* Run: ```mingw32-make rotating_car_app``` to build example application
* Run: ```mingw32-make tests``` to build tests executable

###Building on Windows (Visual Studio 2013 / 2015)
* Download SDL2 ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php), select VC++ version of development library). Download SDL2Image ([https://www.libsdl.org/projects/SDL_image/](https://www.libsdl.org/projects/SDL_image/)). If you want to build tests you must also download Google C++ Testing Framework source code ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open cmd.exe, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```set SDL2DIR=<path to SDL2>```
* Run: ```set SDL2IMAGEDIR=<path to SDL2>```
* Run: ```set GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G "Visual Studio 12" ..``` for VS 2013 (or ```cmake -G "Visual Studio 14" ..``` for VS 2015) to generate VS solution
* Visual Studio solution is ready now
* If you're facing linking problems in SDL2main library on VS 2015, you can recompile SDL2 yourself using VS 2015 or just download SDL2 build bot package here: [https://buildbot.libsdl.org/sdl-builds/sdl-visualstudio/](https://buildbot.libsdl.org/sdl-builds/sdl-visualstudio/)

###Building on Linux
* Install SDL2 ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php), or just ```sudo apt-get install libsdl2-dev``` for apt). Install SDL2_Image  ([https://www.libsdl.org/projects/SDL_image/](https://www.libsdl.org/projects/SDL_image/), or just ```sudo apt-get install libsdl2-image-dev``` for apt). If you want to build tests you must also download Google C++ Testing Framework ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list) or just ```sudo apt-get install libgtest-dev``` for apt)
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```export GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target. If you installed it via apt the path is ```/usr/src/gtest```
* Run: ```cmake -G "Unix Makefiles" ..``` to generate makefile
* Run: ```make lantern``` to build library
* Run: ```make rotating_car_app``` to build example application
* Run: ```make tests``` to build tests executable

###Building on Mac OS X
* Install SDL2 ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php)) to ```/Library/Frameworks/``` or ```~/Library/Frameworks/```. Install SDL2Image ([https://www.libsdl.org/projects/SDL_image/](https://www.libsdl.org/projects/SDL_image/)). If you want to build tests you must also download Google C++ Testing Framework ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```export GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G Xcode ..``` to generate project
* XCode project is ready now

###Known issues
* Lantern might fail loading resources when running from directory different then the one where the executable is
