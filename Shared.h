#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Font.h"
#include "File.h"
#include <HSS.h>
#include "Input.h"
#include "Keyboard.h"
using namespace Frog;

enum GameState { MAINMENU, TRANSITION, TUTORIAL, HIGHSCORES, EXIT, JOGGLE};
enum TransitionType { BLEND, WIPELEFT, WIPEUP, WIPEDOWN, WIPERIGHT,MOVERIGHT, MOVELEFT, MOVEUP, MOVEDOWN, FADE, PIXELATION };
class Shared
{
public:
	char whatLength;
	bool newgame;
	Keyboard *keyboard;
	Input *input;
	PHAL::KeyList *m_keys;
	Font *font, *bigfont, *font8, *font16, *centaurfont, *bookmanfont;
	GameState gamestate;
	GameState nextstate;
	File *file;
	double delta_time;
	hssSpeaker m_speaker;
	hssSound   m_sounds[5];
	Surface *before, *after;
	TransitionType transitiontype;
	char sound;
	int trial;
	double trialtime;
	
	//high score stuff
	int longestword[3], highestscoringword[3],accumulatedpoints[3], totalwordsfound[3],totallettersfound[3];
	TCHAR longestwordtext[3][10], highestscoringwordtext[3][10];
	int highscores[3][5];
	float highpercentscores[3][5];
	int totalwords[3];
public:
	Shared(void);
	void Init(Display *display, PHAL::KeyList *f_keys);
	void End();
	void PlaySoundA(int sound_id) {
		if (sound)
			m_speaker.playSound(&m_sounds[sound_id], NULL); 
	}
public:
	~Shared(void);
	void SetFromSurface(Surface *surface) { before = surface; }
	void SetToSurface(Surface *surface) { after = surface; }
	void RenderKeyboard(Display *display);
	void AnimateKeyboard(Display *display);
};