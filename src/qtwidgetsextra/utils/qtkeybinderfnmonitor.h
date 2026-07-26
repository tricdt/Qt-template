#pragma once
#include <functional>
#include <qglobal.h>

#ifdef Q_OS_MACOS
void qtStartFnKeyMonitor(std::function<void()> onFnKeyPressed);
void qtStopFnKeyMonitor();
#else
inline void qtStartFnKeyMonitor(std::function<void()>) {}
inline void qtStopFnKeyMonitor() {}
#endif

