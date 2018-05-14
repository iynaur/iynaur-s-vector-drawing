#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SHAPEEDITORS_LIB)
#  define SHAPEEDITORS_EXPORT Q_DECL_EXPORT
# else
#  define SHAPEEDITORS_EXPORT Q_DECL_IMPORT
# endif
#else
# define SHAPEEDITORS_EXPORT
#endif
