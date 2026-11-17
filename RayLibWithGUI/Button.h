#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "raylib.h"

class Button {
public:
	bool hovering{ false };
	bool selected{ false };
	Rectangle rect;

	const char* text{ "" };
	int fontSize;
	Color textColour;

	Color colourCurrent;
	Color colourIdle;
	Color colourSel;
	Color colourHov;
	Color colourHovSel;

	Button(Rectangle newRect, const char* newText, int newFontSize, Color newTextColour, Color newColourCurrent, Color newColourIdle, Color newColourSel, Color newColourHov, Color newColourHovSel);

	bool IsMouseOver(Rectangle rect);

	bool DrawButton();

};


#endif