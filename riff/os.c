#include "os.h"

/* run another program/shell command */
int os_syscall(const char* args);

/* the current directory as a string */
char* os_getcwd(void);

/* GID function */

/* UID function */

/* should return this information: file/dir name, file/dir name length, file/directory?, read/write access?*/
/* will require some struct */ void os_listdir(); /* change ret type */

int os_makedir(const char* dir);

int os_changedir(const char* to);

/* can also move files */
int os_rename(char* src, char* dest);

int os_rmdir(char* dir);