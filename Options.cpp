#include "Options.h"

Options::Options(void)
{
}

Options::~Options(void)
{
}


void Options::GameInit(Display *display)
{
	redefining = 0;
	i_background = LoadImage(display, IDB_OPTIONS);
//	b_exitdown.Init(display, IDB_EXITDOWN, 10,292,51,17);
	nextState = OPTIONS;
//	i_levelselect = LoadImage(display, IDB_LEVELSELECT);
//	i_choice = LoadImage(display, IDB_CHOICE);
//	i_soundoffselect = LoadImage(display, IDB_SOUNDOFFSELECT);
}



void Options::GameEnd()
{

}



void Options::GameLoop(Display *display)
{
	if (nextState != OPTIONS) {
		shared->gamestate = nextState;
		nextState= OPTIONS;
	}

	Render(display);
}

void Options::Render(Display *display) {
	display->Blit(0,0,i_background);
	b_exitdown.Render(display);
	display->SetBlending(128);
	

	display->SetBlending(255);
}

void Options::StylusDown( Point stylus )
{	
	
}

void Options::StylusUp( Point stylus )
{
	if (b_exitdown.StylusUp(stylus) && redefining == 0) {
		nextState = MAINMENU;
		shared->PlaySoundA(0);
	}
}



void Options::StylusMove( Point stylus )
{
	b_exitdown.StylusMove(stylus);

}
void Options::ButtonUp(int button) {

}

void Options::ButtonDown(int button) {
}
void Options::FinishAnimation() {
}

void Options::StartAnimation() {
}