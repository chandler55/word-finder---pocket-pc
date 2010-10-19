#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Font.h"
#include "Input.h"
using namespace Frog;

class Keyboard
{
	Surface *sip, *sipdown;
	int cursor;
	TCHAR name[10];
	bool caps, shift;
public:
	Keyboard(void);
public:
	void Init(Display *display);
	~Keyboard(void);	
	void Render(Display *display, Input *input, Font *font8, Font *font16);
	void Animate(Display *display, Input *input, Font *font8, Font *font16);
};
