#include <psp2/kernel/processmgr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Vita newlib must be initialized before any C++ global constructor runs.
 * This follows newlib/libc/sys/vita/crt0.c from VitaSDK.  In particular,
 * malloc's lightweight mutex must exist before libstdc++ can allocate memory.
 */
#define ARGC_MAX 31

extern int main(int argc, char *argv[]);
extern void __libc_init_array(void);
extern void __libc_fini_array(void);

extern void _init_vita_heap(void);
extern void _init_vita_reent(void);
extern void _init_vita_malloc(void);
extern void _init_vita_io(void);

extern void _free_vita_io(void);
extern void _free_vita_malloc(void);
extern void _free_vita_reent(void);
extern void _free_vita_heap(void);

void _init_vita_newlib(void)
{
	_init_vita_heap();
	_init_vita_reent();
	_init_vita_malloc();
	_init_vita_io();
}

void _start(unsigned int args, void *argp)
{
	char *argv[ARGC_MAX + 1] = { "" };
	char *ptr = (char *)argp;
	int argc = 1;
	unsigned int loc = 0;

	while (loc < args && argc < ARGC_MAX) {
		argv[argc++] = &ptr[loc];
		loc += strnlen(&ptr[loc], args - loc) + 1;
	}
	argv[argc] = NULL;

	_init_vita_newlib();
	__libc_init_array();
	int r = main(argc, argv);
	__libc_fini_array();
	exit(r);
}

/* newlib libc.a _exit() calls this to release newlib state before exiting. */
void _free_vita_newlib(void)
{
	_free_vita_io();
	_free_vita_malloc();
	_free_vita_reent();
	_free_vita_heap();
}

/*
 * liblua's lauxlib.c calls popen(); newlib on Vita provides no popen() (there
 * is no fork/exec). Stub it so Lua scripts using io.popen do not break the
 * build; the function simply reports "not available".
 */
FILE *popen(const char *cmd, const char *mode)
{
	(void)cmd;
	(void)mode;
	return NULL;
}

int pclose(FILE *stream)
{
	(void)stream;
	return -1;
}
