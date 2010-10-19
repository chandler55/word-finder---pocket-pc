#include "MainMenu.h"


//#include <assert.h>
#include <ctype.h>
#include <limits.h>
//#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "termios.h"
//#include <time.h>
#include "unistd.h"

MainMenu::MainMenu(void)
{
}

MainMenu::~MainMenu(void)
{
}

void MainMenu::GameInit(Display *display)
{
	boardy = 0;
	animate = 0;
	whichtile= 1;
//	i_tiles = LoadImage(display, IDB_TILES);
	i_atime[0] = LoadImage(display, IDB_ATIME1);
	i_atime[1] = LoadImage(display, IDB_ATIME2);
	i_atime[2] = LoadImage(display, IDB_ATIME3);
	i_rgbtime[0] = LoadImage(display, IDB_RGBTIME1);
	i_rgbtime[1] = LoadImage(display, IDB_RGBTIME2);
	i_rgbtime[2] = LoadImage(display, IDB_RGBTIME3);
	i_background = LoadImage(display, IDB_JOGGLETITLESCREEN);
	nextState = MAINMENU;
	b_newgame = new Button();
	b_newgame->Init(ANIMATION_MOVELEFT, display, IDB_NEWGAME3, IDB_NEWGAMEDOWN, 62,185,117,19);
	b_highscores.Init(ANIMATION_MOVERIGHT, display, IDB_HIGHSCORESBUTTON, IDB_HIGHSCORESDOWN, 62,210, 117,19);
	b_tutorial.Init(ANIMATION_MOVELEFT, display, IDB_RULES, IDB_RULESDOWN, 62,210+25,117,19);
	b_exit.Init(ANIMATION_MOVERIGHT, display, IDB_EXIT, IDB_EXITDOWN2, 62,210+50,117,19);
}

void MainMenu::GameEnd()
{
	
}
void MainMenu::StylusDown(Frog::Point stylus) {
	b_newgame->StylusDown(shared->input->getCoordinates());
	b_highscores.StylusDown(shared->input->getCoordinates());
	b_tutorial.StylusDown(shared->input->getCoordinates());
	b_exit.StylusDown(shared->input->getCoordinates());
}
void MainMenu::StylusMove(Frog::Point stylus) {
	b_newgame->StylusMove(shared->input->getCoordinates());
	b_highscores.StylusMove(shared->input->getCoordinates());
	b_tutorial.StylusMove(shared->input->getCoordinates());
	b_exit.StylusMove(shared->input->getCoordinates());
}
void MainMenu::Animate() {
bool finishedanimating = false;
	if ((boardy >= 0 && animating == 2) || (boardy <= -320 && animating == 1))
		finishedanimating= true;

		if (animate > 0 || !finishedanimating) {
			animate -= shared->delta_time;
			if (animating == 2) {
				boardy += shared->delta_time * 500;
				if (boardy > 0)
					boardy = 0;
			}
			else if (animating == 1)
			{
				boardy -= shared->delta_time * 500;
				if (boardy < -320)
					boardy = -320;
			}
		}
		else {	
			animate = 0;	
		}
}
void MainMenu::GameLoop(Display *display)
{
	// Gameplay
	if (animate) {
		Animate();
	} else {
		if (nextState != MAINMENU) {
			shared->gamestate = nextState;
			nextState= MAINMENU;
		}
	}

	Input();
	// Render
	Render(display);

}
void MainMenu::Input() {

		if (shared->input->StylusUp()) {
		if (Frog::Rect(30,144,30+56,144+34).Contains(shared->input->getCoordinates())) {
			shared->PlaySoundA(0);
					shared->whatLength = 0;
		}
		else if (Frog::Rect(90,144,90+56,144+34).Contains(shared->input->getCoordinates())) {
			shared->PlaySoundA(0);
			shared->whatLength = 1;
		}
		else if (Frog::Rect(159,144,159+56,144+34).Contains(shared->input->getCoordinates())) {
			shared->whatLength = 2;
			shared->PlaySoundA(0);
		}
		if (b_newgame->StylusUp(shared->input->getCoordinates())) {
			shared->PlaySoundA(1);
			nextState = JOGGLE;
			animate = 0.5;
			animating= 1;
		}
		if (b_highscores.StylusUp(shared->input->getCoordinates())) {
			shared->PlaySoundA(1);
			animate = 0.5;
			animating= 1;
			nextState = HIGHSCORES;
		}
		if (b_tutorial.StylusUp(shared->input->getCoordinates())) {
			shared->PlaySoundA(1);
			animate = 0.5;
			animating= 1;
			nextState = TUTORIAL;
		}
		if (b_exit.StylusUp(shared->input->getCoordinates())) {
			shared->PlaySoundA(1);
			nextState = EXIT;
		}
	}
}
void MainMenu::Render(Display *display) {
	int i;
	TCHAR buffer[100];
	display->Blit(0,0,i_background);

	b_newgame->Render(display,boardy);
	b_highscores.Render(display,boardy);
	b_tutorial.Render(display,boardy);
	b_exit.Render(display,boardy);
	
	if (shared->trial) {
	 
		_stprintf(buffer, TEXT("%d MINUTES LEFT"),int( 20 - (shared->trialtime / 60)));
		shared->bigfont->DisplayText(display,130-(lstrlen(buffer)*5),10,buffer,Color(0,0,0));

		_stprintf(buffer, TEXT("TRIAL VERSION"));
		shared->bigfont->DisplayText(display,130-(lstrlen(buffer)*5),135,buffer,Color(0,0,0));

		_stprintf(buffer, TEXT("WWW.HANDANGO.COM"));
		shared->bigfont->DisplayText(display,120-(lstrlen(buffer)*5),300,buffer,Color(0,0,0));
	}
	display->Blit(30,144,i_rgbtime[shared->whatLength],i_atime[shared->whatLength]);
		
}

void MainMenu::FinishAnimation() {
	boardy = -320;
	animating= 0;
}

void MainMenu::StartAnimation() {
	animating= 2;
	animate = 0.5;
	boardy = 0.5 * -320;
}