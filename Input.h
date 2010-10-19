#pragma once
#include "resource.h"
#include <PocketFrog/PocketFrog.h>
using namespace Frog;
class Input
{
	Point mousecoordinates, lastmousecoordinates;
	bool stylusup;
	bool stylusdown;
	bool buttonup, buttondown;
	int16_t button;
	char buttonupcounter, styluscounter;
	bool styluspressed;
public:
	Input(void);
	bool StylusUp() {return stylusup; }
	bool StylusDown() {return stylusdown; }
	int ButtonPressed();
	int ButtonIsDown();
	bool StylusPressed();
	bool StylusIsDown();
	void Process();
	void StylusDown(Point stylus);
	bool StylusIsPressed();
	void StylusUp(Point stylus);
	void StylusMove(Point stylus);
	void ButtonUp(int16_t button2);
	void ButtonDown(int16_t button2);
	Point getCoordinates() { return mousecoordinates; }
	Point getLastCoordinates() { return lastmousecoordinates; }
public:
	~Input(void);
};
