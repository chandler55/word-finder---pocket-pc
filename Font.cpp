/*

  Font class 0.1 by Jason Chong.  jcvw75@yahoo.com
  Compatible with bitmap font files generated by
  MetalShard Fontmaker found in gapidraw package.
  This is early release, feel free to fix bugs and improve
  upon it and don't forget to send a copy to me at my email! :-)

*/

#include "font.h"

Font::Font(Surface *fs)
{
  newcolor= false;
  HDC hSurfaceDC;
  COLORREF color;
  int ch,klength, i;

  FontSurface = fs;

  hSurfaceDC = FontSurface->GetDC( false );

  ascii= new TCHAR[256];
  _tcscpy(ascii,TEXT(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~_"));

  ch=0;
  klength=0;
  for (i=0; i<256; i++) 
  {
    chardata[i].length=0;
    chardata[i].offset=0;
  }

  for (i=0; i<fs->GetWidth(); i++)
  {
    color = GetPixel(hSurfaceDC,i,0);    
    //if (color == RGB(255,255,255))  // doesn't work in pocketpc, only desktop pc. white is not always white ? Help me with this.
    if ((GetRValue(color)>0) && (GetGValue(color)>0) && (GetBValue(color)>0)) // had to use these instead.
    { // test for the width data on top of each char. Leave 1 blank channel when using color masks. use 255,0,255 for mask. 
      if (klength==0) chardata[ascii[ch]].offset=i;
      klength++;     // true, not a mask color so record length of char.
    } 
    else
    {  // it's a mask color so ignore.
      if (klength!=0) 
      {
        chardata[ascii[ch]].length=klength;
        ch++;
      }
      klength=0;
    }
  }

  FontSurface->ReleaseDC( hSurfaceDC ); 
  FontSurface->SetColorMask(Color(255,0,255));
}

void Font::DisplayText(Display *display, int x, int y, TCHAR *str, Frog::Pixel color) {
	MakeNewColor((color & RED_MASK) >> 8, (color & GREEN_MASK) >> 3, (color & BLUE_MASK) << 3);
	DisplayText(display,x,y,str);
}

void Font::DisplayText(Display *display, int x, int y, TCHAR *str)
{
  unsigned int i,xoffset;
  Rect R;
  int height = FontSurface->GetHeight();
  
  xoffset=x; 
  if (newcolor) {
	  display->SetPixelShader(&newColor);
	  newcolor = false;
  }

  for (i=0; i<_tcslen(str); i++)
  {
    SetRect(&R,chardata[str[i]].offset,1,chardata[str[i]].offset+chardata[str[i]].length,height);
	
    display->Blit(xoffset,y,FontSurface,&R);    
    xoffset += chardata[str[i]].length;
  }

  display->SetDefaultShader();
}

Font::~Font()
{ 
  delete ascii;
}
