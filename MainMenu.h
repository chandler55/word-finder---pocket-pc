#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Module.h"
#include "Shared.h"
#include "Button.h"
using namespace Frog;

class MainMenu : public Module
{
	float boardy;
	float animate;
	Button *b_newgame, b_options, b_exit, b_tutorial, b_highscores;
	Surface *i_tiles, *i_tile1, *i_atime[3], *i_rgbtime[3];
	//Surface *i_background;
	int whichtile;
	int buttontest;
	char animating;
	//TCHAR 
public:
	MainMenu(void);
public:
	~MainMenu(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
	virtual void StartAnimation();
	virtual void FinishAnimation();
	void Input();
	void Animate();
	void StylusDown(Point stylus);
	void StylusMove(Point stylus);
};
