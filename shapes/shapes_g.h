#ifndef SHAPES_G_H
#define SHAPES_G_H



#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SHAPES_LIB)
#  define DLL Q_DECL_EXPORT
# else
#  define DLL Q_DECL_IMPORT
# endif
#else
# define DLL
#endif



#endif // SHAPES_G_H
