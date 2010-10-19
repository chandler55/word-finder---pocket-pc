#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Module.h"
#include "Button.h"

class HighScores :
	public Module
{
	char animating;
	float animate;
	float boardy;
	Button *b_menu;

	Surface *i_atime[3], *i_rgbtime[3];

public:
	HighScores(void);
public:
	~HighScores(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
	void Animate();
	virtual void StartAnimation();
	virtual void FinishAnimation();
	void Input();
	void StylusDown(Frog::Point stylus);
	void StylusMove(Frog::Point stylus);
};
