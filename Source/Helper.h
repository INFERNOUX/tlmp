#pragma once

#include <stdarg.h>
#include <string>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Timer.h"
#include "Containers.h"

#define EXEBASEREAL   0x400000
#define EXEBASE       0x400000
#define EXEOFFSET(o1) (((o1)-EXEBASE)+EXEBASEREAL)

#define TLFUNC(v1, t1, c1, a1)          static t1(c1 *v1)a1;
#define TLFUNCPTR(v1, t1, c1, a1, o1)   t1(c1 *v1)a1 = ((t1(c1 *)a1)o1);

void xcept(const char *fmt, ...);

static HANDLE h_log_file = INVALID_HANDLE_VALUE;
void log(const char *fmt, ...);

#define NL_ITERATE(var,type,list) for (slist_table<type>::iterator var(list);var.i!=list.size();++var)
string format(const char *format, ...);
string vformat(const char*format, va_list args);
