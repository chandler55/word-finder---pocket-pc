#include "Input.h"

Input::Input(void)
{
	styluscounter = buttonupcounter= 0;
    button = buttonup = buttondown = false;
	styluspressed = stylusdown = stylusup = false;
}

Input::~Input(void)
{

}

void Input::Process() { 
	if (buttonupcounter <= 1 && buttonup)
		buttonupcounter++;

	if (buttonupcounter == 2 && buttonup) {
		buttonupcounter= 0;
		buttonup = false;
	}
	if (styluscounter <= 1 && stylusup)
		styluscounter++;

	if (styluscounter == 2 && stylusup) {
		styluscounter= 0;
		stylusup = false;
	}

}

void Input::StylusDown(Point stylus) { 
	lastmousecoordinates = mousecoordinates;
	mousecoordinates = stylus;
	stylusdown= true;
	styluspressed= true;
}

void Input::StylusUp(Point stylus) {
	lastmousecoordinates = mousecoordinates;
	mousecoordinates = stylus;
	stylusup = true;
	stylusdown= false;
	styluspressed = false;
}

void Input::StylusMove(Point stylus) {
	lastmousecoordinates = mousecoordinates;
	mousecoordinates = stylus;
	styluspressed= false;

}
void Input::ButtonUp(int16_t button2) { 
	buttonup = true;
	buttondown= false;
	button = button2;

}
void Input::ButtonDown(int16_t button2) { 
	buttondown = true;
	button = button2;
}

int Input::ButtonPressed() {
	if (buttonup)
		return button; 
	else
		return -1;
}
int Input::ButtonIsDown() {
	if (buttondown)
		return button; 
	else
		return -1;
}
bool Input::StylusPressed() {
	return stylusup;
}
bool Input::StylusIsPressed() {
	return styluspressed;
}

bool Input::StylusIsDown() {
	return stylusdown;
}