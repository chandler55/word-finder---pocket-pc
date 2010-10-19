#include "HighScores.h"

HighScores::HighScores(void)
{
}

HighScores::~HighScores(void)
{
}


void HighScores::GameInit(Display *display)
{
	i_atime[0] = LoadImage(display, IDB_ATIME1);
	i_atime[1] = LoadImage(display, IDB_ATIME2);
	i_atime[2] = LoadImage(display, IDB_ATIME3);
	i_rgbtime[0] = LoadImage(display, IDB_RGBTIME1);
	i_rgbtime[1] = LoadImage(display, IDB_RGBTIME2);
	i_rgbtime[2] = LoadImage(display, IDB_RGBTIME3);
	i_background = LoadImage(display, IDB_HIGHSCORES);
	b_menu = new Button();
	b_menu->Init(ANIMATION_MOVELEFT, display, IDB_JOGGLEMENUUP, IDB_JOGGLEMENU,0,0,54,19);
	nextState = HIGHSCORES;
	boardy = -500;
}

void HighScores::GameEnd()
{
	
}

void HighScores::GameLoop(Display *display)
{
	if (animate) {
		Animate();
	} else {
		if (nextState != HIGHSCORES) {
			shared->gamestate = nextState;
			nextState= HIGHSCORES;
		}
	}
	
	
	// Gameplay
	Input();
	// Render
	Render(display);
}

void HighScores::Render(Display *display) {
	int i;
	display->Blit(0,0,i_background);
	b_menu->Render(display,boardy);
	TCHAR text[100];
	_stprintf(text, TEXT("ALL-TIME:"));
	shared->font->DisplayText(display, 25,55, text,Color(0,0,0));
	_stprintf(text, TEXT("LONGEST WORD: %s (%d LETTERS)"), shared->longestwordtext[shared->whatLength], shared->longestword[shared->whatLength]);
	shared->font->DisplayText(display, 25,65, text,Color(0,0,0));
	_stprintf(text, TEXT("HIGHEST SCORING WORD: %s (%d POINTS)"), shared->highestscoringwordtext[shared->whatLength], shared->highestscoringword[shared->whatLength]);
	shared->font->DisplayText(display, 25,75, text,Color(0,0,0));
	_stprintf(text, TEXT("STATISTICS:"));
	shared->font->DisplayText(display, 25,95, text,Color(0,0,0));
	_stprintf(text, TEXT("ACCUMULATED POINTS: %d"), shared->accumulatedpoints[shared->whatLength]);
	shared->font->DisplayText(display, 25,105, text,Color(0,0,0));
	_stprintf(text, TEXT("TOTAL WORDS FOUND: %d"), shared->totalwordsfound[shared->whatLength]);
	shared->font->DisplayText(display, 25,115, text,Color(0,0,0));
	_stprintf(text, TEXT("TOTAL LETTERS USED: %d"), shared->totallettersfound[shared->whatLength]);
	shared->font->DisplayText(display, 25,125, text,Color(0,0,0));
	_stprintf(text, TEXT("TOTAL %% WORDS FOUND: %.2f %%"), (float)shared->totalwordsfound[shared->whatLength] / (float)shared->totalwords[shared->whatLength]);
	shared->font->DisplayText(display, 25,135, text,Color(0,0,0));
	_stprintf(text, TEXT("TOP 5 HIGHSCORES"));
	shared->font->DisplayText(display, 20,185, text,Color(0,0,0));
	_stprintf(text, TEXT("TOP 5 %% WORDS FOUND"));
	shared->font->DisplayText(display, 140,185, text,Color(0,0,0));
	for ( i = 0; i < 5; i++) {
			_stprintf(text, TEXT("%d. %d"),i+1,shared->highscores[shared->whatLength][i]);
			shared->font->DisplayText(display,30,195 + (10 * i),text, Color(0,0,0));
			_stprintf(text, TEXT("%d. %.2f %%"),i+1,shared->highpercentscores[shared->whatLength][i]);
			shared->font->DisplayText(display,150,195 + (10 * i),text, Color(0,0,0));
		}
	display->Blit(30,260,i_rgbtime[shared->whatLength],i_atime[shared->whatLength]);

}

void HighScores::Animate() {
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
void HighScores::StylusDown(Frog::Point stylus) {
	b_menu->StylusDown(shared->input->getCoordinates());
}

void HighScores::StylusMove(Frog::Point stylus) {
	b_menu->StylusMove(shared->input->getCoordinates());
}

void HighScores::Input() {

	if (shared->input->StylusUp()) {
		if (b_menu->StylusUp(shared->input->getCoordinates())) {
			nextState = MAINMENU;
			shared->PlaySoundA(1);
			animate = 0.5;
			animating= 1;
		}
		if (Frog::Rect(30,260,30+56,260+34).Contains(shared->input->getCoordinates())) {
			shared->whatLength = 0;
			shared->PlaySoundA(0);
		}
		else if (Frog::Rect(90,260,90+56,260+34).Contains(shared->input->getCoordinates())) {
			shared->whatLength = 1;
			shared->PlaySoundA(0);
		}
		else if (Frog::Rect(159,260,159+56,260+34).Contains(shared->input->getCoordinates())) {
			shared->whatLength = 2;
			shared->PlaySoundA(0);
		}
	}
}
void HighScores::FinishAnimation() {
	animating= 0;
	boardy = -500;
}

void HighScores::StartAnimation() {
	animating= 2;
	boardy = 0.5 * -500;
	animate = 0.5;
}

