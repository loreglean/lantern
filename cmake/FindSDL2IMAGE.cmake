find_path(SDL2IMAGE_INCLUDE_DIR
  NAMES
    SDL_image.h
  HINTS
    ENV SDL2IMAGEDIR
  PATH_SUFFIXES
    include/
    include/SDL2Image)

find_library(SDL2IMAGE_LIBRARY
  NAMES
    SDL2_image
  HINTS
    ENV SDL2IMAGEDIR
  PATH_SUFFIXES
    lib
    lib/x86)

find_file(SDL2IMAGE_DLL
  NAMES
    SDL2_image.dll
  HINTS
    ENV SDL2IMAGEDIR
  PATH_SUFFIXES
    bin
    lib/x86)

# We need libpng and zlib for loading .png files on windows

find_file(SDL2IMAGE_LIBPNG_DLL
  NAMES
    libpng16-16.dll
  HINTS
    ENV SDL2IMAGEDIR
  PATH_SUFFIXES
    bin
    lib/x86)

find_file(SDL2IMAGE_ZLIB_DLL
  NAMES
    zlib1.dll
  HINTS
    ENV SDL2IMAGEDIR
  PATH_SUFFIXES
    bin
    lib/x86)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2IMAGE
                                  REQUIRED_VARS SDL2IMAGE_LIBRARY SDL2IMAGE_INCLUDE_DIR)