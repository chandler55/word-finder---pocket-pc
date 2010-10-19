#include "Transition.h"

Transition::Transition(void)
{
}

Transition::~Transition(void)
{
}


void Transition::GameInit(Display *display)
{
	nextState = TRANSITION;
	timer = 0;
	wipetimer = 0;
	howlong= 0.5;
	i_pixelate = new Surface(240,320);
}



void Transition::GameEnd()
{

}



void Transition::GameLoop(Display *display)
{
	Render(display);
	timer += shared->delta_time;
	if (timer > howlong) {
		timer = 0;
		wipetimer = 0;
		shared->gamestate = shared->nextstate;
	}
}

void Transition::Render(Display *display) {
	switch (shared->transitiontype) {
		case WIPEDOWN:
			WipeDown(display);
			break;
		case WIPEUP:
			WipeUp(display);
			break;
		case WIPERIGHT:
			WipeRight(display);
			break;
		case WIPELEFT:
			WipeLeft(display);
			break;
		case MOVERIGHT:
			MoveRight(display);
			break;
		case MOVELEFT:
			MoveLeft(display);
			break;
		case MOVEUP:
			MoveUp(display);
			break;
		case MOVEDOWN:
			MoveDown(display);
			break;
		case FADE:
			Fade(display);
			break;
		case BLEND:
			Blend(display);
			break;
		case PIXELATION:
			Pixelation(display);
			break;
	}
}

void Transition::MoveRight(Display *display) {
	display->Blit((int)((timer/howlong)*240)-240,0,shared->after);
	display->Blit((int)((timer/howlong)*240),0,shared->before);
}
void Transition::MoveDown(Display *display) {
	display->Blit(0,(int)((timer/howlong)*320)-320,shared->after);
	display->Blit(0,(int)((timer/howlong)*320),shared->before);
}
void Transition::MoveLeft(Display *display) {
	display->Blit((int)((timer/howlong)*240)*-1+240,0,shared->after);
	display->Blit((int)((timer/howlong)*240)*-1,0,shared->before);
}
void Transition::MoveUp(Display *display) {
	display->Blit(0,(int)((timer/howlong)*320)*-1+320,shared->after);
	display->Blit(0,(int)((timer/howlong)*320)*-1,shared->before);
}
void Transition::WipeDown(Display *display) {
	display->Blit(0,0,shared->after);
	display->Blit(0,(int)((timer/howlong)*320),shared->before);
}
void Transition::WipeUp(Display *display) {
	display->Blit(0,0,shared->after);
	display->Blit(0,(int)((timer/howlong)*320)*-1,shared->before);
}
void Transition::WipeRight(Display *display) {
	display->Blit(0,0,shared->after);
	display->Blit((int)((timer/howlong)*240),0,shared->before);
}
void Transition::WipeLeft(Display *display) {
	display->Blit(0,0,shared->after);
	display->Blit((int)((timer/howlong)*240)*-1,0,shared->before);
}
void Transition::Fade(Display *display) {
	if ((timer/howlong)*255 > 128) {
		display->SetBlending((PHAL::uint8_t)(((timer/howlong)*255)*2));
		display->Blit(0,0,shared->after);
	} else {
		display->SetBlending((PHAL::uint8_t)(255-((timer/howlong)*255)*2));
		display->Blit(0,0,shared->before);
	}
	display->SetBlending(255);
}
void Transition::Blend(Display *display) {
	display->SetBlending((PHAL::uint8_t)((timer/howlong)*255));
	display->Blit(0,0,shared->after);
	display->SetBlending((PHAL::uint8_t)(255-((timer/howlong)*255)));
	display->Blit(0,0,shared->before);
	display->SetBlending(255);
}

void Transition::Pixelation(Display *display) {
	if (timer/howlong < 0.1)
		display->Blit(0,0,Pixelate(display,shared->before,128));
	else if (timer/howlong < 0.2)
		display->Blit(0,0,Pixelate(display,shared->before,64));
	else if (timer/howlong < 0.3)
		display->Blit(0,0,Pixelate(display,shared->before,32));
	else if (timer/howlong < 0.4)
		display->Blit(0,0,Pixelate(display,shared->before,16));
	else if (timer/howlong < 0.5)
		display->Blit(0,0,Pixelate(display,shared->after,16));
	else if (timer/howlong < 0.6)
		display->Blit(0,0,Pixelate(display,shared->after,32));
	else if (timer/howlong < 0.7)
		display->Blit(0,0,Pixelate(display,shared->after,64));
	else if (timer/howlong < 0.8)
		display->Blit(0,0,Pixelate(display,shared->after,128));
}
void Transition::StylusDown( Point stylus )
{

}

void Transition::StylusUp( Point stylus )
{
}



void Transition::StylusMove( Point stylus )
{

}

Surface *Transition::Pixelate(Display *display,Surface *surface, int size) {
	int i, j;
	display->SetRenderTarget(surface);
	Frog::Surface::LockInfo lockinfo;
	surface->Lock(lockinfo,NULL,NULL);
	for ( i = 0; i < 320; i+=320/size)
		for (j = 0; j < 240; j+=240/size) {
			display->FillRect(j,i,j+240/size,i+320/size,lockinfo.pixels[j + (240*i)]);
		}
	surface->Unlock();
	display->SetRenderTarget(NULL);
	return surface;
}
/*Frog::Surface::LockInfo lockinfo;
surface->Lock(lockinfo,NULL,NULL);
lockinfo.pixels[j + (240*i)] = Color(255,0,255);
surface->Unlock();
*/
void Transition::FinishAnimation() {
}

void Transition::StartAnimation() {
}