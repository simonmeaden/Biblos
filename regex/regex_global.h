#ifndef REGEX_GLOBAL_H
#define REGEX_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(REGEX_LIBRARY)
#  define REGEXSHARED_EXPORT Q_DECL_EXPORT
#else
#  define REGEXSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // REGEX_GLOBAL_H
