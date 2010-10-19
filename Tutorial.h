#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Module.h"
#include "Button.h"
enum RULESPAGES { PAGE1, PAGE2, PAGE3, PAGE4 };
class Tutorial :
	public Module
{
	RULESPAGES page;
	Surface *i_background2, *i_background3, *i_background4;
	char animating;
	float animate;
	Button *b_menu, *b_next;
	float boardy;
public:
	Tutorial(void);
public:
	~Tutorial(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
    virtual void StylusDown( Point stylus );
    virtual void StylusMove( Point stylus );
	virtual void ButtonUp(int button);
	virtual void ButtonDown(int button);
	virtual void StartAnimation();
	virtual void FinishAnimation();
	void Input();
	void Animate();
};
