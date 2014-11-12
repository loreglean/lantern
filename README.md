lantern
=======

Software renderer written for educational purposes. Uses SDL2 for rendering and Google C++ Testing Framework for tests.

###Building on Linux
* Install SDL2 (https://www.libsdl.org/download-2.0.php, or just ```sudo apt-get install libsdl2-dev``` for apt). If you want to build tests target, you must also install Google Testing Framework (https://code.google.com/p/googletest/downloads/list, or just ```sudo apg-get install libgtest-dev``` for apt)
* Run terminal. Move to lantern source code folder
* Run: ```mkdir build && cd build``` to create and move to the build folder
* Run: ```cmake -G "Unix Makefiles" ..``` to generate makefile
* Run: ```make lantern -j$(nproc) -s``` to build library
* Run: ```make rotating_car_app -j$(nproc) -s``` to build example application
* Run: ```make tests -j$(nproc) -s``` to build tests executable

###Building on Windows (using MinGW)
* Install SDL2 (https://www.libsdl.org/download-2.0.php,
* select Visual C++ 32/64-bit of development library, unpack to root of source dir, at include folder create SDL2 and move all containce files to it).
* If you want to build tests target, you must also install Google Testing Framework (https://code.google.com/p/googletest/downloads/list just unpack to source dir, current 1.7.0 using now) and pthreads library (you can use MinGW Installation Manager for this one)
* Make sure your PATH environment variable contains MinGW's bin folder - CMake needs it
* Run cmd.exe
* Move to your build folder
* Run: ```<path_to_cmake_bin_folder>/cmake.exe -G "MinGW Makefiles" <path_to_lantern_source_folder>``` to generate makefile
* Run: ```mingw32-make lantern -j%NUMBER_OF_PROCESSORS% -s``` to build library
* Run: ```mingw32-make rotating_car_app -j%NUMBER_OF_PROCESSORS% -s``` to build example application
* Run: ```mingw32-make tests -j%NUMBER_OF_PROCESSORS% -s``` to build tests executable

###Building on Windows (using Visual Studio)
* Install SDL2 and gtest the same way like in MinGW case
* Run cmd like in MinGW case
* Run: ```cmake -G "Visual Studio 12" ..``` to generate solution and projects files (in this case we use VS Express 2013 for Desktop or more common Visual Studio 2013)
* Run: ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" .\lantern.sln /m /t:lantern``` to build library
* Run: ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" .\lantern.sln /m /t:rotating_car_app``` to build example application
* Run: ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" .\lantern.sln /m /t:tests``` to build tests executable


You can also use CLion EAP (http://confluence.jetbrains.com/display/CLION/Early+Access+Program) for both platforms.
