#pragma once
#include "Shared.h"
#include "resource.h"
#include <PocketFrog/PocketFrog.h>


using namespace Frog;

class Module
{
protected:
	GameState nextState;
	Shared *shared;
	Surface *i_background;
public:
	Module(void);
	~Module(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
    virtual void StylusDown( Point stylus );
    virtual void StylusUp( Point stylus );
	virtual void ButtonUp(int button);
	virtual void ButtonDown(int button);
    virtual void StylusMove( Point stylus );
	void LinkShared(Shared *f_shared) { shared = f_shared; }
	virtual void StartAnimation();
	virtual void FinishAnimation();
};
