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

#include "blit.h"
#include "resource.h"
#include <HSS.h>
using namespace Frog;



//////////////////////////////////////////////////////////////////////////////
//
// _Module : This is needed to use ATL Windows
//
//////////////////////////////////////////////////////////////////////////////

CComModule _Module;

//////////////////////////////////////////////////////////////////////////////
//
// BlitSample
//
//////////////////////////////////////////////////////////////////////////////

BlitSample::BlitSample()
{

	introscreen = 1;
	buttonpressed= 0;
    m_config.szWindowName = _T( "Prototype" );
    m_config.orientation  = ORIENTATION_NORMAL;
}



bool BlitSample::GameInit()
{
	  Display* display = GetDisplay();
	  i_loading = LoadImage(display, IDB_LOADING);
return true;	

}
void BlitSample::ManualInit() {
	int i;
	timerdiff = secsperframe = movement = moveThisMuch = 0;
	timer = PocketPC::GetTickCount();

	// Initialize random seed
    srand( PocketPC::GetTickCount() );

	// Initialize video stuff
    Display* display = GetDisplay();
	beforeSurface = new Surface(240,320);
	afterSurface = new Surface(240,320);

	module[MAINMENU] = new MainMenu();
	//module[1] = new MainGame();
	//module[2] = new Options();
	module[TRANSITION] = new Transition();
	module[TUTORIAL] = new Tutorial();
	module[HIGHSCORES] = new HighScores();
	//module[DODGE] = new Dodge();
	module[EXIT] = new MainMenu();
	/*module[MEMORY] = new Memory();
	module[HANGMAN] = new Hangman();
	module[ANAGRAM] = new Anagram();
	module[WHAC] = new Whac();
	module[LOWTOHIGH] = new LowToHigh();
	module[WORDFINDER] = new WordFinder();*/
	module[JOGGLE] = new Joggle();
	//module[SUDOKU] = new Sudoku();


	// link shared
	shared = new Shared();
	shared->Init(display, &m_keys);
	shared->file->ErrorWrite("initialized shared structure");

	// init modules
	for (i = 0; i < NUM_MODULES; i++) {
		module[i]->LinkShared(shared);
		module[i]->GameInit(display);
	}
	shared->file->ErrorWrite("linked shared structure");
	shared->file->ErrorWrite("initialized modules");

	//images
    i_numbers = LoadImage( display, IDB_NUMBERS );
	
    m_FPSCounter = 0;
	m_lastTime = 0;
 
}
void BlitSample::GameEnd()
{
		if (introscreen)
		return;
	int i;
	for (i = 0; i < NUM_MODULES; i++)
			module[i]->GameEnd();

	shared->End();
	delete i_numbers;
}



void BlitSample::GameLoop()
{
	if (introscreen == 2) {
		ManualInit();
		introscreen = 0;
	}
	else if (introscreen == 1) {
		Display* display = GetDisplay();
		display->GetBackBuffer();
		display->Clear(Color(255,255,255));
		display->Blit(82,146,i_loading);
		display->Update();
		introscreen++;
		return;
	}
	shared->input->Process();

	if (shared->gamestate == EXIT)
		return;

	// Render
    Display* display = GetDisplay();
    display->GetBackBuffer();
	display->Clear(Color(0,0,0));
	
	before = shared->gamestate;
	module[shared->gamestate]->GameLoop(display);
    after = shared->gamestate;
	   /**/

//	DrawFPS(display);
	//DrawDebug(display);
	//TCHAR paused3[200];
	//_stprintf(paused3, TEXT("%d %d %d"),  buttonpressed, m_keys.vkUp, m_keys.vkDown);
//	shared->bigfont->DisplayText(display,10,30,paused3,Color(255,0,255));

    display->Update();

	// transition
	if (before != after && before != TRANSITION) {
		if (after == EXIT) {
			Shutdown();
			return;
		}
		//display = GetDisplay();
		display->SetRenderTarget(beforeSurface);
		module[before]->Render(display);
		display->Update();
		
	//	display = GetDisplay();
		display->SetRenderTarget(afterSurface);
		if (shared->newgame && after == JOGGLE)
			module[JOGGLE]->StartAnimation();

		module[after]->Render(display);
		display->Update();

		display->SetRenderTarget(NULL);
		shared->SetFromSurface(beforeSurface);
		shared->SetToSurface(afterSurface);

		shared->gamestate = TRANSITION;
		shared->nextstate = after;
		module[before]->FinishAnimation();
	}
	if (before == TRANSITION) {
		module[after]->StartAnimation();
	}

	// timing
	timerdiff = PocketPC::GetTickCount()-timer;
	secsperframe=(timerdiff/1000.0f);
	movement = secsperframe;
	moveThisMuch = fabs(secsperframe);// framerate independent movement
	if (moveThisMuch > 0)
		shared->delta_time = moveThisMuch;
	timer = PocketPC::GetTickCount();

}


void BlitSample::StylusDown( Point stylus )
{
	if (introscreen)
		return;
	shared->input->StylusDown(stylus);
	module[shared->gamestate]->StylusDown(stylus);
}

void BlitSample::StylusUp( Point stylus )
{
	if (introscreen)
		return;
	shared->input->StylusUp(stylus);
	module[shared->gamestate]->StylusUp(stylus);

}

void BlitSample::StylusMove( Point stylus )
{
		if (introscreen)
		return;
	shared->input->StylusMove(stylus);
	module[shared->gamestate]->StylusMove(stylus);
}

void BlitSample::ButtonDown(int button) {
	if (introscreen)
		return;
         if(m_keys.vkUp == button || m_keys.vkDown == button ||
 m_keys.vkLeft == button || m_keys.vkRight == button || m_keys.vkStart ==
 button || m_keys.vkA == button || m_keys.vkB == button || m_keys.vkC ==
 button || m_keys.vkAux1 == button || m_keys.vkAux2 == button ||
 m_keys.vkAux3 == button || m_keys.vkAux4 == button || m_keys.vkAux5 ==
 button || m_keys.vkAux6 == button || m_keys.vkAux7 == button ||
 m_keys.vkAux8 == button){
	buttonpressed=button;

	shared->input->ButtonDown(button);
	module[shared->gamestate]->ButtonDown(button);
		 }
}

void BlitSample::ButtonUp(int button) {
	if (introscreen)
		return;
         if(m_keys.vkUp == button || m_keys.vkDown == button ||
 m_keys.vkLeft == button || m_keys.vkRight == button || m_keys.vkStart ==
 button || m_keys.vkA == button || m_keys.vkB == button || m_keys.vkC ==
 button || m_keys.vkAux1 == button || m_keys.vkAux2 == button ||
 m_keys.vkAux3 == button || m_keys.vkAux4 == button || m_keys.vkAux5 ==
 button || m_keys.vkAux6 == button || m_keys.vkAux7 == button ||
 m_keys.vkAux8 == button) {
	buttonpressed=button;
	shared->input->ButtonUp(button);
	module[shared->gamestate]->ButtonUp(button);
}
}

//////////////////////////////////////////////////////////////////////////////
//
// WinMain - Entry point
//
//////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
    _Module.Init( 0, hInstance );
    
    BlitSample game;
    
    game.Run();
    
    return 0;
}
void BlitSample::DrawFPS(Display *display) {

    // Update FPS
    m_FPSTicks[ m_FPSCounter & 15 ] = PocketPC::GetTickCount();
    if (m_FPSCounter > 15)
    {
        uint32_t totalTime = m_FPSTicks[ m_FPSCounter & 15 ] - m_FPSTicks[ (m_FPSCounter+1) & 15 ];
        if (totalTime == 0) totalTime = 1;
        uint32_t fps  = 16000 / totalTime;
        uint32_t n100 = fps / 100; fps -= n100 * 100;
        uint32_t n10  = fps / 10;  fps -= n10 * 10;
        uint32_t n1   = fps;

        if (n100 > 9) { n100 = 9; n10 = 9; n1 = 9; }
        
        // Display fps
        display->Blit(  0, 0, i_numbers, &Rect( n100 * 8, 0, (n100+1)*8, 11 ) );
        display->Blit(  8, 0, i_numbers, &Rect( n10 * 8, 0, (n10+1)*8, 11 ) );
        display->Blit( 16, 0, i_numbers, &Rect( n1 * 8, 0, (n1+1)*8, 11 ) );
    }
    ++m_FPSCounter;
}
void BlitSample::DrawDebug(Display *display) {
	/*//debugging
	TCHAR debug[30];
	_stprintf(debug,"%f",moveThisMuch);
	shared->bigfont->DisplayText(display,50,70,debug, Color(255,0,255));
	shared->file->ErrorWrite(debug);
	/**/
}