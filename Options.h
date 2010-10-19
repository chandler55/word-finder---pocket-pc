#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Module.h"
#include "Button.h"
class Options :
	public Module
{
	Button b_exitdown;
	Surface *i_levelselect, *i_soundonselect, *i_soundoffselect, *i_choice;
	TCHAR button[20];
	int redefining;
public:
	Options(void);
public:
	~Options(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
    virtual void StylusDown( Point stylus );
    virtual void StylusUp( Point stylus );
    virtual void StylusMove( Point stylus );
	virtual void ButtonUp(int button);
	virtual void ButtonDown(int button);
	virtual void StartAnimation();
	virtual void FinishAnimation();
};
