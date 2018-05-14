#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ICALLBACK_LIB)
#  define ICALLBACK_EXPORT Q_DECL_EXPORT
# else
#  define ICALLBACK_EXPORT Q_DECL_IMPORT
# endif
#else
# define ICALLBACK_EXPORT
#endif
