#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Input.h"
#include "Shared.h"

class Scrollbar
{
	Surface *scrollbox, *i_bottomarrow, *i_toparrow, *i_scrollbar;
	
	int scrollbarx, scrollbary;
	float scrolly;
	bool scrollPressed;
	int number;
	int height;
	int width;
public:
	Scrollbar(void);
public:
	~Scrollbar(void);
	void Render(Display *display, Shared *shared, TCHAR *found[1000], int num_found, TCHAR *points[1000]);
	void GameLoop(Shared *shared);
	void Init(Display *display);
	void Init(Display *display, int h, int w, int x, int y);
	void MoveDownOne(int num_found);
	void InitGraphics(Display *display);
	void ResetScrollY();
	void Render(Display *display, Shared *shared, TCHAR *found[1000], int num_found, TCHAR *points[1000], bool founded[1000]);
};
