#include "os.h"

#include <direct.h>
#include <sys\stat.h>
#include <sys\types.h>

int os_syscall(const char* args) {
	return (args && (system(args) >= 0));
}

char* os_getcwd(void);

/* GID function */

/* UID function */

/* should return this information: file/dir name, file/dir name length, file/directory?, read/write access?*/
/* will require some struct */ void os_listdir(); /* change ret type */

int os_makedir(const char* dir) {
#if defined(OS_WINDOWS)
	return _mkdir(dir);
#else
	return mkdir(dir);
#endif
}

int os_changedir(const char* to) {
#if defined(OS_WINDOWS)
	return _chdir(to);
#else
	return chdir(to);
#endif
}

int os_rename(char* src, char* dest) {
	return rename(src, dest);
}

int os_rmdir(char* dir) {
#if defined(OS_WINDOWS)
	return _rmdir(dir);
#else
	return rmdir(dir);
#endif
}