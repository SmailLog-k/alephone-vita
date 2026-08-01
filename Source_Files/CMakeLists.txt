cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "VitaSDK toolchain")
  else()
    message(FATAL_ERROR "Set VITASDK environment variable before configuring")
  endif()
endif()

project(AlephOne_Vita LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Vita-specific definitions
add_compile_definitions(
  VITA
  __VITA__
  HAVE_SDL2
  HAVE_SDL_TTF
  HAVE_OPENGL
  HAVE_ZLIB
  HAVE_SNDFILE
  HAVE_OPENAL
  HAVE_BOOST_FILESYSTEM
  HAVE_BOOST_SYSTEM
  HAVE_ASIO
  HAVE_SDL_IMAGE
  HAVE_PNG
  A1_NETWORK_STANDALONE_HUB=0
  DISABLE_NETWORKING=1
  DISABLE_STEAM=1
  SCENARIO_IS_BUNDLED=1
  _GNU_SOURCE
  _POSIX_C_SOURCE=200809L
  SDL_MAIN_HANDLED
)

# Don't include CMake's compiler detection files in the build
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Disable compiler detection
set(CMAKE_C_COMPILER_ID_RUN 1)
set(CMAKE_CXX_COMPILER_ID_RUN 1)

# Disable CMake's built-in compiler detection entirely
set(CMAKE_DISABLE_FIND_PACKAGE_CompilerIdC TRUE)
set(CMAKE_DISABLE_FIND_PACKAGE_CompilerIdCXX TRUE)

# Don't check for working compiler
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,-z,max-page-size=0x1000")

# VitaSDK paths
set(VITASDK_ROOT "$ENV{VITASDK}/arm-vita-eabi")

# Include directories
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/CSeries
  ${CMAKE_CURRENT_SOURCE_DIR}/Files
  ${CMAKE_CURRENT_SOURCE_DIR}/GameWorld
  ${CMAKE_CURRENT_SOURCE_DIR}/Input
  ${CMAKE_CURRENT_SOURCE_DIR}/Lua
  ${CMAKE_CURRENT_SOURCE_DIR}/Misc
  ${CMAKE_CURRENT_SOURCE_DIR}/ModelView
  ${CMAKE_CURRENT_SOURCE_DIR}/RenderMain
  ${CMAKE_CURRENT_SOURCE_DIR}/RenderOther
  ${CMAKE_CURRENT_SOURCE_DIR}/Sound
  ${CMAKE_CURRENT_SOURCE_DIR}/Videos
  ${CMAKE_CURRENT_SOURCE_DIR}/XML
  ${CMAKE_CURRENT_SOURCE_DIR}/../VitaPlatform
  ${VITASDK_ROOT}/include
  ${VITASDK_ROOT}/include/SDL2
  ${VITASDK_ROOT}/include/freetype2
)

# Find all source files
file(GLOB_RECURSE ALEPHONE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/CSeries/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Files/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/GameWorld/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Input/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Lua/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Misc/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ModelView/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Network/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RenderMain/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RenderOther/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Sound/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TCPMess/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Videos/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/XML/*.cpp
)

# Remove platform-specific files that won't compile on Vita
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*shell_misc_win32\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*shell_misc_osx\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*videos_ffmpeg\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*steam.*\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*SDL_main\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*SDL_rwops_zzip\\.c")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Network.*")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*TCPMess.*")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*metaserver.*")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*sdl_widgets\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*csdialogs_sdl\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*csalerts_sdl\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Statistics\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*sdl_dialogs\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*preferences\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*ScenarioChooser\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*preference_dialogs\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*game_wad\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*FileHandler\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*items\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*player\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*lua_hud_objects\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*lua_hud_script\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*lua_script\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Console\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*shared_widgets\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*lua_map\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*preferences_widgets_sdl\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*lua_objects\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*lua_ephemera\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*thread_priority_sdl_win32\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*DefaultStringSets\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*interface\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*marathon2\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OGL_Shader\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*RenderRasterize_Shader\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OGL_Render\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Rasterizer_Shader\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OGL_Textures\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OGL_Setup\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OGL_LoadScreen\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*screen\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*motion_sensor\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*overhead_map\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OGL_FBO\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*QuickSave\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*sdl_widgets\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*AudioPlayer\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OpenALManager\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*SoundPlayer\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*OGL_Faders\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*ChaseCam\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*SoundManager\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*SoundFile\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Decoder\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*MusicPlayer\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Music\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*ReplacementSounds\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*shell\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*shell_misc\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*shell_options\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*sdl_dialogs\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*sdl_fonts\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*preferences\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*ScenarioChooser\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*preference_dialogs\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*preferences_widgets_sdl\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*interface\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Statistics\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Logging\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*Console\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*shared_widgets\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*DefaultStringSets\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*thread_priority_sdl_win32\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*thread_priority_sdl_posix\\.cpp")

# Add Vita platform files
file(GLOB VITA_PLATFORM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../VitaPlatform/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../VitaPlatform/*.c)

# Create executable
add_executable(alephone_vita ${ALEPHONE_SOURCES} ${VITA_PLATFORM_SOURCES})

# Link libraries
target_link_libraries(alephone_vita
  # Vita system libraries (only those that exist)
  SceLibKernel_stub
  SceLibc_stub
  SceDisplay_stub
  SceCtrl_stub
  SceTouch_stub
  SceAudio_stub
  SceKernelThreadMgr_stub
  SceSysmem_stub
  SceIofilemgr_stub
  SceNet_stub
  SceNetCtl_stub
  ScePower_stub
  SceRtc_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceAppUtil_stub
  ScePgf_stub
  SceJpeg_stub
  # Third-party libraries
  SDL2
  SDL2main
  SDL2_ttf
  SDL2_image
  SDL2_mixer
  SDL2_net
  png
  freetype
  boost_filesystem
  boost_system
  asio
  openal
  sndfile
  vitaGL
  vita2d
  vita2d_ext
  vita2d_vgl
  jpeg
  webp
  curl
  vitashark
  psp2
)

# Vita-specific target properties
set_target_properties(alephone_vita PROPERTIES
  LINK_FLAGS "-Wl,-q -Wl,--gc-sections -Wl,-z,max-page-size=0x1000"
  OUTPUT_NAME "alephone_vita"
)

# Exclude compiler detection files from the build
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*CMakeCCompilerId\\.c")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*CMakeCXXCompilerId\\.cpp")
list(FILTER ALEPHONE_SOURCES EXCLUDE REGEX ".*thread_priority_sdl_macosx\\.cpp")

# Create VPK packaging target
find_program(VITA_CREATE_VPK vita_create_vpk)
if(VITA_CREATE_VPK)
  add_custom_command(TARGET alephone_vita POST_BUILD
    COMMAND ${VITA_CREATE_VPK} alephone_vita.vpk
    -n "Aleph One (Marathon Trilogy)"
    -t ALEPH0001
    -i ${CMAKE_CURRENT_SOURCE_DIR}/../data/icons/128x128/alephone.png
    -b ${CMAKE_CURRENT_SOURCE_DIR}/../data/icons/256x256/alephone.png
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Creating VPK package"
  )
endif()
