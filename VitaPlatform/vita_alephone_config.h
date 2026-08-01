#ifndef VITA_ALEPHONE_CONFIG_H
#define VITA_ALEPHONE_CONFIG_H

#define VITA
#define __VITA__
#define HAVE_SDL2
#define HAVE_SDL_TTF
/* #define HAVE_OPENGL -- port targets the software/SDL renderer (no GL runtime) */
#define HAVE_ZLIB
#define HAVE_SNDFILE
#define HAVE_OPENAL
#define HAVE_BOOST_FILESYSTEM
#define HAVE_BOOST_SYSTEM
#define HAVE_ASIO
#define HAVE_SDL_IMAGE
#define HAVE_PNG
#define A1_NETWORK_STANDALONE_HUB 0
#define DISABLE_NETWORKING 1
#define DISABLE_STEAM 1
#define SCENARIO_IS_BUNDLED 1

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#define SDL_MAIN_HANDLED

#define A1_HOMEPAGE_URL "https://alephone.lhowon.org/"

#define PATH_LIST_SEPARATOR ':'

#define MAX_PATH 256

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

#endif