#pragma once
#include <PocketFrog/PocketFrog.h>

using namespace Frog;

struct makeNewColor : public PixelShader {
	int r, g, b;
	Pixel operator() (Pixel src, Pixel dest) const {
		return Color(r,g,b);
	}
	void changeColor(int a, int c, int d) {
		r = a; g = c; b = d;
	}
};

typedef struct _CDATA {
  int offset;
  int length;
}CDATA;

class Font{

public:
  Font(Surface *fs);
  ~Font(); 
  void DisplayText(Display *display, int x, int y, TCHAR *str, Frog::Pixel color);
  void DisplayText(Display *display, int x, int y, TCHAR *str);
  void MakeNewColor(int a, int b, int c)
  { 
	  newcolor = true; 
	  newColor.changeColor(a,b,c);
  }
private:
  TCHAR *ascii;
  CDATA chardata[256];
  Surface *FontSurface;
  makeNewColor newColor;
  bool newcolor;
};