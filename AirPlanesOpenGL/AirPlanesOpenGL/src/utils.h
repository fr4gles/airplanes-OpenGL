#pragma once

class Color
{
public:
	Color() : red(0.0f), green(0.0f), blue(0.0f){}
	Color(GLfloat r, GLfloat g, GLfloat b) : red(r), green(g), blue(b){}
		
	GLfloat red, green, blue;
};
