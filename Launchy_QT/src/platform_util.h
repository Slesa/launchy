#ifndef PLATFORM_UTIL
#define PLATFORM_UTIL

#include "platform_base.h"

#ifdef WIN32
#include "platform_win.h"
#endif

#ifdef OSX_BUILD
#include "platform_osx.h"
#endif

#ifdef KDE_BUILD
#include "platform_kde.h"
#endif

#endif