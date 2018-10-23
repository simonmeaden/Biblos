#ifndef LIBMOBI_GLOBAL_H
#define LIBMOBI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBMOBI_LIBRARY)
#  define LIBMOBISHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBMOBISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBMOBI_GLOBAL_H
