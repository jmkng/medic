#if defined(__APPLE__)
#include "fs_darwin.c"
#else
#error "unsupported platform for fs module"
#endif
