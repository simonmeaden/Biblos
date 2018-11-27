#ifndef LIBEPUB_GLOBAL_H
#define LIBEPUB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBEPUB_LIBRARY)
#  define LIBEPUBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBEPUBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBEPUB_GLOBAL_H
