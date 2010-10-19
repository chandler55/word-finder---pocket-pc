#pragma once
#include "module.h"
#include "Scrollbar.h"

#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define PLAYER 0
#define COMPUTER 1
#define READY 2
#define GAMEOVER 3
class Memory :
	public Module
{
	int pattern[100];
	int playerpattern[100];
	int step;
	int temp_step;
	int size;
	int state;
	float step_time;
	float ready_time;
	int player_step;
	char highlightPad[4];
	Surface *scrollbox;

	int scrollbarx, scrollbary;
	double scrolly;
	bool scrollPressed;
	int number;
	Scrollbar *scrollbar;
public:
	Memory(void);
public:
	~Memory(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
	virtual void StartAnimation();
	virtual void FinishAnimation();
};
