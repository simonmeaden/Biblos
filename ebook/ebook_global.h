#ifndef EBOOK_GLOBAL_H
#define EBOOK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EBOOK_LIBRARY)
#  define EBOOKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EBOOKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EBOOK_GLOBAL_H
