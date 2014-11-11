lantern
=======

Software renderer written for educational purposes. Uses SDL2 for rendering and Google C++ Testing Framework for tests.

###Building on Linux
* Install SDL2 (https://www.libsdl.org/download-2.0.php, or just ```sudo apt-get install libsdl2-dev``` for apt). If you want to build tests target, you must also install Google Testing Framework (https://code.google.com/p/googletest/downloads/list)
* Run terminal. Move to lantern source code folder
* Run: ```mkdir build && cd build``` to create and move to the build folder
* Run: ```cmake -G "Unix Makefiles" ..``` to generate makefile
* Run: ```make lantern``` to build library
* Run: ```make rotating_car_app``` to build example application
* Run: ```make tests``` to build tests executable

###Building on Windows (using MinGW)
* Install SDL2 (https://www.libsdl.org/download-2.0.php, select MinGW version of development library). If you want to build tests target, you must also install Google Testing Framework (https://code.google.com/p/googletest/downloads/list) and pthreads library (you can use MinGW Installation Manager for this one)
* Make sure your PATH environment variable contains MinGW's bin folder - CMake needs it
* Run cmd.exe
* Move to your build folder
* Run: ```<path_to_cmake_bin_folder>/cmake.exe -G "MinGW Makefiles" <path_to_lantern_source_folder>``` to generate makefile
* Run: ```mingw32-make lantern``` to build library
* Run: ```mingw32-make rotating_car_app``` to build example application
* Run: ```mingw32-make tests``` to build tests executable

You can also use CLion EAP (http://confluence.jetbrains.com/display/CLION/Early+Access+Program) for both platforms.
