#pragma once
#include <GLFW\glfw3.h>
#include <vector>


class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	std::vector<float> coords;

	BRICKTYPE brick_type;
	ONOFF onoff;

	//Default constructor
	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		onoff = ON;
	};

	//Overloaded constructor to faciliate raw coordinates
	Brick(BRICKTYPE bt, std::vector<float> c_coords, float rr, float gg, float bb) {
		brick_type = bt, coords = c_coords, red = rr, green = gg, blue = bb;
		onoff = ON;
	}


	//Render bricks
	void drawBrick()
	{
		//If brick is active
		if (onoff == ON)
		{
			//define variables to augment the shape of bricks into rectangle
			double halfside = width / 2;
			double longside = width * 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + longside, y + halfside);
			glVertex2d(x + longside, y - halfside);
			glVertex2d(x - longside, y - halfside);
			glVertex2d(x - longside, y + halfside);

			glEnd();
		}
	}

	void drawBrickCoords() {
		//If brick is active
		if (onoff == ON)
		{
			//define variables to augment the shape of bricks into rectangle
			double halfside = width / 2;
			double longside = width * 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(coords.at(0), coords.at(1));   //Top right
			glVertex2d(coords.at(2), coords.at(3));   //Bottom right
			glVertex2d(coords.at(4), coords.at(5));   //Bottom left
			glVertex2d(coords.at(6), coords.at(7));   //Top left

			glEnd();
		}

	}

	void drawPaddle() {
		//If brick is active
		if (onoff == ON)
		{
			//define variables to augment the shape of bricks into rectangle
			double halfside = width / 1.5;
			double longside = width * 1.5;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + longside, y + halfside);
			glVertex2d(x + longside, y - halfside);
			glVertex2d(x - longside, y - halfside);
			glVertex2d(x - longside, y + halfside);

			glEnd();
		}
	}

	void movePaddle(Brick* paddle, int direction) {
		if (direction == 1) {
			paddle->x += 0.001;
		}
		if (direction == -1) {
			paddle->x += 0.001;
		}
	}
};