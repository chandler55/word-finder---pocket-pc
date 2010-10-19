#include "Joggle.h"
//#include <algorithm>
struct eqstr
{
  bool operator()(const TCHAR* s1, const TCHAR* s2) const
  {
    return _tcscmp(s1, s2) == 0;
  }
};
int my_sort(const void *a, const void *b)
{
  return (_tcscmp(*(TCHAR **)a, *(TCHAR **)b));
}

inline int getLetter() {
	if (!(rand() % 5 == 0))
		return rand() % 26 + 97;
	else
		switch (rand() % 5) {
			case 0:
				return 97;
				break;
			case 1:
				return 101;
				break;
			case 2:
				return 100;
				break;
			case 3:
				return 117;
				break;
			case 4: 
				return 105;
				break;
	}
	return 97;
}
void Joggle::try_print(TCHAR *word)
{
  int i;


  for (i = 0; i < num_found; i++)
    if (!_tcscmp(word, found[i]))
      return;

  found[num_found++] = _tcsdup(word);

  //printf("bog %s\n", word);
  //words++;
}


void Joggle::output(int depth)
{


  static int bot, top, cen, res;

  choices[depth] = '\0';

  bot = 0;
  top = num_words -1;

  for(;;) {
    cen = (bot+top)/2;
	
    res = _tcscmp(choices, dict[cen]);
    if (!res) {
      try_print(choices);
      return;
    } else if (bot >= top) {
      return;
    } else if (res > 0) {
      bot = ++cen;
    } else
      top = --cen;
  }
 }
void Joggle::FinishAnimation() {
		animating= 0;
		boardy = -500;
}

void Joggle::StartAnimation() {
	int i;
	if (shared->newgame) {
		newHighscore = newPercentScore = 0;
		
		what_round = 0;
		state = INITIATEGAME;
		shared->newgame = false;
		totalpoints = 0;
		roundpoints[0] = 0;
		roundpoints[1] = 0;
		roundpoints[2] = 0;

		for (i = 0; i < 10; i++)
		{
			longestwordtext[i] = highestscoringwordtext[i] = 0;
		}
		percentFound = 0; highestscoringword = 0; longestword = 0;
		wordsFound = wordsTotal = 0;
		//InitiateGame();
	}
		animating= 2;

		boardy = 0.5 * -500;

		animate = 0.5;
}

void Joggle::dfs(int x, int y, int depth)
{
  int i, j;
	
	if (depth >= 4) {
		#ifdef _UNICODE
		char s[10];
		wcstombs(s,choices,10);
		if (!test[s[0] - 'a'][s[1] - 'a'][s[2] - 'a'][s[3] - 'a']) {
		#else
		if (!test[choices[0] - 'a'][choices[1] - 'a'][choices[2] - 'a'][choices[3] - 'a']) {
		#endif
	  return;
	  }
	}
 
  if (x < 0 || y < 0 || x >= BOX_SIZE || y >= BOX_SIZE)
    return;
  if (!round[what_round][x][y])
	  return;
  if (graph[x][y] == SEEN) {
    return;
  }

  graph[x][y] = SEEN;

  choices[depth++] = grid[x][y];
  if (choices[depth-1] == 'q')
	  choices[depth++] = 'u';


  if (depth >= MIN_WORD_LEN) {
	 /* // Q needs a U in word finder/worggle
		  if (choices[0] == 'q' && choices[0+1] != 'u') {
			  for (i = depth+1; i != 1; i--)
				choices[i] = choices[i-1];


			  choices[0+1] = 'u';
			 depth++;
		  }*/
	  

    output(depth);
  }

  if (depth < MAX_WORD_LEN) {
    for (j = -1; j <= 1; j++)
      for(i = -1; i <= 1; i++)
	if ((i || j))
	  dfs(x+i, y+j, depth);
  }

  graph[x][y] = NOT_SEEN;
}


Joggle::Joggle(void)
{
}

Joggle::~Joggle(void)
{
}

void Joggle::GameEnd() {
}
bool Joggle::MoveClock() {
	clockmover+= shared->delta_time;
	if (clockmover > 1) {
		if (seconds > 0 )
			seconds--;
		else
			if (minutes >0) {
				minutes--;
				seconds = 59;
			}
		clockmover = 0;
	}
	if (minutes == 0 && seconds == 0)
		return true;
	return false;
}
void Joggle::InitiateGame() {
	int i,j;
	showAnswerTime= 0;
	showAnswerY =0;
	showAnswerX=0;
	num_found=0;
	for (i = 0; i < FOUND_SIZE;i++)
		founded[i] = false;

	clockmover= 0;
	nextroundY= 0;
	fliproundover=0;

	flipnextround = false;
	switch (shared->whatLength) {
		case 0:
			minutes = 1; 
			break;
		case 1:
			minutes = 2; 
			break;
		case 2:
			minutes = 3; 
			break;
	}
	
	seconds = 0;
	roundover= 0;
	last_picked.x = 0;
	last_picked.y = 0;
	
	player_num_found = 0;
	for (i = 0; i < 36; i++)
		undo[i].Set(-2,-2);

	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++) {
			picked[i][j] = 0;
			gridarrows[i][j] = 0;
			pointsgrid[i][j] = 1;
		}
	switch (what_round) {
		case 0:
			break;
		case 1:
			pointsgrid[0][5] = 3;
			pointsgrid[0][0] = 2;
			pointsgrid[5][0] = 3;
			pointsgrid[5][5] = 2;
			break;
		case 2:
			pointsgrid[0][0] = 3;
			pointsgrid[0][3] = 2;
			pointsgrid[5][5] = 3;
			pointsgrid[5][2] = 2;
			break;
	}

	for (i = 0; i < 1000; i++)
		points[i] = 0;

	words =0 ;
	for (i = 0; i < 1000; i++) {
		found[i] = 0;
		playerfound[i] =0;
		realpoints[i]=0;
	}
	for (i = 0; i < 36; i++)
		name[i] = 0;

	cursor = -1;
	//animate = 0;
	
		  for (j = 0; j < BOX_SIZE; j++) {
		for (i = 0; i < BOX_SIZE; i++) {
		  graph[i][j] = NOT_SEEN;
		}
	  }
		  	// generate game board
	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++)
			grid[i][j] = getLetter();

	  for (i = 0; i < BOX_SIZE; i++)
		for (j = 0; j < BOX_SIZE; j++)
		  dfs(i, j, 0);

  if (num_found < 10) {
		 InitiateGame();
		 return;
	  }

	qsort(found, num_found, sizeof(char *), my_sort);
	for (i = 0; i < num_found; i++)
		for (j = 0; j < _tcslen(found[i]); j++)
			if (found[i][j] > 90)
				found[i][j] -= 32;
	shared->totalwords[shared->whatLength] += num_found;
}
void Joggle::GameInit(Display *display) {
	int i,j;
	init = 0;
	newHighscore = newPercentScore = 0;
	soundOn = false;
	smallmenuOn= false;
	smallmenuY = -200;
	shared->newgame = true;
	boardy = -500;
	num_bigwords=num_words= num_found=0;
	temp = new Surface(196,196);
	temp->SetColorMask(Color(255,0,255));
	
	b_clear = new Button();
	b_enter = new Button();
	b_menu = new Button();
	b_nextround= new Button();
	b_backtomainmenu = new Button();
	b_nextround->Init(ANIMATION_MOVEUP, display, IDB_RGBNEXTROUNDDOWN, IDB_RGBNEXTROUND, IDB_ANEXTROUNDDOWN, IDB_ANEXTROUND,178,280, 60,29);
	b_menu->Init(ANIMATION_MOVELEFT, display, IDB_JOGGLEMENUUP, IDB_JOGGLEMENU,0,0,54,19);
	b_enter->Init(ANIMATION_MOVERIGHT, display, IDB_JOGGLEENTERUP,IDB_JOGGLEENTER, 167,219,54,19);
	b_clear->Init(ANIMATION_MOVELEFT, display,IDB_JOGGLECLEARUP, IDB_JOGGLECLEAR,22,219,54,19);
	b_backtomainmenu->Init(ANIMATION_MOVEUP, display, IDB_RGBBACKTOMAINMENUDOWN, IDB_RGBBACKTOMAINMENU, IDB_ABACKTOMAINMENUDOWN, IDB_ABACKTOMAINMENU,130,280,107,29);
	scrollbar = new Scrollbar();
	i_finalresults = LoadImage(display, IDB_FINALRESULTS);
	scrollbar->Init(display,73,130,9,243);
	resultsscrollbar = new Scrollbar();
	resultsscrollbar->Init(display, 96,130,54,175);
	b_tofinalresults = new Button();
	b_tofinalresults->Init(ANIMATION_MOVEUP, display, IDB_RGBTOFINALRESULTSDOWN, IDB_RGBTOFINALRESULTS, IDB_ATOFINALRESULTSDOWN, IDB_ATOFINALRESULTS,160,280, 79,29);
	i_round1 = LoadImage(display, IDB_JOGGLEROUND1);
	i_round2 = LoadImage(display, IDB_JOGGLEROUND2);
	i_round4 = LoadImage(display, IDB_JOGGLEROUND4);
	i_asmallmenu = LoadImage(display, IDB_ASMALLMENU);
	i_smallmenu2= LoadImage(display, IDB_SMALLMENU2);
	i_rgbsmallmenu = LoadImage(display, IDB_RGBSMALLMENU);
	i_resultsHeader[0] = LoadImage(display, IDB_RESULTS1);
	i_resultsHeader[1] = LoadImage(display, IDB_RESULTS2);
	i_resultsHeader[2] = LoadImage(display, IDB_RESULTS3);
	i_resultsHeader[3] = LoadImage(display, IDB_RESULTS4);
	i_smallmenu = LoadImage(display, IDB_SMALLMENU);
	i_aarrows= LoadImage(display, IDB_ALPHAARROWS);
	i_rgbarrows = LoadImage(display, IDB_RGBARROWS);
	i_aroundover = LoadImage(display, IDB_AROUNDOVER);
	i_rgbroundover = LoadImage(display, IDB_RGBROUNDOVER);
	i_round1->SetColorMask(Color(255,0,255));
	i_round2->SetColorMask(Color(255,0,255));
	i_round4->SetColorMask(Color(255,0,255));
	i_aroundheader[0] = LoadImage(display, IDB_AROUND1);
	i_aroundheader[1] = LoadImage(display, IDB_AROUND2);
	i_aroundheader[2] = LoadImage(display, IDB_AROUND3);
	i_rgbroundheader[0] = LoadImage(display, IDB_RGBROUND1);
	i_rgbroundheader[1] = LoadImage(display, IDB_RGBROUND2);
	i_rgbroundheader[2] = LoadImage(display, IDB_RGBROUND3);
	i_background = LoadImage(display,IDB_JOGGLEBACKGROUND);
	i_background2 = LoadImage(display,IDB_JOGGLEBACKGROUND2);
	i_results= LoadImage(display, IDB_RESULTS);
	i_doubleword = LoadImage(display, IDB_DOUBLEWORD);
	i_doubleword->SetColorMask(Color(255,0,255));
	i_letterpicked = LoadImage(display, IDB_LETTERPICKED);
	i_letterpicked->SetColorMask(Color(255,0,255));
	i_tripleword = LoadImage(display, IDB_TRIPLEWORD);
	i_tripleword->SetColorMask(Color(255,0,255));
	i_square = LoadImage(display, IDB_JOGGLESQUARE);
	i_letters[0] = LoadImage(display, IDB_JOGGLEA);
	i_letters[1] = LoadImage(display, IDB_JOGGLEB);
	i_letters[2] = LoadImage(display, IDB_JOGGLEC);
	i_letters[3] = LoadImage(display, IDB_JOGGLED);
	i_letters[4] = LoadImage(display, IDB_JOGGLEE);
	i_letters[5] = LoadImage(display, IDB_JOGGLEF);
	i_letters[6] = LoadImage(display, IDB_JOGGLEG);
	i_letters[7] = LoadImage(display, IDB_JOGGLEH);
	i_letters[8] = LoadImage(display, IDB_JOGGLEI);
	i_letters[9] = LoadImage(display, IDB_JOGGLEJ);
	i_letters[10] = LoadImage(display, IDB_JOGGLEK);
	i_letters[11] = LoadImage(display, IDB_JOGGLEL);
	i_letters[12] = LoadImage(display, IDB_JOGGLEM);
	i_letters[13] = LoadImage(display, IDB_JOGGLEN);
	i_letters[14] = LoadImage(display, IDB_JOGGLEO);
	i_letters[15] = LoadImage(display, IDB_JOGGLEP);
	i_letters[16] = LoadImage(display, IDB_JOGGLEQU);
	i_letters[17] = LoadImage(display, IDB_JOGGLER);
	i_letters[18] = LoadImage(display, IDB_JOGGLES);
	i_letters[19] = LoadImage(display, IDB_JOGGLET);
	i_letters[20] = LoadImage(display, IDB_JOGGLEU);
	i_letters[21] = LoadImage(display, IDB_JOGGLEV);
	i_letters[22] = LoadImage(display, IDB_JOGGLEW);
	i_letters[23] = LoadImage(display, IDB_JOGGLEX);
	i_letters[24] = LoadImage(display, IDB_JOGGLEY);
	i_letters[25] = LoadImage(display, IDB_JOGGLEZ);
	i_smallletters[0] = LoadImage(display, IDB_ASMALL);
	i_smallletters[1] = LoadImage(display, IDB_BSMALL);
	i_smallletters[2] = LoadImage(display, IDB_CSMALL);
	i_smallletters[3] = LoadImage(display, IDB_DSMALL);
	i_smallletters[4] = LoadImage(display, IDB_ESMALL);
	i_smallletters[5] = LoadImage(display, IDB_FSMALL);
	i_smallletters[6] = LoadImage(display, IDB_GSMALL);
	i_smallletters[7] = LoadImage(display, IDB_HSMALL);
	i_smallletters[8] = LoadImage(display, IDB_ISMALL);
	i_smallletters[9] = LoadImage(display, IDB_JSMALL);
	i_smallletters[10] = LoadImage(display, IDB_KSMALL);
	i_smallletters[11] = LoadImage(display, IDB_LSMALL);
	i_smallletters[12] = LoadImage(display, IDB_MSMALL);
	i_smallletters[13] = LoadImage(display, IDB_NSMALL);
	i_smallletters[14] = LoadImage(display, IDB_OSMALL);
	i_smallletters[15] = LoadImage(display, IDB_PSMALL);
	i_smallletters[16] = LoadImage(display, IDB_QUSMALL);
	i_smallletters[17] = LoadImage(display, IDB_RSMALL);
	i_smallletters[18] = LoadImage(display, IDB_SSMALL);
	i_smallletters[19] = LoadImage(display, IDB_TSMALL);
	i_smallletters[20] = LoadImage(display, IDB_USMALL);
	i_smallletters[21] = LoadImage(display, IDB_VSMALL);
	i_smallletters[22] = LoadImage(display, IDB_WSMALL);
	i_smallletters[23] = LoadImage(display, IDB_XSMALL);
	i_smallletters[24] = LoadImage(display, IDB_YSMALL);
	i_smallletters[25] = LoadImage(display, IDB_ZSMALL);
	i_highlighted[0] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDA);
	i_highlighted[1] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDB);
	i_highlighted[2] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDC);
	i_highlighted[3] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDD);
	i_highlighted[4] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDE);
	i_highlighted[5] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDF);
	i_highlighted[6] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDG);
	i_highlighted[7] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDH);
	i_highlighted[8] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDI);
	i_highlighted[9] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDJ);
	i_highlighted[10] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDK);
	i_highlighted[11] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDL);
	i_highlighted[12] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDM);
	i_highlighted[13] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDN);
	i_highlighted[14] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDO);
	i_highlighted[15] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDP);
	i_highlighted[16] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDQU);
	i_highlighted[17] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDR);
	i_highlighted[18] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDS);
	i_highlighted[19] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDT);
	i_highlighted[20] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDU);
	i_highlighted[21] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDV);
	i_highlighted[22] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDW);
	i_highlighted[23] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDX);
	i_highlighted[24] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDY);
	i_highlighted[25] = LoadImage(display, IDB_JOGGLEHIGHLIGHTEDZ);

	for (i = 0; i < 8; i++) {
		arrows[i].top = 0;
		arrows[i].bottom = 19;
	}

	roundoverY= 0;
	arrows[0].left = 0;
	arrows[0].right = 19;
	arrows[1].left = 21;
	arrows[1].right = 21+12;
	arrows[2].left = 35;
	arrows[2].right = 55;
	arrows[3].left = 56;
	arrows[3].right = 56+16;
	arrows[4].left = 73;
	arrows[4].right = 73+16;
	arrows[5].left = 90;
	arrows[5].right = 90+16;
	arrows[6].left = 107;
	arrows[6].right = 107+16;
	arrows[7].left = 124;
	arrows[7].right = 136;

	what_round = 0;	

	// set up the game boards
	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++) {
			if (i > 0 && i < 5 && j > 0 && j < 5)
				round[0][i][j] = 1;
			else
				round[0][i][j] = 0;
				round[1][i][j] = 1;
				round[2][i][j] = 1;
		}

	round[1][2][2] = 0;
	round[1][2][3] = 0;
	round[1][3][2] = 0;
	round[1][3][3] = 0;

	round[2][4][0] = 0;
	round[2][5][0] = 0;
	round[2][4][1] = 0;
	round[2][5][1] = 0;

	round[2][0][4] = 0;
	round[2][0][5] = 0;
	round[2][1][4] = 0;
	round[2][1][5] = 0;

	nextState= JOGGLE;

	int k, l;
	
	TCHAR buf[128];
	char s[128];
	for (i =0; i < 26; i++)
	for (j = 0; j < 26; j++)
	for (int m = 0; m < 26; m++)
		for (int p = 0; p < 26; p++)
		test[i][j][m][p] = 0;
	shared->file->ErrorWrite("loading dictionary");
	shared->file->OpenForRead(TEXT("newdict.txt"));
	/*while (_fgetts(buf, 128, shared->file->GetFile())) {
		buf[_tcslen(buf)-1-1] = '\0';
		//for (i = 0; i < _tcslen(buf); i++) 
		  //buf[i] = tolower(buf[i]);
		if (_tcslen(buf) >= 3) {
			_stprintf(dict[num_words++],buf);
			 //dict[num_words++] = _tcsdup(buf);
			#ifdef _UNICODE
			wcstombs(s,buf,128);
			 test[s[0] - 'a'][s[1] - 'a'][s[2] - 'a'][s[3] - 'a'] = 1;
			#else
			  test[buf[0] - 'a'][buf[1] - 'a'][buf[2] - 'a'][buf[3] - 'a'] = 1;
			#endif
			
		}
	}*/
	num_words= 80625;
	
	shared->file->OpenForRead(TEXT("data.dat"));
	fread(&dict,sizeof(dict),1,shared->file->GetFile());
	shared->file->Close();
	
	for ( i= 0; i < num_words;i++)
		if (_tcslen(dict[i]) >3) {
			#ifdef _UNICODE
			wcstombs(s,dict[i],128);
			 test[s[0] - 'a'][s[1] - 'a'][s[2] - 'a'][s[3] - 'a'] = 1;
			#else
			  test[dict[i][0] - 'a'][dict[i][1] - 'a'][dict[i][2] - 'a'][dict[i][3] - 'a'] = 1;
			#endif
		}

	shared->file->ErrorWrite("loaded dictionary");
	shared->file->Close();
	totalpoints = 0;
	roundpoints[0] = 0;
	roundpoints[1] = 0;
	roundpoints[2] = 0;
	//InitiateGame();
	state = INITIATEGAME;
	for (i = 0; i < 10; i++)
	{
		longestwordtext[i] = highestscoringwordtext[i] = 0;
	}
	percentFound = 0; highestscoringword = 0; longestword = 0;
	wordsFound = wordsTotal = 0;
}
void Joggle::Animate() {
bool finishedanimating = false;
	if ((boardy >= 0 && animating == 2) || (boardy <= -320 && animating == 1))
		finishedanimating= true;

		if (animate > 0 || !finishedanimating) {
			animate -= shared->delta_time;
			if (animating == 2) {
				boardy += shared->delta_time * 500;
				if (boardy > 0)
					boardy = 0;
			}
			else if (animating == 1)
			{
				boardy -= shared->delta_time * 500;
				if (boardy < -320)
					boardy = -320;
			}
		}
		else {	
			animate = 0;	
		}
}
void Joggle::GameLoop(Frog::Display *display) {
	// Gameplay
	int i,j;

	// Gameplay
	if (animate) {
		Animate();
	} else {
		if (nextState != JOGGLE) {
			shared->gamestate = nextState;
			nextState= JOGGLE;
		}
	}

	switch (state) {
		case INITIATEGAME:
			if (init == 1) {
				InitiateGame();
				init = 0;
				state = JOGGLEGAME;
			} else {
				init++;
			}
			Render(display);
			break;
		case SMALLMENU:
			// input
			if (shared->input->StylusUp()) {
				if (Frog::Rect(32,128,32+173,128+21).Contains(shared->input->getCoordinates())) {
					smallmenuOn = false;
					state = JOGGLEGAME;
					shared->PlaySoundA(4);
				}
				else if (Frog::Rect(35,172,35+170,172+20).Contains(shared->input->getCoordinates())) {
					smallmenuOn = false;
					state = JOGGLEGAME;
					shared->PlaySoundA(4);
					shared->newgame = true;
					nextState = MAINMENU;

				}
				else if (Frog::Rect(35,146,35+170,146+20).Contains(shared->input->getCoordinates())) {
					shared->sound = !shared->sound;
					shared->PlaySoundA(4);
				}
			}
			Render(display);
			
			break;
		case JOGGLEGAME:

			if (shared->trial && shared->trialtime > 1200) {
				Render(display);
				TCHAR paused2[200];
				_stprintf(paused2, TEXT("TRIAL PERIOD ENDED!"));
				shared->bigfont->DisplayText(display,10,20,paused2,Color(255,0,255));
				if (shared->input->StylusUp()) {
					if (b_menu->StylusUp(shared->input->getCoordinates())) {
						smallmenuOn = true;
						state = SMALLMENU;
						shared->PlaySoundA(4);
					}
				}
			} else {
				if (shared->trial)
					shared->trialtime += shared->delta_time;
				showAnswerY -= shared->delta_time * 40;
				showAnswerTime -= shared->delta_time * 255;
				if (showAnswerY < -10)
					showAnswerY = -10;
				if (showAnswerTime < 0)
					showAnswerTime = 0;
			
			Input();

			for (i = 0; i < 6; i++)
				for (j = 0; j < 6; j++) {
					gridarrows[i][j] = 0;
				}
			for (i = 1; i < 34+2; i++) {
				if (name[i] == 'Q'){
					gridarrows[undo[i-1].x][undo[i-1].y] = ConnectedHow(undo[i-1].x, undo[i-1].y, undo[i+1].x, undo[i+1].y);
					i++;
				}
				else
					gridarrows[undo[i-1].x][undo[i-1].y] = ConnectedHow(undo[i-1].x, undo[i-1].y, undo[i].x, undo[i].y);
			}

			scrollbar->GameLoop(shared);
			for (i = 0; i < 6; i++)
				for (j = 0; j < 6; j++)
					if (picked[i][j] > 0) {
						if (picked[i][j] < 255)
							picked[i][j] += shared->delta_time * 255 * 2;
						else
							picked[i][j] = 255;
						if (picked[i][j] > 255)
							picked[i][j] = 255;
					}

			Render(display);
			if (MoveClock())  {
				CalculateResults();
			}
			}
			break;
		case READYSETGO:
			Render(display);
			break;
		case RESULTS:
			if (shared->input->StylusDown()) {
				b_nextround->StylusDown(shared->input->getCoordinates());
				b_tofinalresults->StylusDown(shared->input->getCoordinates());
			}
			if (shared->input->StylusUp()) {
				if (what_round < 2) {
					if (b_nextround->StylusUp(shared->input->getCoordinates()))
					{
						what_round++;
						wordsFound += player_num_found;
						wordsTotal += num_found;
						shared->PlaySoundA(4);
						state = INITIATEGAME;
						
						//InitiateGame();
					}
				}
				else
					if (b_tofinalresults->StylusUp(shared->input->getCoordinates())) {
						percentFound = ((float)wordsFound / (float)wordsTotal) * 100;

						int i,j;
						for (i = 0; i < 5; i++) {
							if (totalpoints > shared->highscores[shared->whatLength][i]) {
								newHighscore = i+1;
								for (j = 4; j != i; j--) {
									shared->highscores[shared->whatLength][j] = shared->highscores[shared->whatLength][j-1];
									
								}
								shared->highscores[shared->whatLength][i] = totalpoints;
								break;
							}
						}
						for (i = 0; i < 5; i++) {
							if (percentFound > shared->highpercentscores[shared->whatLength][i]) {
								newPercentScore = i+1;
								for (j = 4; j != i; j--) {
									shared->highpercentscores[shared->whatLength][j] = shared->highpercentscores[shared->whatLength][j-1];
			
								}
								shared->highpercentscores[shared->whatLength][i] = percentFound;
								break;
							}
						}
						state = FINALRESULTS;
						shared->PlaySoundA(4);
					}
			}
			if (flipnextround) {
				nextroundY += shared->delta_time*15;
				if (nextroundY > 5) {
					nextroundY = 5;
					flipnextround = !flipnextround;
				}
			} else {
				nextroundY -= shared->delta_time*15;
				if (nextroundY < 0 ) {
					nextroundY = 0;
					flipnextround = !flipnextround;
				}
			}

			resultsscrollbar->GameLoop(shared);
			Render(display);
			break;
		case FINALRESULTS:
			if (shared->input->StylusDown()) {
				b_backtomainmenu->StylusDown(shared->input->getCoordinates());
			}
			if (shared->input->StylusUp()) {
				if (b_backtomainmenu->StylusUp(shared->input->getCoordinates())) {
					nextState = MAINMENU;
					what_round = 0;
					shared->newgame= true;
					shared->PlaySoundA(4);
					//InitiateGame();
				}
			}
			if (flipnextround) {
				nextroundY += shared->delta_time*15;
				if (nextroundY > 5) {
					nextroundY = 5;
					flipnextround = !flipnextround;
				}
			} else {
				nextroundY -= shared->delta_time*15;
				if (nextroundY < 0 ) {
					nextroundY = 0;
					flipnextround = !flipnextround;
				}
			}
			Render(display);
			break;
		case ROUNDOVER:
			if (roundover > 3) {
				if (what_round <= 2) {
					state = RESULTS;
					scrollbar->ResetScrollY();
				}
				else 
					state = FINALRESULTS;
				roundover = 0;
			}
			roundover += shared->delta_time;
			
			if (fliproundover ==0 || fliproundover==2) {
				if (roundoverY >= 117 && fliproundover ==0)
					fliproundover++;
				else if (roundoverY >= 100 && fliproundover ==2)
					roundoverY = roundoverY;
				else if (fliproundover == 0)
					roundoverY += shared->delta_time * 500;
				else
					roundoverY += shared->delta_time * 100;
			} else if (fliproundover == 1){
				if (roundoverY <= 80) {
					fliproundover = 2;
				}
				else		
					roundoverY -= shared->delta_time * 200;
			}
			Render(display);
			break;
	}
	
}

void Joggle::CalculateResults() {
	int i,j;
	for (i = 0; i < num_found; i++)
					{
						bool foundedThis = false;
						for (j = 0; j < player_num_found;j++)
						{
							if (_tcscmp(found[i],playerfound[j]) == 0)
							{
								founded[i] = true;
								realpoints[i] = _tcsdup(points[j]);
								foundedThis= true;	
							}
						}
						if (!foundedThis)
						{
							founded[i] = false;
						}
					}
					shared->PlaySoundA(2);
					state = ROUNDOVER;
}
char Joggle::ConnectedHow(char a, char b, char c, char d) {
	if (a-c == 1 && b-d == 1)
		return 1;
	else if (a-c == 0 && b-d == 1) 
		return 2;
	else if (a-c == -1 && b-d == 1)
		return 3;
	else if (a-c == 1 && b-d == 0)
		return 4;
	else if (a-c == -1 && b-d == 0)
		return 5;
	else if (a-c == 1 && b-d == -1)
		return 6;
	else if (a-c == 0 && b-d == -1)
		return 7;
	else if (a-c == -1 && b-d == -1)
		return 8;
	else
		return 0;
}

void Joggle::Render(Frog::Display *display) {
	int i,j;
	switch (state) {
		case INITIATEGAME:
			//display->SetBlending(128);
	int i,j;
	display->Blit(0,0,i_background);

	
//scrollbar->Render(display,shared, playerfound, player_num_found, points);
	
	b_menu->Render(display,boardy);
	b_enter->Render(display,boardy);
	b_clear->Render(display,boardy);
	TCHAR time[10], pointstext[10];
	//_stprintf(time, TEXT("%d:%02d"), minutes, seconds);
	//_stprintf(pointstext, TEXT("%d"), totalpoints);
	shared->bigfont->DisplayText(display,153,246,TEXT("TIME LEFT:"),Color(0,0,0));
	//shared->bigfont->DisplayText(display,174,260,time,Color(0,0,0));
	shared->bigfont->DisplayText(display,162,276,TEXT("POINTS:"),Color(0,0,0));
	//shared->bigfont->DisplayText(display,174,292,pointstext,Color(0,0,0));
	//shared->centaurfont->DisplayText(display,120-(_tcslen(name) * 5),220,name,Color(0,0,0));
		//display->SetBlending(255);
			shared->font->DisplayText(display,80,150,TEXT("GENERATING BOARD"),Color(0,0,0));
			break;
		case SMALLMENU:
			RenderGame(display);
			display->SetBlending(128);
			display->Clear(Color(0,0,0));
			display->SetBlending(255);
			display->Blit(26,120,i_rgbsmallmenu, i_asmallmenu);
			if (shared->sound)
				display->Blit(26+9,120+9,i_smallmenu2);
			else
				display->Blit(26+9,120+9,i_smallmenu);
			break;
		case JOGGLEGAME:
			RenderGame(display);
			break;
		case RESULTS:
			RenderResults(display);
			display->Blit(0,0,i_resultsHeader[what_round]);
			if (what_round < 2)
				b_nextround->Render(display,nextroundY);
			else
				b_tofinalresults->Render(display, nextroundY);
			break;
		case READYSETGO:
			RenderGame(display);
			break;
		case ROUNDOVER:
			RenderGame(display);
			display->Blit(17,roundoverY,i_rgbroundover,i_aroundover);
			break;
		case FINALRESULTS:
			display->Blit(0,0,i_finalresults);
			b_backtomainmenu->Render(display, nextroundY);
			TCHAR text[100];
			_stprintf(text, TEXT("LONGEST WORD: %s (%d LETTERS)"), longestwordtext, longestword);
			shared->font->DisplayText(display, 45,71, text,Color(0,0,0));
			_stprintf(text, TEXT("HIGHEST SCORING WORD: %s (%d POINTS)"), highestscoringwordtext, highestscoringword);
			shared->font->DisplayText(display, 45,81, text,Color(0,0,0));
			_stprintf(text, TEXT("ROUND 1: %d POINTS"), roundpoints[0]);
			shared->font->DisplayText(display, 45,91, text,Color(0,0,0));
			_stprintf(text, TEXT("ROUND 2: %d POINTS"), roundpoints[1]);
			shared->font->DisplayText(display, 45,101, text,Color(0,0,0));
			_stprintf(text, TEXT("ROUND 3: %d POINTS"), roundpoints[2]);
			shared->font->DisplayText(display, 45,111, text,Color(0,0,0));
			_stprintf(text, TEXT("TOTAL: %d POINTS"), totalpoints);
			shared->font->DisplayText(display, 45,121, text,Color(0,0,0));
			_stprintf(text, TEXT("%% WORDS FOUND: %.2f %%"), percentFound);
			shared->font->DisplayText(display, 45,131, text,Color(0,0,0));

			// top 5 highscores
			_stprintf(text, TEXT("TOP 5 HIGHSCORES"));
			shared->font->DisplayText(display, 20,185, text,Color(0,0,0));
			_stprintf(text, TEXT("TOP 5 %% WORDS FOUND"));
			shared->font->DisplayText(display, 140,185, text,Color(0,0,0));
			for ( i = 0; i < 5; i++) {
				if (newHighscore  == i+1) {
					_stprintf(text, TEXT("%d. %d"),i+1,shared->highscores[shared->whatLength][i]);
					shared->font->DisplayText(display,30,195 + (10 * i),text, Color(0,128,0));
				} else {
					_stprintf(text, TEXT("%d. %d"),i+1,shared->highscores[shared->whatLength][i]);
					shared->font->DisplayText(display,30,195 + (10 * i),text, Color(0,0,0));
				}
				if (newPercentScore == i+1) {
					_stprintf(text, TEXT("%d. %.2f %%"),i+1,shared->highpercentscores[shared->whatLength][i]);
					shared->font->DisplayText(display,150,195 + (10 * i),text, Color(0,128,0));
				} else {
					_stprintf(text, TEXT("%d. %.2f %%"),i+1,shared->highpercentscores[shared->whatLength][i]);
					shared->font->DisplayText(display,150,195 + (10 * i),text, Color(0,0,0));
				}
			}
			if (newHighscore) {
				_stprintf(text, TEXT("NEW HIGHSCORE!"));
				shared->font->DisplayText(display, 30,250, text,Color(0,128,0));
			}
			if (newPercentScore) {
				_stprintf(text, TEXT("NEW TOP %% SCORE!"));
				shared->font->DisplayText(display, 150,250, text,Color(0,128,0));
			}
				
			break;
	}
}
void Joggle::RenderResults(Display *display) {
	int i,j;
	display->Blit(0,0,i_results);
	resultsscrollbar->Render(display,shared, found, num_found, realpoints, founded);
	Frog::Rect dest = Frog::Rect(68,35,68+98,35+98);
	display->SetRenderTarget(temp);
	display->Clear(Color(255,0,255));

	switch (what_round)
	{
	case 0:
		display->Blit(0,0,i_round1);
		break;
	case 1:
		display->Blit(0,0,i_round2);
		break;
	case 2:
		display->Blit(0,0,i_round4);
		break;
	}
	

	display->SetRenderTarget(NULL);
	display->BlitStretch(dest,temp);
	for (i = 0; i < 6 ; i++)
		for (j = 0; j < 6; j++)
			if (round[what_round][i][j])
				display->Blit(68 + 1+ i * 16,35 + 1+j * 16, i_smallletters[grid[i][j]-97]);
	TCHAR text[50];
	_stprintf(text, TEXT("TOTAL POINTS:%d"), totalpoints);
	shared->bigfont->DisplayText(display,5,300,text,Color(0,0,0)); 
	_stprintf(text, TEXT("ROUND POINTS:%d"), roundpoints[what_round]);
	shared->bigfont->DisplayText(display,5,280,text,Color(0,0,0)); 
}

void Joggle::RenderGame(Display *display) {
	int i,j;
	display->Blit(0,0,i_background);
	switch (what_round)
	{
	case 0:
		display->Blit(23,boardy + 21,i_round1);
		break;
	case 1:
		display->Blit(23,boardy + 21,i_round2);
		break;
	case 2:
		display->Blit(23,boardy + 21,i_round4);
		break;
	}
	
	for (i = 0; i < 6 ; i++)
		for (j = 0; j < 6; j++)
			if (round[what_round][i][j])
				display->Blit(26 + i * 32, boardy + 24 + j * 32, i_letters[grid[i][j]-97]);

	scrollbar->Render(display,shared, playerfound, player_num_found, points);
	
	b_menu->Render(display,boardy);
	b_enter->Render(display,boardy);
	b_clear->Render(display,boardy);
	TCHAR time[10], pointstext[10];
	_stprintf(time, TEXT("%d:%02d"), minutes, seconds);
	_stprintf(pointstext, TEXT("%d"), totalpoints);
	shared->bigfont->DisplayText(display,153,246,TEXT("TIME LEFT:"),Color(0,0,0));
	shared->bigfont->DisplayText(display,174,260,time,Color(0,0,0));
	shared->bigfont->DisplayText(display,162,276,TEXT("POINTS:"),Color(0,0,0));
	shared->bigfont->DisplayText(display,174,292,pointstext,Color(0,0,0));
	shared->centaurfont->DisplayText(display,120-(_tcslen(name) * 5),220,name,Color(0,0,0));

	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++)
			if (picked[i][j]) {
				display->SetBlending(picked[i][j]);
				display->Blit(26 + i * 32, boardy + 24 + j * 32, i_letterpicked);
				display->SetBlending(255);
			}

	for ( i =0; i < 6; i++)
		for (j = 0; j < 6;j++)
			switch (gridarrows[i][j]) {
				case 1:
					display->Blit(26 + i * 32-10, boardy + 24 + j * 32-10,i_rgbarrows,i_aarrows,&arrows[3]);
					break;
				case 2:
					display->Blit(26 + i * 32+9, boardy + 24 + j * 32-10,i_rgbarrows,i_aarrows,&arrows[1]);
					break;
				case 3:
					display->Blit(26 + i * 32+20, boardy + 24 + j * 32-10,i_rgbarrows,i_aarrows,&arrows[4]);
					break;
				case 4:
					display->Blit(26 + i * 32-10, boardy + 24 + j * 32+5,i_rgbarrows,i_aarrows,&arrows[2]);
					break;
				case 5:
					display->Blit(26 + i * 32+20, boardy + 24 + j * 32+5,i_rgbarrows,i_aarrows,&arrows[0]);
					break;
				case 6:
					display->Blit(26 + i * 32-12, boardy + 24 + j * 32+22,i_rgbarrows,i_aarrows,&arrows[6]);
					break;
				case 7:
					display->Blit(26 + i * 32+9, boardy + 24 + j * 32+22,i_rgbarrows,i_aarrows,&arrows[7]);
					break;
				case 8:
					display->Blit(26 + i * 32+22, boardy + 24 + j * 32+22,i_rgbarrows,i_aarrows,&arrows[5]);
					break;
			}

		for ( i = 0; i < 6; i++)
			for (j = 0; j <6 ; j++)
				if (pointsgrid[i][j] ==2)
					display->Blit(26 + i * 32, boardy + 24 + j * 32, i_doubleword);
				else if (pointsgrid[i][j] == 3)
					display->Blit(26 + i * 32, boardy + 24 + j * 32, i_tripleword);

		display->Blit(-boardy+120-30,5,i_rgbroundheader[what_round],i_aroundheader[what_round]);
		if (showAnswerTime) {
			shared->bigfont->DisplayText(display,showAnswerX,showAnswerY,showAnswer,Color(0,128,0));
		}
}
void Joggle::StylusDown(Frog::Point stylus) {
	switch (state) {
		case JOGGLEGAME:
			b_menu->StylusDown(stylus);
			b_clear->StylusDown(stylus);
			b_enter->StylusDown(stylus);
			break;
	}

}
void Joggle::StylusMove(Frog::Point stylus) {
	switch (state) {
		case JOGGLEGAME:
			b_menu->StylusMove(stylus);
			b_clear->StylusMove(stylus);
			b_enter->StylusMove(stylus);
			break;
	}
}
void Joggle::Input() {
	int i,j;
	
	if (shared->input->StylusUp()) {
		if (b_enter->StylusUp(shared->input->getCoordinates())) {
			CheckWordFound();

			cursor = -1;
			for (i = 0; i < 36; i++)
				name[i] = 0;
			for (i = 0; i < 6; i++)
				for (j = 0; j < 6; j++) {
					picked[i][j] = 0;
					gridarrows[i][j] = 0;
				}
			for (i = 0; i < 36; i++)
				undo[i].Set(-2,-2);
		}
		if (b_clear->StylusUp(shared->input->getCoordinates())) {
			cursor = -1;
			for (i = 0; i < 36; i++)
				name[i] = 0;
			for (i = 0; i < 6; i++)
				for (j = 0; j < 6; j++) {
					picked[i][j] = 0;
					gridarrows[i][j] = 0;
				}
			for (i = 0; i < 36; i++)
				undo[i].Set(-2,-2);
		}
		if (b_menu->StylusUp(shared->input->getCoordinates())) {
			smallmenuOn = true;
			state = SMALLMENU;
			shared->PlaySoundA(4);
			/*
			animate = 0.5;
			animating= 1;
			nextState = MAINMENU;
			*/
		}
	}

	if (shared->input->StylusPressed()) {
		int t, u;
		bool pickedLetter=false;
		for (i = 0; i < 6; i++)
			for ( j= 0; j < 6; j++) {
				if (round[what_round][i][j]){
					if (cursor < 35)
					if (Frog::Rect(26 + i * 32, 24 + j * 32, 26 + i * 32 + 30, 24 + j * 32 + 30).Contains(shared->input->getCoordinates()))
						if (picked[i][j] == false) {
							// here we add a letter 
							for (t = -1; t <= 1; t++)
								for(u = -1; u <= 1; u++)
									if ((t || u))
										if ((t+last_picked.x == i && j == u+last_picked.y) || cursor == -1) {
											if (grid[i][j] - 97 + 65 == 'Q' && cursor < 6) {
												name[++cursor] = 'Q';
												name[++cursor] = 'U';
												undo[cursor].Set(i,j);
												picked[i][j] = true;
												last_picked.x = i;
												last_picked.y = j;		
												pickedLetter=true;
												shared->PlaySoundA(0);
											} else if (grid[i][j] - 97 + 65 != 'Q' && cursor < 7) {
												name[++cursor] = grid[i][j] - 97 + 65;	
												picked[i][j] = true;
												last_picked.x = i;
												last_picked.y = j;
												undo[cursor].Set(i,j);
												pickedLetter=true;
												shared->PlaySoundA(0);
											}
																					
										}
							if (!pickedLetter) {
								int a,b;
								CheckWordFound();
								cursor = -1;
								for (a = 0; a < 36; a++)
									name[a] = 0;
								for (a = 0; a < 6; a++)
									for (b = 0; b < 6; b++) {
										picked[a][b] = 0;
										gridarrows[a][b] = 0;
									}
								for (a = 0; a < 36; a++)
									undo[a].Set(-2,-2);

								if (grid[i][j] - 97 + 65 == 'Q' && cursor < 6) {
												name[++cursor] = 'Q';
												name[++cursor] = 'U';
												undo[cursor].Set(i,j);
												picked[i][j] = true;
												last_picked.x = i;
												last_picked.y = j;		
												pickedLetter=true;
												shared->PlaySoundA(0);
											} else if (grid[i][j] - 97 + 65 != 'Q' && cursor < 7) {
												name[++cursor] = grid[i][j] - 97 + 65;	
												picked[i][j] = true;
												last_picked.x = i;
												last_picked.y = j;
												undo[cursor].Set(i,j);
												pickedLetter=true;
												shared->PlaySoundA(0);
											}
							}
						} else {
							int a,b;
							// submit word if user clicks "cursor" letter
							if (last_picked.x == i && last_picked.y == j) {
								CheckWordFound();
								cursor = -1;
								for (a = 0; a < 36; a++)
									name[a] = 0;
								for (a = 0; a < 6; a++)
									for (b = 0; b < 6; b++) {
										picked[a][b] = 0;
										gridarrows[a][b] = 0;
									}
								for (a = 0; a < 36; a++)
									undo[a].Set(-2,-2);
							}  
							else {
							// here we backtrack and user wants to go back some letters
								for (t = 0; t < cursor+1; t++)
									if (undo[t].x == i && undo[t].y == j) {
										for (u = 35; u != t; u--) {
											name[u] = 0;
											if (undo[u].x != -2) {
												picked[undo[u].x][undo[u].y] = false;
												undo[u].Set(-2,-2);
											}
										}
										last_picked.x = undo[t].x;
										last_picked.y = undo[t].y;
										cursor = t;
										shared->PlaySoundA(0);
										break;
									}
							}
						}
						
				}
			}
	}
}

void Joggle::CheckWordFound() {
	int i, j;

	for (i = 0; i < num_found; i++)
		if (_tcscmp(found[i],name) == 0) {
			for (j = 0; j < player_num_found; j++)
				if (_tcscmp(playerfound[j],name) == 0) // if player already found word
					return;
			shared->PlaySoundA(3);
			int multiplier = 10;

			for (i = 0; i < 36; i++)
				if (undo[i].x != -2)
					if (pointsgrid[undo[i].x][undo[i].y])
						multiplier *= pointsgrid[undo[i].x][undo[i].y];

			TCHAR numpoints[10];
			_stprintf(numpoints, TEXT("%d"), _tcslen(name)*multiplier);
			totalpoints += _tcslen(name)*multiplier;
			roundpoints[what_round] += _tcslen(name)*multiplier;
			points[player_num_found] = _tcsdup(numpoints);
			playerfound[player_num_found++] = _tcsdup(name);
			scrollbar->MoveDownOne(player_num_found);
			
			if (highestscoringword < _tcslen(name)*multiplier) {
				_stprintf(highestscoringwordtext, name);
				highestscoringword = _tcslen(name)*multiplier;
			}
			//all-time version
			if (shared->highestscoringword[shared->whatLength] < _tcslen(name)*multiplier) {
				_stprintf(shared->highestscoringwordtext[shared->whatLength], name);
				shared->highestscoringword[shared->whatLength] = _tcslen(name)*multiplier;
			}
			if (longestword < _tcslen(name)) {
				_stprintf(longestwordtext, name);
				longestword = _tcslen(name);
			}
			// all-time version
			if (shared->longestword[shared->whatLength] < _tcslen(name)) {
				_stprintf(shared->longestwordtext[shared->whatLength], name);
				shared->longestword[shared->whatLength] = _tcslen(name);
			}
			// all-time stuff
			shared->accumulatedpoints[shared->whatLength] += _tcslen(name)*multiplier;
			shared->totalwordsfound[shared->whatLength] += 1;
			shared->totallettersfound[shared->whatLength] += _tcslen(name);
			_stprintf(showAnswer, TEXT("%s    +%d"), name, _tcslen(name)*multiplier);
			showAnswerTime = 255;
			
			showAnswerY = 289;
			showAnswerX = 24;
			break;
		}
	
}