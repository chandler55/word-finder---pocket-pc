#pragma once
#include <PocketFrog/PocketFrog.h>
#include "Input.h"

using namespace Frog;

#define ANIMATION_MOVERIGHT 1
#define ANIMATION_MOVELEFT 2
#define ANIMATION_MOVEUP 3
#define ANIMATION_MOVEDOWN 4

#define PI 3.14159265

class RectFloat {
public:
	float left,right,top,bottom;
	bool  Contains( const Point& point ) const   { return point.x >= left && point.x < right && point.y >= top && point.y < bottom; }
	RectFloat()                                       {}
	RectFloat( int l, int t, int r, int b)            { left = (float)l; top = (float)t; right = (float)r; bottom = (float)b; }
	RectFloat( const RectFloat& r )                        { *static_cast<RectFloat*>(this) = r; }
	RectFloat( const Point& p, int w, int h )         { left = p.x; top = p.y; right = p.x + w; bottom = p.y + h; }
	RectFloat( const Point& p1, const Point& p2 )     { left = p1.x; top = p1.y; right = p2.x; bottom = p2.y; }
};
class Button {
	Surface *downImage, *downImageAlpha;
	Surface *upImage, *upImageAlpha;
	bool hasAlpha;
	RectFloat rect;
	bool isDown;
	char animation;
	char animating;
	bool upimageexist;
	float leftbackup,rightbackup, upbackup,downbackup;
	int width, height;
public:
	Button() { }
	~Button() {}
	void SetAnimate(int a) { animating = a; }
	void Animate(float delta) {
		if (animating == 1) {
			switch (animation) {
						case ANIMATION_MOVERIGHT:
							rect.left+=delta*500;
							rect.right+=delta*500;
							break;
						case ANIMATION_MOVELEFT:
							rect.left-=delta*500;
							rect.right-=delta*500;
							break;
						case ANIMATION_MOVEUP:
							rect.bottom-=delta*500;
							rect.top-=delta*500;
							break;
						case ANIMATION_MOVEDOWN:
							rect.bottom+=delta*500;
							rect.top+=delta*500;
							break;
			}
		} else if (animating == 2) {
			switch (animation) {
						case ANIMATION_MOVERIGHT:
							rect.left-=delta*(500);
							rect.right-=delta*(500);
							if (rect.left < leftbackup)
								rect.left = leftbackup;
							if (rect.right < rightbackup)
								rect.right = rightbackup;
							break;
						case ANIMATION_MOVELEFT:
							rect.left+=delta*(500);
							rect.right+=delta*(500);
							if (rect.left > leftbackup)
								rect.left = leftbackup;
							if (rect.right > rightbackup)
								rect.right = rightbackup;
							break;
						case ANIMATION_MOVEUP:
							rect.bottom+=delta*500;
							rect.top+=delta*500;
							if (rect.bottom > downbackup)
								rect.bottom = downbackup;
							if (rect.top > upbackup)
								rect.top = upbackup;
							break;
						case ANIMATION_MOVEDOWN:
							rect.bottom-=delta*500;
							rect.top-=delta*500;
							if (rect.bottom < downbackup)
								rect.bottom = downbackup;
							if (rect.top < upbackup)
								rect.top = upbackup;
							break;
			}
		}
	}
	void Done() {
		rect.left = leftbackup;
		rect.right =rightbackup;
		rect.top = upbackup;
		rect.bottom = downbackup;
		
		animating = 0;
	}
	void Start() {
		leftbackup =rect.left;
		rightbackup = rect.left + width;
		upbackup = rect.top;
		downbackup = rect.top + height;
		switch (animation) {
			case ANIMATION_MOVERIGHT:
				rect.left = 0.5 * 500 + rect.left;
				rect.right = 0.5 * 500 + rect.right;
				break;
			case ANIMATION_MOVELEFT:
				rect.left = -1 * 0.5 * 500 + rect.left;
				rect.right = -1 * 0.5 * 500 + rect.right;
				break;
			case ANIMATION_MOVEUP:
				rect.bottom = 0.5 * 500 + rect.bottom;
				rect.top =  0.5 * 500 + rect.top;
				break;
			case ANIMATION_MOVEDOWN:
				rect.bottom = -1 * 0.5 * 500 + rect.bottom;
				rect.top =  -1 * 0.5 * 500 + rect.top;
				break;
		}
		animating = 2;
	}
	void makeFalse() { isDown = false;}
	void draw(Display *display, float offsetx, float offsety) {
		if (!hasAlpha)
			display->Blit((int)rect.left + offsetx, (int)rect.top + offsety,downImage);
		else
			display->Blit((int)rect.left + offsetx, (int)rect.top + offsety,downImage, downImageAlpha);
	}
	void StylusMove(Point stylus) {
		if (!rect.Contains(stylus))
			isDown = false;
	}
	void StylusDown(Point stylus) {
		if (rect.Contains(stylus))
			isDown = true;
		else
			isDown = false;
	}
	bool Contains(Point stylus) {
		return rect.Contains(stylus);
	}
	bool StylusUp(Point stylus) {
		bool temp = isDown;
		isDown = false;
		return rect.Contains(stylus) && temp;
	}
	void Render(Display *display) {
		if (animating != 0) {
			if (upimageexist)
				display->Blit((int)rect.left, (int)rect.top,upImage);
			if (isDown)
				draw(display, 0,0);
		} else {
			if (upimageexist)
				display->Blit((int)rect.left, (int)rect.top,upImage);
		}
	}
	void Render(Display *display, float offset) {
		float offsetx = 0, offsety=0;

		switch (animation) {
			case ANIMATION_MOVERIGHT:
				offsetx = -offset;
				break;
			case ANIMATION_MOVELEFT:
				offsetx = offset;
				break;
			case ANIMATION_MOVEUP:
				offsety = offset;
				break;
			case ANIMATION_MOVEDOWN:
				offsety = -offset;
				break;
		}
		if (animating != 0) {
			if (upimageexist) 
				if (!hasAlpha)
					display->Blit((int)rect.left + offsetx, (int)rect.top + offsety,upImage);
				else
					display->Blit((int)rect.left + offsetx, (int)rect.top + offsety,upImage,upImageAlpha);
			if (isDown)
				draw(display, offsetx,offsety);
		} else {
			if (upimageexist)
				if (!hasAlpha)
					display->Blit((int)rect.left + offsetx, (int)rect.top + offsety,upImage);
				else
					display->Blit((int)rect.left + offsetx, (int)rect.top + offsety,upImage,upImageAlpha);
		}
	}
	void GameLoop(Display *display, Input *input) {
		if (isDown)
			draw(display, 0,0);
	}
	void Init(Display *display, long id, int left, int top, int fwidth, int fheight ) {
		hasAlpha = false;
		downImage = LoadImage(display, id);
		rect = RectFloat(left, top, left + fwidth, top + fheight);
		isDown = false;
		upimageexist = false;
		leftbackup =rect.left;
		rightbackup = rect.left + fwidth;
		upbackup = rect.top;
		downbackup = rect.top + fheight;
		width = fwidth;
		height = fheight;
	}
	void Init(int animation2, Display *display,long id2, long id, int left, int top, int fwidth, int fheight ) {
		hasAlpha = false;
		animation= animation2;
		upImage = LoadImage(display, id2);
		downImage = LoadImage(display, id);
		rect = RectFloat(left, top, left + fwidth, top + fheight);
		isDown = false;
		upimageexist =true;
		leftbackup =rect.left;
		rightbackup = rect.left + fwidth;
		upbackup = rect.top;
		downbackup = rect.top + fheight;
		width = fwidth;
		height = fheight;
	}
	void Init(int animation2, Display *display,long id, long id2, long idalpha, long id2alpha, int left, int top, int fwidth, int fheight ) {
		hasAlpha = true;
		animation= animation2;
		upImage = LoadImage(display, id2);
		downImage = LoadImage(display, id);
		upImageAlpha = LoadImage(display,id2alpha);
		downImageAlpha = LoadImage(display, idalpha);
		rect = RectFloat(left, top, left + fwidth, top + fheight);
		isDown = false;
		upimageexist =true;
		leftbackup =rect.left;
		rightbackup = rect.left + fwidth;
		upbackup = rect.top;
		downbackup = rect.top + fheight;
		width = fwidth;
		height = fheight;
	}
	void SetColorMask() {
		downImage->SetColorMask(Color(255,0,255));
	}
	void SetColorMask(COLORREF color) {
		downImage->SetColorMask(color);
	}
	void Delete() {
		delete downImage;
	}
	void SetY(float y) {
		rect.top = y;
		rect.bottom = rect.top + height;
	}
};