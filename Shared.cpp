#include "Shared.h"
#include "resource.h"
#include <PocketFrog/PocketFrog.h>
using namespace Frog;

Shared::Shared(void)
{
	input = new Input();
}

Shared::~Shared(void)
{
	input->~Input();
}

void Shared::Init(Display *display,PHAL::KeyList *f_keys) {
	int i,j;
	whatLength = 0;
	delta_time = 0;
	newgame = false;
	keyboard = new Keyboard();
	keyboard->Init(display);
	trial = true;
	transitiontype = FADE;
	bookmanfont = new Font(LoadImage(display, IDB_BOOKMANFONT));
	bigfont = new Font(LoadImage(display, IDB_BIGFONT));
	font = new Font(LoadImage(display, IDB_TINYFONT));
	font8 = new Font(LoadImage(display, IDB_FONT8));
	font16 = new Font(LoadImage(display, IDB_FONT16));
	centaurfont = new Font(LoadImage(display, IDB_CENTAURFONT));
	file = new File();
	file->Init();
	m_keys = f_keys;
	sound = true;
	m_speaker.open(22050, 16, true, 2, 32);
	m_sounds[0].load(_Module.GetModuleInstance(), IDR_CLICK);
	m_sounds[1].load(_Module.GetModuleInstance(), IDR_MENUSLIDE);
	m_sounds[2].load(_Module.GetModuleInstance(), IDR_ROUNDOVER);
	m_sounds[3].load(_Module.GetModuleInstance(), IDR_CORRECT);
	m_sounds[4].load(_Module.GetModuleInstance(), IDR_INTERFACE);

	// initial game state
	gamestate = MAINMENU;
	
	if (trial) {
		if (!file->OpenForRead(TEXT("moredata.dat"))) {
			trialtime = 0;
		} else {
			fread(&trialtime, 8, 1, file->GetFile());
			file->Close();
		}
	}

	if (!file->OpenForRead(TEXT("hiscores.dat"))) {
		for (j =0; j < 3; j++) {
		// if theres no highscores data
						longestword[j] =0;
						highestscoringword[j] = 0;
						accumulatedpoints[j] = 0;
						totalwordsfound[j] =0 ;
						totallettersfound[j] = 0;
						for (i =0 ;i < 10; i++) {
							longestwordtext[j][i] = 0;
							highestscoringwordtext[j][i] = 0;
						}
						for (i =0; i < 5; i++) {
							highscores[j][i] = 0;
							highpercentscores[j][i] = 0;
						}
						totalwords[j] = 1;
		}
	} else {
		for (j =0; j < 3; j++) {
			fread(&longestword[j], 4, 1, file->GetFile());
			fread(&highestscoringword[j], 4, 1, file->GetFile());
			fread(&accumulatedpoints[j], 4, 1, file->GetFile());
			fread(&totalwordsfound[j], 4, 1, file->GetFile());
			fread(&totallettersfound[j], 4, 1, file->GetFile());
			for (i =0 ;i < 10; i++) {
				fread(&longestwordtext[j][i], 1, 1, file->GetFile());
				fread(&highestscoringwordtext[j][i], 1, 1, file->GetFile());
			}
			for (i =0; i < 5; i++) {
				fread(&highscores[j][i], 4, 1, file->GetFile());
				fread(&highpercentscores[j][i], 4, 1, file->GetFile());
			}
			fread(&totalwords[j], 4, 1, file->GetFile());
			
		}
		file->Close();
	}
}
void Shared::End() {
	int i,j;
	if (trial) {
		file->OpenForWrite(TEXT("moredata.dat"));
		fwrite(&trialtime, 8, 1, file->GetFile());
		file->Close();
	}
	file->OpenForWrite(TEXT("hiscores.dat"));
	for (j =0; j < 3; j++) {
		fwrite(&longestword[j], 4, 1, file->GetFile());
		fwrite(&highestscoringword[j], 4, 1, file->GetFile());
		fwrite(&accumulatedpoints[j], 4, 1, file->GetFile());
		fwrite(&totalwordsfound[j], 4, 1, file->GetFile());
		fwrite(&totallettersfound[j], 4, 1, file->GetFile());
		for (i =0 ;i < 10; i++) {
			fwrite(&longestwordtext[j][i], 1, 1, file->GetFile());
			fwrite(&highestscoringwordtext[j][i], 1, 1, file->GetFile());
		}
		for (i =0; i < 5; i++) {
			fwrite(&highscores[j][i], 4, 1, file->GetFile());
			fwrite(&highpercentscores[j][i], 4, 1, file->GetFile());
		}
		fwrite(&totalwords[j], 4, 1, file->GetFile());
	}
	file->Close();
}

void Shared::RenderKeyboard(Display *display) {
	keyboard->Render(display, input, font8, font16);
}

void Shared::AnimateKeyboard(Display *display) {
	keyboard->Animate(display, input, font8, font16);
}