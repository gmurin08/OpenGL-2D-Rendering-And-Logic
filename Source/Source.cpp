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
#include "Brick.h"
#include "Circle.h"

void processInput(GLFWwindow* window, Brick* paddlePos);
void displayWelcomeMessage();
void handleMovement(Brick paddle);
void createBrickGrid(int rows, int cols);

//Object vectors to hold circle and brick objects
vector<Circle> ballVec;
vector<Brick> brickVec;

int main(void) {

	//Window Initialization
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

	//Build an array of brick objects and render onto screen
	createBrickGrid(10, 5);

	//Generate objects for ball and paddle
	Brick paddle(REFLECTIVE, 0.0, -1.1, 0.2, 1.0, 1.0, 1.0);
	Circle B(0, -0.8, 02, 2, 0.05, 1, 1, 0);
	ballVec.push_back(B);

	//Main loop
	while (!glfwWindowShouldClose(window)) {

		//Setup view
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		//check position of objects in the view
		processInput(window, &paddle);

		//Populate brick vector to keep score
		for (int i = 0; i < brickVec.size(); i++) {
			brickVec.at(i).drawBrickCoords();
		}

		//IF its the first iteration of the loop show welcome popup
		if (cycle == 1) {
			displayWelcomeMessage();
		}

		//Draw paddle and take int new coords for next draw
		paddle.drawPaddle();
		handleMovement(paddle);

		glfwSwapBuffers(window);
		glfwPollEvents();

		//IF the player wins or loses show a message
		if (brickVec.size() < 1) {
			MessageBoxA(NULL, "You WON!!!", "Congratulations!", MB_ICONASTERISK | MB_OK);
			break;
		}
		if (ballVec.size() < 1) {
			MessageBoxA(NULL, "***********************\n You Ran Out of Balls!\n\n "
				"GAME OVER!\n * **********************\n\n", "Oops!", MB_ICONASTERISK | MB_OK);
			break;
		}

		//Iterate cycle
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

void displayWelcomeMessage() {

	MessageBoxA(NULL, "-Press 'A' to move the paddle left\n-Press 'D' to move the paddle right."
		"\n\nTry to get all the bricks before you lose your ball!", "Welcome To Brick Breaker!", MB_ICONASTERISK | MB_OK);

}

void handleMovement(Brick paddle) {
	//Movement
	for (int i = 0; i < ballVec.size(); i++)
	{
		ballVec[i].CheckCollision(&paddle);
		for (int j = 0; j < brickVec.size(); j++) {
			if (ballVec[i].CheckCollision(&brickVec.at(j))) {
				brickVec.erase(brickVec.begin() + j);
			}
		}
		ballVec[i].Move();
		ballVec[i].DrawCircle();
		if (ballVec[i].y < -0.99) {
			ballVec.erase(ballVec.begin() + i);
		}
	}
}

void processInput(GLFWwindow* window, Brick* paddlePos)
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
		ballVec.push_back(B);
		
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