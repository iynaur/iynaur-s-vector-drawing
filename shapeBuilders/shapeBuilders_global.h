#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SHAPEBUILDERS2_LIB)
#  define SHAPEBUILDERS_EXPORT Q_DECL_EXPORT
# else
#  define SHAPEBUILDERS_EXPORT Q_DECL_IMPORT
# endif
#else
# define SHAPEBUILDERS_EXPORT
#endif
