#pragma once
#include "module.h"
#include "Scrollbar.h"
#include "Button.h"


#define BOX_SIZE	6

#define MIN_WORD_LEN	3
#define MAX_WORD_LEN	7

#define NOT_SEEN	0
#define SEEN		1

#define DICT_SIZE       81000
#define FOUND_SIZE      1000
enum JoggleState { JOGGLEGAME, RESULTS, READYSETGO, ROUNDOVER, FINALRESULTS, SMALLMENU, INITIATEGAME };
class Joggle :
	public Module
{
	char init;
	float showAnswerX, showAnswerY, showAnswerTime;
	TCHAR showAnswer[30];

	int newHighscore, newPercentScore;
	bool smallmenuOn;
	bool soundOn;
	float smallmenuY;
	int longestword; int highestscoringword; int wordsFound, wordsTotal; float percentFound;
	TCHAR longestwordtext[10]; TCHAR highestscoringwordtext[10];

	//bool newgame;
	float roundoverY;
	float roundover;
	float nextroundY;
	bool flipnextround;
	char fliproundover;
	float speedroundover;
	Surface *temp;
	float clockmover;
	JoggleState state;
	int words;
	char animating;
	float animate;
	int seconds, minutes;
	char pointsgrid[6][6];
	char gridarrows[6][6];
	char grid[6][6];
	float picked[6][6];
	Frog::Point last_picked;
	Frog::Point undo[36];
	Frog::Rect arrows[8];
	Surface *i_background2, *i_background, *i_square,*i_smallletters[26],*i_highlighted[26], *i_letters[26],
		*i_round1, *i_round2, *i_round4, *i_doubleword, *i_tripleword, *i_letterpicked,
		*i_aarrows, *i_rgbarrows, *i_aroundheader[3], *i_rgbroundheader[3], *i_results, *i_aroundover, *i_rgbroundover
		, *i_resultsHeader[4], *i_anextround, *i_anextrounddown,
		 *i_finalresults, *i_rgbsmallmenu, *i_asmallmenu, *i_smallmenu, *i_smallmenu2;
	Scrollbar *scrollbar, *resultsscrollbar;
	Button *b_enter, *b_clear, *b_menu, *b_nextround, *b_tofinalresults, *b_backtomainmenu;
	int cursor, undocursor;
	TCHAR name[36];
	char what_round;
	float boardy;
	TCHAR *playerfound[FOUND_SIZE];
	TCHAR *found[FOUND_SIZE];
	TCHAR *points[FOUND_SIZE];
	TCHAR *realpoints[FOUND_SIZE];
	bool founded[FOUND_SIZE];
	int totalpoints;
	int roundpoints[3];
	int num_bigwords;
	int num_words, num_found, player_num_found;

	char letters[6][6];
	int graph[6][6];
	TCHAR choices[MAX_WORD_LEN+1];
	TCHAR dict[DICT_SIZE][8];
	char test[26][26][26][26];

	bool round[3][6][6];
public:
	Joggle(void);
public:
	~Joggle(void);
	void Animate();
	void InitiateGame();
    virtual void GameInit(Display *display);
    virtual void GameEnd();
    virtual void GameLoop(Display *display);
	virtual void Render(Display *display);
	void RenderResults(Display *display);
	void RenderGame(Display *display);
	virtual void StartAnimation();
	virtual void FinishAnimation();
	void Input();
	bool MoveClock();
	void try_print(TCHAR *word);
	void output(int depth);
	void dfs(int x, int y, int depth);
	void CheckWordFound();
	char ConnectedHow(char a, char b, char c, char d);
	void CalculateResults();
	void StylusDown(Frog::Point stylus);
	void StylusMove(Frog::Point stylus);
};
