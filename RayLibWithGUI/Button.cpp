#include "Button.h"
#include "raylib.h"

Button::Button(Rectangle newRect, const char* newText, int newFontSize, Color newTextColour, Color newColourCurrent, Color newColourIdle, Color newColourSel, Color newColourHov, Color newColourHovSel)
{
	hovering = false;
	selected = false;
	rect = newRect;

	text = newText;
	fontSize = newFontSize;
	textColour = newTextColour;

	colourCurrent = newColourCurrent;
	colourIdle = newColourIdle;
	colourSel = newColourSel;
	colourHov = newColourHov;
	colourHovSel = newColourHovSel;
}

bool Button::IsMouseOver(Rectangle rect) {
	return CheckCollisionPointRec(GetMousePosition(), rect);
}

bool Button::DrawButton()
{
	//Check if it is being hovered over.
	hovering = IsMouseOver(rect);
	
	if (hovering == true)
	{ 
		if (selected == true)
		{
			colourCurrent = colourHovSel;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				selected = false;
			}
		}
		else
		{
			colourCurrent = colourHov;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				selected = true;
			}
		}
	}
	else if (selected == true)
	{
		colourCurrent = colourSel;
	}
	else
	{
		colourCurrent = colourIdle;
	}

	DrawRectangleRounded(rect, 0.1, 10, colourCurrent);

	DrawText(
		text,
		rect.x + rect.width/2 - MeasureText(text,fontSize)/2,				//x pos
		rect.y + rect.height/2 - fontSize/2,								//y pos
		fontSize,															//font size
		textColour															//text colour
		);

	return selected;
}