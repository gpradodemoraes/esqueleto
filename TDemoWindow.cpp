#define Uses_TWindow
#include <tvision/tv.h>
#include "TDemoWindow.h"

TDemoWindow::TDemoWindow(const TRect r, const char *aTitle, short aNumber)
	: TWindow(r, aTitle, aNumber), TWindowInit(&TDemoWindow::initFrame) {}
