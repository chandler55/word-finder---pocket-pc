#include "Memory.h"

Memory::Memory(void)
{
}

Memory::~Memory(void)
{
}

void Memory::GameEnd() {
}

void Memory::GameInit(Display *display) {
	int i;
	scrollbar = new Scrollbar();
	scrollbar->Init(display);
	nextState = MEMORY;

	
	size = 0;
	for (i = 0; i < 100;i++)
		pattern[i] = 0;
	state = READY;
	step = 0;
	step_time = 1;
	ready_time = 3;
	temp_step = 0;
	for (i = 0; i < 4; i++)
		highlightPad[i] = 0;
	for (i = 0; i < 100;i++)
		playerpattern[i] = 0;

}

void Memory::GameLoop(Frog::Display *display) {
	int i;
	if (nextState != MEMORY) {
		shared->gamestate = nextState;
		nextState= MEMORY;
	}

	if (state == READY) {
		ready_time-= shared->delta_time;
		if (ready_time < 0)
			state = COMPUTER;
	}
	if (state == COMPUTER) {

		step_time+= shared->delta_time;
		if (step_time > 0.5)
			for (i = 0; i < 4; i++)
				highlightPad[i] = 0;
		if (step_time > 1 ) {

			if (pattern[temp_step]) {
				highlightPad[pattern[temp_step]-1] = 1;
				temp_step++;
				step_time = 0;
			} else {
				for (i = 0; i < 100;i++)
					playerpattern[i] = 0;
				player_step=0;
				state = PLAYER;
				step_time = 0;
				pattern[temp_step] = rand() % 4 + 1;
				step++;
			}
		}
	}
	if (state == PLAYER) {
		if (pattern[step-2] == playerpattern[step-2] || step == 1) {
			temp_step = 0;
			state = COMPUTER;
			for (i = 0; i < 4; i++)
				highlightPad[i] = 0;
		}
		if (shared->input->StylusDown()) {
			for (i = 0; i < 4; i++)
				highlightPad[i] = 0;
			if (Rect(20,20,20+75,20+75).Contains(shared->input->getCoordinates()))
				highlightPad[0] = 1;
			if (Rect(100,20,100+75,20+75).Contains(shared->input->getCoordinates()))
				highlightPad[1] = 1;
			if (Rect(20,100,20+75,100+75).Contains(shared->input->getCoordinates()))
				highlightPad[2] = 1;
			if (Rect(100,100,100+75,100+75).Contains(shared->input->getCoordinates()))
				highlightPad[3] = 1;
		}
		if (shared->input->StylusPressed()) {
			for (i = 0; i < 4; i++)
				highlightPad[i] = 0;
			int temp = player_step;
			if (Rect(20,20,20+75,20+75).Contains(shared->input->getCoordinates())) 
				playerpattern[player_step++] = RED;
			if (Rect(100,20,100+75,20+75).Contains(shared->input->getCoordinates()))
				playerpattern[player_step++] = GREEN;
			if (Rect(20,100,20+75,100+75).Contains(shared->input->getCoordinates()))
				playerpattern[player_step++] = BLUE;
			if (Rect(100,100,100+75,100+75).Contains(shared->input->getCoordinates()))
				playerpattern[player_step++] = YELLOW;
			if (temp != player_step) {
				if (playerpattern[player_step-1] != pattern[player_step-1])
					state = GAMEOVER;
			}
		}
	}

	scrollbar->GameLoop(shared);
	Render(display);
}

void Memory::Render(Frog::Display *display) {
	int i;
	display->Clear(Color(0,0,0));

	if (highlightPad[0])
		display->FillRect(20,20,20+75,20+75,Color(255,0,0));
	if (highlightPad[1])
		display->FillRect(100,20,100+75,20+75,Color(0,255,0));
	if (highlightPad[2])
		display->FillRect(20,100,20+75,100+75,Color(0,0,255));
	if (highlightPad[3])
		display->FillRect(100,100,100+75,100+75,Color(255,255,0));

	TCHAR buffer[20];
	_stprintf(buffer, TEXT("%f"), step_time);
	shared->font16->DisplayText(display,60,150,buffer,Color(0,255,255));
	_stprintf(buffer, TEXT("step %d"), step);
	shared->font16->DisplayText(display,60,190,buffer,Color(0,255,255));
	_stprintf(buffer, TEXT("temp_step %d"), temp_step);
	shared->font16->DisplayText(display,60,210,buffer,Color(0,255,255));
	if (state == PLAYER) {
	_stprintf(buffer, TEXT("PLAYERS TURN"));
	shared->font16->DisplayText(display,60,230,buffer,Color(0,255,255));
	}
	if (state == READY) {
		_stprintf(buffer, TEXT("%d"), (int)ready_time + 1);
		shared->font16->DisplayText(display,60,170,buffer,Color(0,255,255));
	}

	for (i = 0; i < 10; i++) {
		_stprintf(buffer, TEXT("%d"), pattern[i]);
		shared->font16->DisplayText(display,10 + i *10,280,buffer,Color(0,255,255));
		_stprintf(buffer, TEXT("%d"), playerpattern[i]);
		shared->font16->DisplayText(display,10 + i *10,300,buffer,Color(0,255,255));
	}
	if (state == GAMEOVER) {
		_stprintf(buffer, TEXT("GAMEOVER"));
		shared->font16->DisplayText(display,120,160,buffer,Color(0,255,255));
	}
//	scrollbar->Render(display,shared, found);
}
void Memory::FinishAnimation() {
}

void Memory::StartAnimation() {
}