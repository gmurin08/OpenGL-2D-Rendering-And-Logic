#pragma once
#include <GLFW\glfw3.h>

class Circle
{
public:
	float red, green, blue, radius, x, y, speed = 0.03;
	int direction; // 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left
	float x_travel = 0.0, y_travel = 0.0;

	Circle(float xx, float yy, double rr, int dir, float rad, float r, float g, float b)
	{
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
	}

	int CheckCollision(Brick* brk)
	{
		if (brk->brick_type == REFLECTIVE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				y_travel = -y_travel;
			}
		}
		else if (brk->brick_type == DESTRUCTABLE)
		{
			if ((x >= brk->coords[6] && x <= brk->coords[2]) && (y <= brk->coords[3] && y >= brk->coords[1])) {
				brk->onoff = OFF;
				x_travel = -x_travel;
				y_travel = -y_travel;
				return 1;
			}
		}

		return 0;
	}

	//used at startup to randomize movement
	int GetRandomDirection()
	{
		return (rand() % 8) + 1;
	}

	//Moves Circle object one step in a given direction
	int Move() {
		int flag = 0;
		//IF velocity is at 0 (newly initialized)
		if (x_travel == 0 && y_travel == 0) {
			//Set a starting speed and trajectory

			//Randomize horizontal trajectory
			if (rand() % 2) {
				x_travel = 0.02;
			}
			else {
				x_travel = -0.02;
			}

			y_travel = 0.02;
		}

		//IF we reach left or right edge of screen
		if (x >= 0.99 || x <= -0.99) {
			//Invert our horizontal trajectory to 'bounce off the wall'
			x_travel = -x_travel;
		}

		//IF we hit the top of the screen
		if (y >= 0.99) {
			//Invert vertical trajectory to "bounce"
			y_travel = -y_travel;

		}

		//IF our ball goes out of bounds along bottom of screen
		if (y <= 0.99) {
			//Flag for removal
			flag = 1;
		}

		//Increment final current trajectory by 1
		x += x_travel;
		y += y_travel;

		return flag;
	}

	void DrawCircle()
	{
		glColor3f(red, green, blue);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
		}
		glEnd();
	}
};