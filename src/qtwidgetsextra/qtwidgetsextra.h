#pragma once

#include <QtGlobal>

#ifdef QTWIDGETSEXTRA_STATIC
#   define QTWIDGETSEXTRA_EXPORT
#else
#  if defined(QTWIDGETSEXTRA_LIBRARY)
#    define QTWIDGETSEXTRA_EXPORT Q_DECL_EXPORT
#  else
#    define QTWIDGETSEXTRA_EXPORT Q_DECL_IMPORT
#  endif
#endif