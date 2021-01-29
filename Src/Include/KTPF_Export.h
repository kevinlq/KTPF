#pragma once

#include <qglobal.h>

#if defined(KTPF_LIBRARY)
#  define KTPF_EXPORT Q_DECL_EXPORT
#else
#  define KTPF_EXPORT Q_DECL_IMPORT
#endif

