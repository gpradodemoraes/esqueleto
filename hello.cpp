#define Uses_TKeys
#define Uses_TRect
#define Uses_TEvent
#define Uses_TMenuBar
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TSubMenu
#define Uses_TScreen

#include <tvision/tv.h>
#include "hello.h"
#include "TDemoWindow.h"
#include <random>

static std::random_device rd;						 // Non-deterministic generator
static std::mt19937 gen(rd());						 // Seed the Mersenne Twister engine
static std::uniform_int_distribution<> distX(1, 53); // Uniform distribution
static std::uniform_int_distribution<> distY(1, 16); // Uniform distribution

void THelloApp::greetingBox() {
	TDialog *d = new TDialog(TRect(25, 5, 55, 16), "Hello, World!");

	d->insert(new TStaticText(TRect(3, 5, 15, 6), "How are you?"));
	d->insert(new TButton(TRect(16, 2, 28, 4), "Terrific", cmCancel, bfNormal));
	d->insert(new TButton(TRect(16, 4, 28, 6), "Ok", cmCancel, bfNormal));
	d->insert(new TButton(TRect(16, 6, 28, 8), "Lousy", cmCancel, bfNormal));
	d->insert(new TButton(TRect(16, 8, 28, 10), "Cancel", cmCancel, bfNormal));

	deskTop->execView(d);
	destroy(d);
}

static constexpr int RANDOM_WINDOW_W = 10;
static constexpr int RANDOM_WINDOW_H = 5;

void THelloApp::myNewWindow() {
	static short winNumber = 0;
	TRect r(0, 0, RANDOM_WINDOW_W, RANDOM_WINDOW_H);
	TScreen::screenWidth;

	r.move(distX(gen), distY(gen));
	TDemoWindow *window = new TDemoWindow(r, "Demo Window", ++winNumber);
	deskTop->insert(window);
}

THelloApp::THelloApp() : TProgInit(&THelloApp::initStatusLine, &THelloApp::initMenuBar, &THelloApp::initDeskTop) {}

void THelloApp::handleEvent(TEvent &event) {
	TApplication::handleEvent(event);

	messageBox("Hello, World!", mfOKButton | mfInformation);
	if (event.what == evCommand) {
		switch (event.message.command) {
			case cmMyNewWin: myNewWindow(); break;
			case GreetThemCmd: greetingBox(); break;
			case hcResize: exit(1); break;
			default: break;
		}
		clearEvent(event);
	}
}

TMenuBar *THelloApp::initMenuBar(TRect r) {
	r.b.y = r.a.y + 1;

	return new TMenuBar(r, *new TSubMenu("~H~ello", kbAltH) + *new TMenuItem("~G~reeting...", GreetThemCmd, kbAltG) +
							   *new TSubMenu("~R~andom", kbAltR) +
							   *new TMenuItem("~N~ew Window...", cmMyNewWin, kbAltN) + newLine() +
							   *new TMenuItem("E~x~it", cmQuit, cmQuit, hcNoContext, "Alt-X"));
}

TStatusLine *THelloApp::initStatusLine(TRect r) {
	r.a.y = r.b.y - 1;
	return new TStatusLine(r, *new TStatusDef(0, 0xFFFF) + *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit) +
								  *new TStatusItem(0, kbF10, cmMenu));
}

// void THelloApp::changeBounds(const TRect &bounds) {
// 	TView::changeBounds(bounds);
// 	distX = std::uniform_int_distribution<int>(1, TScreen::screenWidth - RANDOM_WINDOW_W);
// 	distY = std::uniform_int_distribution<int>(1, TScreen::screenHeight - RANDOM_WINDOW_H - 2);
// 	drawView();
// }
