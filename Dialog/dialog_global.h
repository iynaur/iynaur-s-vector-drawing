#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DIALOG_LIB)
#  define DIALOG_EXPORT Q_DECL_EXPORT
# else
#  define DIALOG_EXPORT Q_DECL_IMPORT
# endif
#else
# define DIALOG_EXPORT
#endif
