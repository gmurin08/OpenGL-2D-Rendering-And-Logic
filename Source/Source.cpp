#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

const float DEG2RAD = 3.14159 / 180;



enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
enum ONOFF { ON, OFF };


class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	vector<float> coords;
	
	BRICKTYPE brick_type;
	ONOFF onoff;

	//Default constructor
	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		onoff = ON;
	};

	//Overloaded constructor to faciliate raw coordinates
	Brick(BRICKTYPE bt, vector<float> c_coords, float rr, float gg, float bb) {
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

	void drawPaddle(){
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

	void movePaddle(Brick *paddle, int direction) {
		if (direction == 1) {
			paddle->x += 0.001;
		}
		if (direction == -1) {
			paddle->x += 0.001;
		}
	}
};

void p_processInput(GLFWwindow* window, Brick* paddlePos);
void welcomeMessage();
void handleMovement(Brick paddle);
void createBrickGrid(int rows, int cols);

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
		int flag=0;
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

//Vector of circles generated
vector<Circle> world;
//Vector of brick objects for ease of iteration
vector<Brick> brickVec;

int main(void) {
	srand(time(NULL));
	int cycle = 0;
	if (!glfwInit()) {
		::exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(1280,1024, "Brick Breaker", NULL, NULL);
	if (!window) {
		glfwTerminate();
		::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	Brick paddle(REFLECTIVE, 0.0, -1.1, 0.2, 1.0, 1.0, 1.0);
	
	//Build an array of brick objects and render onto screen
	createBrickGrid(10, 5);
	Circle B(0, -0.8, 02, 2, 0.05, 1, 1, 0);
	world.push_back(B);

	while (!glfwWindowShouldClose(window)) {
		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		p_processInput(window, &paddle);
		for (int i = 0; i < brickVec.size(); i++) {
			brickVec.at(i).drawBrickCoords();
		}

		paddle.drawPaddle();

		if (cycle == 1) {
			welcomeMessage();
		}

		handleMovement(paddle);

		glfwSwapBuffers(window);
		glfwPollEvents();
		if (brickVec.size() < 1) {
			MessageBoxA(NULL, "You WON!!!", "Congratulations!", MB_ICONASTERISK | MB_OK);
			break;
		}

		if (world.size() < 1) {
			MessageBoxA(NULL, "***********************\n You Ran Out of Balls!\n\n GAME OVER!\n*********************** \n\n", "Oops!", MB_ICONASTERISK | MB_OK);
			break;
		}

		cycle++;
	}

	glfwDestroyWindow(window);
	glfwTerminate;
	::exit(EXIT_SUCCESS);
}

//Iteratively calculates coordinates for bricks based on given number of rows and columns
void createBrickGrid(int rows, int cols)
{

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			vector<float>coords;
			coords.push_back(-1.0 + (((float)j) / cols) + 0.2);
			coords.push_back((float)i * 0.1); //TR
			coords.push_back(-1.0 + (((float)j) / cols) + 0.2);
			coords.push_back((float)i * 0.1 + 0.1); //BR
			coords.push_back(-1.0 + (((float)j) / cols));
			coords.push_back((float)i * 0.1 + 0.1); //BL
			coords.push_back(-1.0 + (((float)j) / cols));
			coords.push_back((float)i * 0.1); //TR
			float r, g, b;
			r = rand() / 10000.0000;
			g = rand() / 10000.0000;
			b = rand() / 10000.0000;
			Brick B(DESTRUCTABLE, coords, r, g, b);
			B.width = 0.06, B.x = ((B.coords[2] - B.coords[4]) / 2), B.y = ((B.coords[1] - B.coords[3]) / 2);
			brickVec.push_back(B);
			
		}
		//i = rows;
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			vector<float>coords;
			coords.push_back((((float)j) / cols) + 0.2);
			coords.push_back((float)i * 0.1); //TR
			coords.push_back((((float)j) / cols) + 0.2);
			coords.push_back((float)i * 0.1 + 0.1); //BR
			coords.push_back((((float)j) / cols));
			coords.push_back((float)i * 0.1 + 0.1); //BL
			coords.push_back((((float)j) / cols));
			coords.push_back((float)i * 0.1); //TR
			float r, g, b;
			r = rand() / 10000.0000;
			g = rand() / 10000.0000;
			b = rand() / 10000.0000;
			Brick B(DESTRUCTABLE, coords, r, g, b);
			B.width = 0.06, B.x = ((B.coords[2] - B.coords[4]) / 2), B.y = ((B.coords[1] - B.coords[3]) / 2);
			brickVec.push_back(B);
		}
		//i = rows;
	}
}

void welcomeMessage() {

	MessageBoxA(NULL, "-Press 'A' to move the paddle left\n-Press 'D' to move the paddle right."
		"\n\nTry to get all the bricks before you lose your ball!", "Welcome To Brick Breaker!", MB_ICONASTERISK | MB_OK);

}

void handleMovement(Brick paddle) {
	//Movement
	for (int i = 0; i < world.size(); i++)
	{
		world[i].CheckCollision(&paddle);
		for (int j = 0; j < brickVec.size(); j++) {
			if (world[i].CheckCollision(&brickVec.at(j))) {
				brickVec.erase(brickVec.begin() + j);
			}
		}
		world[i].Move();
		world[i].DrawCircle();
		if (world[i].y < -0.99) {
			world.erase(world.begin() + i);
		}
	}
}

void p_processInput(GLFWwindow* window, Brick* paddlePos)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);



	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		double r, g, b;
		r = rand() / 10000;
		g = rand() / 10000;
		b = rand() / 10000;
		Circle B(0, -0.8, 02, 2, 0.05, r, g, b);
		world.push_back(B);
		
	}

	//D and A keys move paddle right/left if within boundaries
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (paddlePos->x < 1.0 - paddlePos->width*1.5) {
			paddlePos->x += 0.01;
		}	
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (paddlePos->x > -1.0 + paddlePos->width * 1.5) {
			paddlePos->x -= 0.01;
		}
	}
}