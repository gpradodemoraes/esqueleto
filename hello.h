#ifndef _HELLO_H
#define _HELLO_H
#define Uses_TMenu
#define Uses_TMenuItem
#define Uses_TDialog
#define Uses_TButton
#define Uses_TStaticText
#define Uses_TDeskTop
#define Uses_TApplication
#define Uses_MsgBox
#include <tvision/tv.h>
#include <tvision/app.h>
#include <tvision/views.h>
#include <tvision/menus.h>

const int GreetThemCmd = 100;
const int cmMyNewWin = 101;

class THelloApp : public TApplication {
   public:
	THelloApp();
	virtual void handleEvent(TEvent &event);
	static TMenuBar *initMenuBar(TRect);
	static TStatusLine *initStatusLine(TRect);
	// void changeBounds(const TRect &bounds);
   private:
	void myNewWindow();
	void greetingBox();
};

#endif
