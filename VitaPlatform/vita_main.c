#include "vita_platform.h"
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#ifdef HAVE_OPENGL
#include <vitaGL.h>
#endif
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Vita entry point is provided by VitaPlatform/crt0.c (_start), which runs
 * libc global constructors then calls main(). main()'s VITA branch performs
 * all platform init (paths, SDL drivers, vita_platform_init).
 *
 * This file retains the legacy _vita_main helper for reference and any
 * callers expecting it, but no longer defines _start to avoid conflicting
 * with crt0.o.
 */
int _vita_main(int argc, char** argv)
{
	vita_platform_init();

	setenv("SDL_VIDEODRIVER", "vita", 1);
	setenv("SDL_AUDIODRIVER", "openal", 1);
	setenv("HOME", vita_get_data_path(), 1);
	setenv("USERPROFILE", vita_get_data_path(), 1);

	SDL_SetMainReady();

	int result = main(argc, argv);

	vita_platform_shutdown();
	sceKernelExitProcess(result);
	return result;
}
