#include "LFrameworkConfig.h"

#include "Detect/DetectOS.h"
#include "Assert.h"

#if defined(LF_NEWLIB_TO_RTOS_MALLOC)
#include "FreeRTOS.h"
#endif

#if defined(LF_SYSCALLS_STUBS) || defined(LF_NEWLIB_TO_RTOS_MALLOC)
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../Debug.h"
#include "System.h"
#include <malloc.h>
#endif//Todo check right files for different configuration

extern "C" {

#if defined(LF_SYSCALLS_STUBS)

//extern int errno;
//register char * stack_ptr asm("sp");

#ifndef __errno_r
#include <sys/reent.h>
#define __errno_r(reent) reent->_errno
#endif

caddr_t _sbrk(int incr) {
	static_cast<void>(incr);
	//std::terminate(); //Do not use newlib memory management
	for(;;);
	return nullptr;
}

int _write_r(struct _reent* r, int file, char* ptr, int len){
	static_cast<void>(r);
	static_cast<void>(ptr);

	if(stdout->_file != file && stderr->_file != file){
		for(;;);//Assert: unknown file descriptor
	}

#if defined(AF_TERMINAL_STDIO)
  Terminal::write(ptr, len);
#endif

  return len;
}

int _read_r(struct _reent *r, int file, char * ptr, int len)
{
  (void)r;
  (void)file;
  (void)ptr;
  (void)len;
  __errno_r(r) = EINVAL;
  return -1;
}

int _lseek_r(struct _reent *r, int file, int ptr, int dir){
  (void)r;
  (void)file;
  (void)ptr;
  (void)dir;
  return 0;
}

int _close_r(struct _reent *r, int file){
    (void)r;
    (void)file;
    return 0;
}

int _open_r ( struct _reent *ptr, const char *file, int flags, int mode ){
	static_cast<void>(ptr);
	static_cast<void>(file);
	static_cast<void>(flags);
	static_cast<void>(mode);
	int fd = -1;
	return fd;
}

int _fstat_r(struct _reent *r, int file, struct stat * st){
    (void)r;
    (void)file;
    memset(st, 0, sizeof(*st));
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty_r(struct _reent *r, int fd){
    (void)r;
    (void)fd;
    return 1;
}

int _kill (int a, int b){
	(void)a;
	(void)b;
	return 0;
}
int _getpid(int a){
	(void)a;
	return 0;
}

#endif
//memory management

#if LF_TARGET_OS == LF_OS_FREERTOS ||  defined(LF_SYSCALLS_STUBS)

void __assert_func(const char * file, int line, const char * func, const char * error){
	lfDebug() << "libc assert";
	lfDebug() << "File: " << file;
	//lfDebug() << "Line: " << line;
	lfDebug() << "Function: " << func;
	lfDebug() << "Expression: " << error;
	for(;;);
}
void _exit(int status){
	lfDebug() << "_exit called";
	for(;;);
}

#endif


#if defined(LF_NEWLIB_TO_RTOS_MALLOC)


void* _realloc_r(struct _reent *re, void* oldAddr, size_t newSize) {
	static_cast<void>(re);
	static_cast<void>(oldAddr);
	static_cast<void>(newSize);
	lfAssert(false); //No mechanism is currently available to correctly reallocate memory, so just don't allow it for now
	return nullptr;
}

void* _calloc_r(struct _reent *re, size_t num, size_t size) {
	static_cast<void>(re);
	void* result = pvPortMalloc(num*size);
	if(result){
		//calloc should zero-initialize allocated memory
		uint8_t* bytes = (uint8_t*)result;
		for(size_t i = 0; i < (num * size); ++i){
			bytes[i] = 0;
		}
	}
	return result;
}

void* _malloc_r(struct _reent *re, size_t size) {
	static_cast<void>(re);
	return pvPortMalloc(size);
}

void _free_r(struct _reent *re, void* ptr) {
	static_cast<void>(re);
	vPortFree(ptr);
}
#endif

} //extern "C"
