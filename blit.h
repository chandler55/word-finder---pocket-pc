 //////////////////////////////////////////////////////////////////////////////
//
// PocketFrog - The Game Library for Pocket PC Devices
// Copyright 2002  Thierry Tremblay
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Thierry Tremblay makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <PocketFrog/PocketFrog.h>
#include "Font.h"
#include "Shared.h"
#include "MainMenu.h"
#include "MainGame.h"
#include "Module.h"
#include "Particle.h"
#include "Button.h"
#include "Scrollbar.h"
#include "Transition.h"
#include "Options.h"
#include "Tutorial.h"
#include "HighScores.h"
#include "Dodge.h"
#include "Memory.h"
#include "Hangman.h"
#include "Anagram.h"
#include "Whac.h"
#include "LowToHigh.h"
#include "WordFinder.h"
#include "Joggle.h"
#include "Sudoku.h"
using namespace Frog;

#define NUM_MODULES 6

//////////////////////////////////////////////////////////////////////////////
//
// BlitSample
//
//////////////////////////////////////////////////////////////////////////////

class BlitSample : public Game
{
	Surface *i_loading;
	// Transitioning stuff
	Surface *beforeSurface;
	Surface *afterSurface;
	GameState before, after;
	uint32_t m_lastTime;
   uint32_t fps;
   uint32_t tickPerFrame;

   uint32_t timeNow;
	uint32_t m_lastTime2;
   uint32_t fps2;
   uint32_t tickPerFrame2;

   uint32_t timeNow2;

	// shared
	Shared *shared;
	char introscreen;
	//Modules
	Module *module[NUM_MODULES];
int buttonpressed;
	// Timing
	//timing
	double timer; //used to check the current time
	double timerdiff; //used to determine the time elapsed
	double secsperframe; //used to hold the value for how many seconds have elapsed between frames
	double movement; // sam as above
	double moveThisMuch;
public:

    // Construction
    BlitSample();

    // Framework callbacks
    virtual bool GameInit();
    virtual void GameEnd();
    virtual void GameLoop();
    virtual void StylusDown( Point stylus );
    virtual void StylusUp( Point stylus );
    virtual void StylusMove( Point stylus );
    // Button press/release events
    virtual void ButtonDown( int button );
    virtual void ButtonUp( int button );
	void DrawFPS(Display *display);
	void DrawDebug(Display *display);
	void ManualInit();

private:

		// Data
    Surface* i_numbers;         // Numbers to display FPS
    int      m_FPSCounter;      // FPS counter
    uint32_t m_FPSTicks[16];    // Ticks for last 16 frames

};