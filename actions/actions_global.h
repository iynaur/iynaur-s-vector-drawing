#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ACTIONS_LIB)
#  define ACTIONS_EXPORT Q_DECL_EXPORT
# else
#  define ACTIONS_EXPORT Q_DECL_IMPORT
# endif
#else
# define ACTIONS_EXPORT
#endif
