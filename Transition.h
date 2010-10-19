#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Module.h"
class Transition : public Module
{
	double timer;
	double wipetimer;
	double howlong;
	Surface *i_pixelate;
public:
	Transition(void);
public:
	~Transition(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
    virtual void StylusDown( Point stylus );
    virtual void StylusUp( Point stylus );
    virtual void StylusMove( Point stylus );
	void WipeLeft(Display *display);
	void WipeUp(Display *display);
	void WipeRight(Display *display);
	void WipeDown(Display *display);
	void MoveRight(Display *display);
	void MoveUp(Display *display);
	void MoveDown(Display *display);
	void MoveLeft(Display *display);
	void Fade(Display *display);
	void Blend(Display *display);
	void Pixelation(Display *display);
	Surface *Pixelate(Display *display,Surface *surface, int size);
	virtual void StartAnimation();
	virtual void FinishAnimation();

};
