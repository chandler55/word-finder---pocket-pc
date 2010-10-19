#include "Scrollbar.h"

Scrollbar::Scrollbar(void)
{
	
}

Scrollbar::~Scrollbar(void)
{
}
void Scrollbar::Init(Display *display) {
	height = 120;
	width = 200;
	scrollbox= new Surface(width,height);
	scrolly = 0;
	scrollPressed = false;
	scrollbarx = 30;
	scrollbary = 190;
	number = 100;
	InitGraphics(display);
}
void Scrollbar::Init(Display *display, int fheight, int fwidth, int x, int y) {
	height = fheight;
	width = fwidth;
	scrollbox= new Surface(width,height);
	scrolly = 0;
	scrollPressed = false;
	scrollbarx = x;
	scrollbary = y;
	number = 100;
	InitGraphics(display);
}
void Scrollbar::InitGraphics(Display *display) {
	i_toparrow = LoadImage(display, IDB_TOPARROW);
	i_bottomarrow = LoadImage(display, IDB_BOTTOMARROW2);
	i_scrollbar= LoadImage(display, IDB_SCROLLBAR);
}
void Scrollbar::GameLoop(Shared *shared) {	
	if (shared->input->StylusDown())
		if (Rect(scrollbarx+width-10,scrollbary+scrolly,scrollbarx+width-10+10,scrollbary+10+scrolly).Contains(shared->input->getCoordinates()))
			scrollPressed = true;
	if(shared->input->StylusUp())
		scrollPressed = false;

	if (scrollPressed)
		scrolly = shared->input->getCoordinates().y - scrollbary - 10 +5;

	if (shared->input->StylusPressed()) {
		if (Rect(scrollbarx+width-10,scrollbary,scrollbarx+width-10+10,scrollbary+10).Contains(shared->input->getCoordinates())) {
			if (scrolly == height -20) 
				scrolly -= (float) (height-30) / number;
			scrolly-=(float)(height-30)/number;

		}
		else if (Rect(scrollbarx+width-10,scrollbary+height-10,scrollbarx+width-10+10,scrollbary+10+height-10).Contains(shared->input->getCoordinates()))

			scrolly+=(float)(height-30)/number;
	}

	if (scrolly < 10)
		scrolly = 10;
	if (scrolly > height-20) {
		scrolly = height-20;
	}
}
void Scrollbar::ResetScrollY() {
	scrolly = 10;
}
void Scrollbar::MoveDownOne(int num_found) {
	number = num_found;
	scrolly = height-20;
	scrolly-=6*(float)(height-30)/number;
}
void Scrollbar::Render(Display *display, Shared *shared, TCHAR *found[1000], int num_found, TCHAR *points[1000]) {
	int i, j; 
	int fontheight= 12;
	fontheight= 12;
	TCHAR buffer[100], buffer2[100];
	display->SetRenderTarget(scrollbox);
	display->Clear(Color(245,243,206));
	number = num_found;
	for (i = 0; i < num_found; i++) {
		if (found[i] == 0)
			break;
		float position_y = 0 - (((number-1)*fontheight)*(scrolly-10)/(height-30)) + (i*fontheight) - 2;
		float temp = position_y;
		position_y = (int)(((position_y+2)/fontheight)) * fontheight - 2;

		if (found[i][0] > 90)
		for (j = 0; found[i][j]; j++)
			found[i][j] = found[i][j]-32;

		_stprintf(buffer, found[i], i, scrolly-10);
		if (points[i])
		_stprintf(buffer2, points[i], i, scrolly-10);
		if (temp >= -2) {
			shared->bookmanfont->DisplayText(display,5,position_y,buffer,Color(0,0,0));
			if (points[i])
			shared->bookmanfont->DisplayText(display, 85, position_y, buffer2, Color(0,0,0));
		}
	}
	display->Blit(width-10, 0, i_toparrow);
	//display->FillRect(width-10,scrolly,width-10+10,10+scrolly,Color(0,128,128));
	display->Blit(width-10, scrolly, i_scrollbar);
	display->Blit(width-10, height-10, i_bottomarrow);
	//display->FillRect(width-10,height-10,width-10+10,height,Color(255,128,128));

	display->SetRenderTarget(NULL);
	display->Blit(scrollbarx,scrollbary,scrollbox);
}
void Scrollbar::Render(Display *display, Shared *shared, TCHAR *found[1000], int num_found, TCHAR *points[1000], bool founded[1000]) {
	int i, j; 
	int fontheight= 12;
	fontheight= 12;
	TCHAR buffer[100], buffer2[100];
	display->SetRenderTarget(scrollbox);
	display->Clear(Color(245,243,206));
	number = num_found;
	for (i = 0; i < num_found; i++) {
		if (found[i] == 0)
			break;
		float position_y = 0 - (((number-1)*fontheight)*(scrolly-10)/(height-30)) + (i*fontheight) - 2;
		float temp = position_y;
		position_y = (int)(((position_y+2)/fontheight)) * fontheight - 2;

		if (found[i][0] > 90)
		for (j = 0; found[i][j]; j++)
			found[i][j] = found[i][j]-32;

		_stprintf(buffer, found[i], i, scrolly-10);
		if (points[i])
		_stprintf(buffer2, points[i], i, scrolly-10);
		Frog::Pixel colorFont = Color(0,0,0);
		if (founded[i])
			colorFont = Color(0,128,0);
		if (temp >= -2) {
			shared->bookmanfont->DisplayText(display,5,position_y,buffer,colorFont);
			if (points[i])
			shared->bookmanfont->DisplayText(display, 85, position_y, buffer2, colorFont);
		}
	}
	display->Blit(width-10, 0, i_toparrow);
	//display->FillRect(width-10,scrolly,width-10+10,10+scrolly,Color(0,128,128));
	display->Blit(width-10, scrolly, i_scrollbar);
	display->Blit(width-10, height-10, i_bottomarrow);
	//display->FillRect(width-10,height-10,width-10+10,height,Color(255,128,128));

	display->SetRenderTarget(NULL);
	display->Blit(scrollbarx,scrollbary,scrollbox);
}
