#ifndef QTOPENAI_GLOBAL_H
#define QTOPENAI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTOPENAI_LIBRARY)
#  define QTOPENAI_EXPORT Q_DECL_EXPORT
#else
#  define QTOPENAI_EXPORT Q_DECL_IMPORT
#endif

#endif // QTOPENAI_GLOBAL_H
