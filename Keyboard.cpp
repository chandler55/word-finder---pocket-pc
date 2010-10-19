#include "Keyboard.h"
#include "Input.h"

Keyboard::Keyboard(void)
{

}

Keyboard::~Keyboard(void)
{
}

void Keyboard::Init(Display *display) {
	sip = LoadImage(display,IDB_SIP);
	sip->SetColorMask(Color(255,0,255));
	sipdown = LoadImage(display,IDB_SIPDOWN);
	sipdown->SetColorMask(Color(255,0,255));
	cursor = -1;
	shift = caps = false;
	//name[cursor] = '5';
}
void Keyboard::Animate(Display *display, Input *input, Font *font8, Font *font16) {
	int x = 0,y = 0;
	int i;
	display->Blit(0+x,320-92+y,sip);
	TCHAR toprow[14], middlerow[12], bottomrow[12], lastrow[12];
	_stprintf(toprow,TEXT("1234567890-="));
	_stprintf(middlerow,TEXT("qwertyuiop"));
	_stprintf(bottomrow,TEXT("asdfghjkl;"));
	_stprintf(lastrow,TEXT("zxcvbnm,./"));
	TCHAR toprow2[14], middlerow2[12], bottomrow2[12], lastrow2[12];
	_stprintf(toprow2,TEXT("!@#$%%^&*()_+"));
	_stprintf(middlerow2,TEXT("QWERTYUIOP"));
	_stprintf(bottomrow2,TEXT("ASDFGHKJL:"));
	_stprintf(lastrow2,TEXT("ZXCVBNM<>?"));	

	if (input->StylusPressed())
		shift = false;
	if (input->StylusPressed() && Frog::Rect(189-186,y+250+17,189-186+21,y+250+17+16).Contains(input->getCoordinates()))
		caps = !caps;

	if (input->StylusDown()) {
			for (i = 0 ; i < 12; i++) {
				if (Frog::Rect(x+11+(i*17),y+320-92+5,x+11+(i*17)+16,y+320-92+5+16).Contains(input->getCoordinates()))
					display->FillRect(x+0+11+(i*17),y+320-92+5,x+11+(i*17)+16,y+320-92+5+16,Color(64,64,64));
				if (i >= 10) continue;
				if (Frog::Rect(x+8+11+(i*17),y+320-92+5+17,x+8+11+(i*17)+16,y+320-92+5+17+16).Contains(input->getCoordinates()))
					display->FillRect(x+8+11+(i*17),y+320-92+5+17,x+8+11+(i*17)+16,y+320-92+5+17+16,Color(64,64,64));
				if (Frog::Rect(x+8+11+(i*17)+6,y+320-92+5+17+17,x+8+11+(i*17)+16+6,y+320-92+5+17+17+16).Contains(input->getCoordinates()))
					display->FillRect(x+8+11+(i*17)+6,y+320-92+5+17+17,x+8+11+(i*17)+16+6,y+320-92+5+17+16+17,Color(64,64,64));
				if (Frog::Rect(x+8+11+(i*17)+6+7,y+320-92+5+17+17+17,x+8+11+(i*17)+16+6+7,y+320-92+5+17+17+17+16).Contains(input->getCoordinates()))
					display->FillRect(x+8+11+(i*17)+6+7,y+320-92+5+17+17+17,x+8+11+(i*17)+16+6+7,y+320-92+5+17+17+16+17,Color(64,64,64));
										
			}
			if (Frog::Rect(x+189,y+250,x+189+47,y+250+16).Contains(input->getCoordinates()))
				display->FillRect(x+189,y+250,x+189+47,y+250+16,Color(64,64,64));
			if (Frog::Rect(x+188+7,y+250+17,x+188+7+28,y+250+17+16).Contains(input->getCoordinates()))
				display->FillRect(x+188+7,y+250+17,x+188+7+28,y+250+17+16,Color(64,64,64));				
			if (Frog::Rect(x+180-89,y+250+51,x+180-89+47,y+250+51+16).Contains(input->getCoordinates()))
				display->FillRect(x+180-89,y+250+51,x+180-89+47,y+250+51+16,Color(64,64,64));	
			if (Frog::Rect(x+189-186,y+250+17,x+189-186+21,y+250+17+16).Contains(input->getCoordinates()))
				display->FillRect(x+189-186,y+250+17,x+189-186+21,y+250+17+16,Color(64,64,64));
	}

	if (caps)
		display->FillRect(x+189-186,y+250+17,x+189-186+21,y+250+17+16,Color(64,64,64));


	if (caps)
		for (i = 0 ; i < 12; i++) {
			TCHAR character[2];
			_stprintf(character,TEXT("%c"),toprow2[i]);
			font16->DisplayText(display,x+0+15+(i*17),y+320-92+5,character,Color(0,0,0));
				if (i >= 10) continue;
			_stprintf(character,TEXT("%c"),middlerow2[i]);
			font16->DisplayText(display,x+8+15+(i*17),y+320-92+5+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),bottomrow2[i]);
			font16->DisplayText(display,x+8+15+(i*17)+6,y+320-92+5+17+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),lastrow2[i]);
			font16->DisplayText(display,x+8+15+(i*17)+6+7,y+320-92+5+17+17+17,character,Color(0,0,0));
		}
	else
		for (i = 0 ; i < 12; i++) {
			TCHAR character[2];
			_stprintf(character,TEXT("%c"),toprow[i]);
			font16->DisplayText(display,x+0+15+(i*17),y+320-92+5,character,Color(0,0,0));
			if (i >= 10) continue;
			_stprintf(character,TEXT("%c"),middlerow[i]);
			font16->DisplayText(display,x+8+15+(i*17),y+320-92+5+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),bottomrow[i]);
			font16->DisplayText(display,x+8+15+(i*17)+6,y+320-92+5+17+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),lastrow[i]);
			font16->DisplayText(display,x+8+15+(i*17)+6+7,y+320-92+5+17+17+17,character,Color(0,0,0));
		}

	if (input->StylusPressed()) {
		if (cursor != 9)
			if (!caps)
				for (i = 0 ; i < 12; i++) {
					if (Frog::Rect(x+0+11+(i*17),y+320-92+5,x+0+11+(i*17)+16,y+320-92+5+16).Contains(input->getCoordinates())) {
						name[++cursor] = toprow[i];		
					}
					if (i >= 10) continue;
					if (Frog::Rect(x+8+11+(i*17),y+320-92+5+17,x+8+11+(i*17)+16,y+320-92+5+17+16).Contains(input->getCoordinates()))
						name[++cursor] = middlerow[i];	
					if (Frog::Rect(x+8+11+(i*17)+6,y+320-92+5+17+17,x+8+11+(i*17)+16+6,y+320-92+5+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = bottomrow[i];	
					if (Frog::Rect(x+8+11+(i*17)+6+7,y+320-92+5+17+17+17,x+8+11+(i*17)+16+6+7,y+320-92+5+17+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = lastrow[i];	
				}
			else 
				for (i = 0 ; i < 12; i++) {
					if (Frog::Rect(x+0+11+(i*17),y+320-92+5,x+0+11+(i*17)+16,y+320-92+5+16).Contains(input->getCoordinates())) {
								name[++cursor] = toprow2[i];		
					}
					if (i >= 10) continue;
					if (Frog::Rect(x+8+11+(i*17),y+320-92+5+17,x+8+11+(i*17)+16,y+320-92+5+17+16).Contains(input->getCoordinates()))
						name[++cursor] = middlerow2[i];	
					if (Frog::Rect(x+8+11+(i*17)+6,y+320-92+5+17+17,x+8+11+(i*17)+16+6,y+320-92+5+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = bottomrow2[i];	
					if (Frog::Rect(x+8+11+(i*17)+6+7,y+320-92+5+17+17+17,x+8+11+(i*17)+16+6+7,y+320-92+5+17+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = lastrow2[i];	
				}
	
		if (cursor != -1)
		if (Frog::Rect(x+189,250,x+189+47,250+16).Contains(input->getCoordinates())) {
				name[cursor--] = 0;
				//cursor--;
		}
		//enter
			if (Frog::Rect(x+188+7,y+250+17,x+188+7+28,y+250+17+16).Contains(input->getCoordinates()))
				display->FillRect(x+188+7,y+250+17,x+188+7+28,y+250+17+16,Color(64,64,64));				
		if (cursor != 9)
			if (Frog::Rect(x+180-89,y+250+51,x+180-89+47,y+250+51+16).Contains(input->getCoordinates()))
				name[++cursor] = ' ';	

	}
	TCHAR character2[30];
	_stprintf(character2,TEXT("BACKSPACE"));
	font8->DisplayText(display,x+192,y+254,character2,Color(0,0,0));
	_stprintf(character2,TEXT("ENTER"));
	font8->DisplayText(display,x+192+7,y+254+17,character2,Color(0,0,0));
	_stprintf(character2,TEXT("CAPS"));
	font8->DisplayText(display,x+192-188,y+254+17,character2,Color(0,0,0));
	_stprintf(character2,TEXT("SPACE"));
	font8->DisplayText(display,x+192-89,y+254+51,character2,Color(0,0,0));
	font16->DisplayText(display,10,y+80,name,Color(0,0,0));
}

void Keyboard::Render(Display *display, Input *input, Font *font8, Font *font16) {
	int i;
	display->Blit(0,320-92,sip);
	TCHAR toprow[14], middlerow[12], bottomrow[12], lastrow[12];
	_stprintf(toprow,TEXT("1234567890-="));
	_stprintf(middlerow,TEXT("qwertyuiop"));
	_stprintf(bottomrow,TEXT("asdfghjkl;"));
	_stprintf(lastrow,TEXT("zxcvbnm,./"));
	TCHAR toprow2[14], middlerow2[12], bottomrow2[12], lastrow2[12];
	_stprintf(toprow2,TEXT("!@#$%%^&*()_+"));
	_stprintf(middlerow2,TEXT("QWERTYUIOP"));
	_stprintf(bottomrow2,TEXT("ASDFGHKJL:"));
	_stprintf(lastrow2,TEXT("ZXCVBNM<>?"));	

	if (input->StylusPressed())
		shift = false;
	if (input->StylusPressed() && Frog::Rect(189-186,250+17,189-186+21,250+17+16).Contains(input->getCoordinates()))
		caps = !caps;

	if (input->StylusDown()) {
			for (i = 0 ; i < 12; i++) {
				if (Frog::Rect(0+11+(i*17),320-92+5,0+11+(i*17)+16,320-92+5+16).Contains(input->getCoordinates()))
					display->FillRect(0+11+(i*17),320-92+5,0+11+(i*17)+16,320-92+5+16,Color(64,64,64));
				if (i >= 10) continue;
				if (Frog::Rect(8+11+(i*17),320-92+5+17,8+11+(i*17)+16,320-92+5+17+16).Contains(input->getCoordinates()))
					display->FillRect(8+11+(i*17),320-92+5+17,8+11+(i*17)+16,320-92+5+17+16,Color(64,64,64));
				if (Frog::Rect(8+11+(i*17)+6,320-92+5+17+17,8+11+(i*17)+16+6,320-92+5+17+17+16).Contains(input->getCoordinates()))
					display->FillRect(8+11+(i*17)+6,320-92+5+17+17,8+11+(i*17)+16+6,320-92+5+17+16+17,Color(64,64,64));
				if (Frog::Rect(8+11+(i*17)+6+7,320-92+5+17+17+17,8+11+(i*17)+16+6+7,320-92+5+17+17+17+16).Contains(input->getCoordinates()))
					display->FillRect(8+11+(i*17)+6+7,320-92+5+17+17+17,8+11+(i*17)+16+6+7,320-92+5+17+17+16+17,Color(64,64,64));
										
/*				_stprintf(character,TEXT("%c"),middlerow2[i]);
				font16->DisplayText(display,8+15+(i*17),320-92+5+17,character,Color(0,0,0));
				_stprintf(character,TEXT("%c"),bottomrow2[i]);
				font16->DisplayText(display,8+15+(i*17)+6,320-92+5+17+17,character,Color(0,0,0));
				_stprintf(character,TEXT("%c"),lastrow2[i]);
				font16->DisplayText(display,8+15+(i*17)+6+7,320-92+5+17+17+17,character,Color(0,0,0));
				*/
			}
			if (Frog::Rect(189,250,189+47,250+16).Contains(input->getCoordinates()))
				display->FillRect(189,250,189+47,250+16,Color(64,64,64));
			if (Frog::Rect(188+7,250+17,188+7+28,250+17+16).Contains(input->getCoordinates()))
				display->FillRect(188+7,250+17,188+7+28,250+17+16,Color(64,64,64));				
			if (Frog::Rect(180-89,250+51,180-89+47,250+51+16).Contains(input->getCoordinates()))
				display->FillRect(180-89,250+51,180-89+47,250+51+16,Color(64,64,64));	
			if (Frog::Rect(189-186,250+17,189-186+21,250+17+16).Contains(input->getCoordinates()))
				display->FillRect(189-186,250+17,189-186+21,250+17+16,Color(64,64,64));
	}

	if (caps)
		display->FillRect(189-186,250+17,189-186+21,250+17+16,Color(64,64,64));


	if (caps)
		for (i = 0 ; i < 12; i++) {
			TCHAR character[2];
			_stprintf(character,TEXT("%c"),toprow2[i]);
			font16->DisplayText(display,0+15+(i*17),320-92+5,character,Color(0,0,0));
				if (i >= 10) continue;
			_stprintf(character,TEXT("%c"),middlerow2[i]);
			font16->DisplayText(display,8+15+(i*17),320-92+5+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),bottomrow2[i]);
			font16->DisplayText(display,8+15+(i*17)+6,320-92+5+17+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),lastrow2[i]);
			font16->DisplayText(display,8+15+(i*17)+6+7,320-92+5+17+17+17,character,Color(0,0,0));
		}
	else
		for (i = 0 ; i < 12; i++) {
			TCHAR character[2];
			_stprintf(character,TEXT("%c"),toprow[i]);
			font16->DisplayText(display,0+15+(i*17),320-92+5,character,Color(0,0,0));
			if (i >= 10) continue;
			_stprintf(character,TEXT("%c"),middlerow[i]);
			font16->DisplayText(display,8+15+(i*17),320-92+5+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),bottomrow[i]);
			font16->DisplayText(display,8+15+(i*17)+6,320-92+5+17+17,character,Color(0,0,0));
			_stprintf(character,TEXT("%c"),lastrow[i]);
			font16->DisplayText(display,8+15+(i*17)+6+7,320-92+5+17+17+17,character,Color(0,0,0));
		}

	if (input->StylusPressed()) {
		if (cursor != 9)
			if (!caps)
				for (i = 0 ; i < 12; i++) {
					if (Frog::Rect(0+11+(i*17),320-92+5,0+11+(i*17)+16,320-92+5+16).Contains(input->getCoordinates())) {
								name[++cursor] = toprow[i];		
					}
					if (i >= 10) continue;
					if (Frog::Rect(8+11+(i*17),320-92+5+17,8+11+(i*17)+16,320-92+5+17+16).Contains(input->getCoordinates()))
						name[++cursor] = middlerow[i];	
					if (Frog::Rect(8+11+(i*17)+6,320-92+5+17+17,8+11+(i*17)+16+6,320-92+5+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = bottomrow[i];	
					if (Frog::Rect(8+11+(i*17)+6+7,320-92+5+17+17+17,8+11+(i*17)+16+6+7,320-92+5+17+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = lastrow[i];	
				}
			else 
				for (i = 0 ; i < 12; i++) {
					if (Frog::Rect(0+11+(i*17),320-92+5,0+11+(i*17)+16,320-92+5+16).Contains(input->getCoordinates())) {
								name[++cursor] = toprow2[i];		
					}
					if (i >= 10) continue;
					if (Frog::Rect(8+11+(i*17),320-92+5+17,8+11+(i*17)+16,320-92+5+17+16).Contains(input->getCoordinates()))
						name[++cursor] = middlerow2[i];	
					if (Frog::Rect(8+11+(i*17)+6,320-92+5+17+17,8+11+(i*17)+16+6,320-92+5+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = bottomrow2[i];	
					if (Frog::Rect(8+11+(i*17)+6+7,320-92+5+17+17+17,8+11+(i*17)+16+6+7,320-92+5+17+17+17+16).Contains(input->getCoordinates()))
						name[++cursor] = lastrow2[i];	
				}
	
		if (cursor != -1)
		if (Frog::Rect(189,250,189+47,250+16).Contains(input->getCoordinates())) {
				name[cursor--] = 0;
				//cursor--;
		}
		//enter
			if (Frog::Rect(188+7,250+17,188+7+28,250+17+16).Contains(input->getCoordinates()))
				display->FillRect(188+7,250+17,188+7+28,250+17+16,Color(64,64,64));				
		if (cursor != 9)
			if (Frog::Rect(180-89,250+51,180-89+47,250+51+16).Contains(input->getCoordinates()))
				name[++cursor] = ' ';	

	}
	TCHAR character2[30];
	_stprintf(character2,TEXT("debug %d %d"),input->getCoordinates().x, input->getCoordinates().y);
//	font16->DisplayText(display,100,300,character2,Color(0,0,0));
	_stprintf(character2,TEXT("BACKSPACE"));
	font8->DisplayText(display,192,254,character2,Color(0,0,0));
	_stprintf(character2,TEXT("ENTER"));
	font8->DisplayText(display,192+7,254+17,character2,Color(0,0,0));
	_stprintf(character2,TEXT("CAPS"));
	font8->DisplayText(display,192-188,254+17,character2,Color(0,0,0));
	_stprintf(character2,TEXT("SPACE"));
	font8->DisplayText(display,192-89,254+51,character2,Color(0,0,0));
	font16->DisplayText(display,10,80,name,Color(0,0,0));
}