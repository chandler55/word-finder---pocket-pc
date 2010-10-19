#include "Tutorial.h"
#include <math.h>
Tutorial::Tutorial(void)
{
}

Tutorial::~Tutorial(void)
{
}

void Tutorial::GameInit(Display *display)
{
	page = PAGE1;
	boardy = -500;
	i_background = LoadImage(display, IDB_RULESBACKGROUND);
	i_background2 = LoadImage(display, IDB_RULESBACKGROUND2);
	i_background3 = LoadImage(display, IDB_RULESBACKGROUND3);
	i_background4 = LoadImage(display, IDB_RULESBACKGROUND4);
	nextState = TUTORIAL;
	b_menu = new Button();
	b_menu->Init(ANIMATION_MOVELEFT, display, IDB_JOGGLEMENUUP, IDB_JOGGLEMENU,0,0,54,19);
	b_next = new Button();
	b_next->Init(ANIMATION_MOVERIGHT, display, IDB_NEXT, IDB_NEXTDOWN,240-45,320-21,45,21);
}

void Tutorial::GameEnd()
{
	
}


void Tutorial::Animate() {
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
void Tutorial::GameLoop(Display *display)
{
	if (animate) {
		Animate();
	} else {
		if (nextState != TUTORIAL) {
		shared->gamestate = nextState;
		nextState= TUTORIAL;
		}
	}

	Input();
	Render(display);
}

void Tutorial::Render(Display *display) {
	display->Clear(Color(0,0,0));
	switch (page) {
		case PAGE1:
			display->Blit(0,0,i_background);
			break;
		case PAGE2:
			display->Blit(0,0,i_background2);
			break;
		case PAGE3:
			display->Blit(0,0,i_background3);
			break;
		case PAGE4:
			display->Blit(0,0,i_background4);
			break;
	}

	b_menu->Render(display,boardy);
	b_next->Render(display, boardy);
	TCHAR pagenumber[20];
	_stprintf(pagenumber, TEXT("PAGE: %d / 4"), page+1);
	shared->font->DisplayText(display, 178,5,pagenumber, Color(0,0,0));
}
void Tutorial::Input() {

	if (shared->input->StylusDown()) {
		b_menu->StylusDown(shared->input->getCoordinates());
		b_next->StylusDown(shared->input->getCoordinates());
	}
	if (shared->input->StylusUp()) {
		if (b_menu->StylusUp(shared->input->getCoordinates())) {
			nextState = MAINMENU;
			animate = 0.5;
			animating= 1;
			shared->PlaySoundA(1);
		}
		if (b_next->StylusUp(shared->input->getCoordinates())) {
			shared->PlaySoundA(0);
		switch (page) {
			case PAGE1:
				page = PAGE2;
				break;
			case PAGE2:
				page = PAGE3;
				break;
			case PAGE3:
				page = PAGE4;
				break;
			case PAGE4:
				page = PAGE1;
				break;
			}
		}
	}
}
void Tutorial::StylusDown(Frog::Point stylus) {

}

void Tutorial::StylusMove(Frog::Point stylus) {
	b_menu->StylusMove(stylus);
	b_next->StylusMove(stylus);
}

void Tutorial::ButtonUp(int button) {
}

void Tutorial::ButtonDown(int button) {
}


void Tutorial::FinishAnimation() {
	animating= 0;
	boardy = -500;
}

void Tutorial::StartAnimation() {
	animating= 2;
	boardy = 0.5 * -500;
	animate = 0.5;
}