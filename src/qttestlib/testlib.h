#pragma once

#include <QtGlobal>

#ifdef QTTESTLIB_STATIC
#   define QTTESTLIB_EXPORT
#else
#  if defined(QTTESTLIB_LIBRARY)
#    define QTTESTLIB_EXPORT Q_DECL_EXPORT
#  else
#    define QTTESTLIB_EXPORT Q_DECL_IMPORT
#  endif
#endif