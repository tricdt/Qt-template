#pragma once

#include <QtGlobal>

#ifdef QTGEOMETRY_STATIC
#   define QTGEOMETRY_EXPORT
#else
#  if defined(QTGEOMETRY_LIBRARY)
#    define QTGEOMETRY_EXPORT Q_DECL_EXPORT
#  else
#    define QTGEOMETRY_EXPORT Q_DECL_IMPORT
#  endif
#endif