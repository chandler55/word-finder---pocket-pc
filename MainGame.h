#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Module.h"
#include "Button.h"
#include "Particle.h"

class MainGame : public Module
{
	float sin_look[361];
	float cos_look[361];
	float wave;
	Surface *i_blocks, *i_background,
		*i_blueblock, *i_redblock, *i_purpleblock,
		*i_greenblock, *i_yellowblock, *i_cyanblock
		,*i_bottombar, *i_endless, *i_endlessmask,
		*i_newhighscore, *i_newhighscoremask;
	double lock60;
	Frog::Rect src;
	int lastX, lastY;
	int whatRow;

	uint32_t m_lastTime;
   uint32_t fps;
   uint32_t tickPerFrame;

   uint32_t timeNow;

		// Data
    Surface* i_numbers;         // Numbers to display FPS
    int      m_FPSCounter;      // FPS counter
    uint32_t m_FPSTicks[16];    // Ticks for last 16 frames
public:
	MainGame(void);
public:
	~MainGame(void);
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
    virtual void StylusDown( Point stylus );
    virtual void StylusUp( Point stylus );
    virtual void StylusMove( Point stylus );
	virtual void ButtonUp(int button);
	virtual void ButtonDown(int button);
	void Input();
	void DrawFPS(Display *display);
private:
	void Render();
	void DebugInfo(Display* display);
	void RenderText(Display *display);
	Surface* m_background;
	void Render_1P(Display *display);
	virtual void StartAnimation();
	virtual void FinishAnimation();
};
