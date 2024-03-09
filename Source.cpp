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

void processInput(GLFWwindow* window);

enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE };
time_t gStartTime;
int gExpireSecs;
bool gFirstTime = true;

class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	BRICKTYPE brick_type;
	int hits;

	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		hits = 0;
	};

	void drawBrick()
	{
		if (hits < 5)
		{
			red -= 0.2 * hits;
			blue -= 0.2 * hits;
			green -= 0.1 * hits;
			double halfside = width / 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + halfside, y + halfside);
			glVertex2d(x + halfside, y - halfside);
			glVertex2d(x - halfside, y - halfside);
			glVertex2d(x - halfside, y + halfside);

			glEnd();
		}
	}
};


class Circle
{
public:
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.01;
	int direction; // 1=up 2=right 3=down 4=left 5 = up right  6 = up left  7 = down right  8= down left

	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
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

	void CheckBCollision(Circle* ball) {
			if ((x > ball->x - ball->radius && x <= ball->x + ball->radius) && (y > ball->y - ball->radius && y <= ball->y + ball->radius))
			{
				// change color of the balls when they collide
				double r, g, b, d;
				r = rand() % 30 + 30;
				r = r / 100;
				b = rand() % 50 + 30;
				b = b / 100;

				red = r;
				blue = b;
				ball->red = r;
				ball->blue = b;
			}
	}

	void CheckCollision(Brick* brk)
	{
		if (brk->brick_type == REFLECTIVE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				// reverse direction physics upon collision with brick
				if (direction == 1 || direction == 2) {
					direction += 2;
				}
				else if (direction == 3 || direction == 4) {
					direction -= 2;
				}
				else if (direction == 5) {
					direction == 8;
				}
				else if (direction == 6) {
					direction = 7;
				}
				else if (direction = 7) {
					direction = 6;
				}
				else if (direction = 8) {
					direction = 5;
				}
				x = x + 0.03;
				y = y + 0.04;
			}
		}
		else if (brk->brick_type == DESTRUCTABLE)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				brk->hits += 1;
				// reverse direction physics upon collision with brick
				if (direction == 1 || direction == 2) {
					direction += 2;
				}
				else if (direction == 3 || direction == 4) {
					direction -= 2;
				}
				else if (direction == 5) {
					direction == 8;
				}
				else if (direction == 6) {
					direction = 7;
				}
				else if (direction = 7) {
					direction = 6;
				}
				else if (direction = 8) {
					direction = 5;
				}
				x = x + 0.03;
				y = y + 0.04;
			}
		}
	}

	int GetRandomDirection()
	{
		return (rand() % 8) + 1;
	}

	void MoveOneStep()
	{
		if (direction == 1 || direction == 5 || direction == 6)  // up
		{
			if (y > -1 + radius) // if not colliding with a wall
			{
				y -= speed;
			}
			else // reverse direction upon wall collision
			{
				if (direction == 1) {
					direction = 3;
				}
				else if (direction == 5) {
					direction = 7;
				}
				else if (direction == 6) {
					direction = 8;
				}
			}
		}

		if (direction == 2 || direction == 5 || direction == 7)  // right
		{
			if (x < 1 - radius) // if not colliding with a wall
			{
				x += speed;
			}
			else
			{
				if (direction == 2) {
					direction = 4;
				}
				else if (direction == 5) {
					direction = 6;
				}
				else if (direction == 7) {
					direction = 8;
				}
			}
		}

		if (direction == 3 || direction == 7 || direction == 8)  // down
		{
			if (y < 1 - radius) {
				y += speed;
			}
			else
			{
				if (direction == 3) {
					direction = 1;
				}
				else if (direction == 7) {
					direction = 5;
				}
				else if (direction == 6) {
					direction = 8;
				}
			}
		}

		if (direction == 4 || direction == 6 || direction == 8)  // left
		{
			if (x > -1 + radius) {
				x -= speed;
			}
			else
			{
				if (direction == 4) {
					direction = 2;
				}
				else if (direction == 6) {
					direction = 5;
				}
				else if (direction == 8) {
					direction = 7;
				}
			}
		}
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


vector<Circle> world;
vector<Brick> dbricks;
vector<Brick> rbricks;


int main(void) {
	srand(time(NULL));

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(480, 480, "Random World of Purples", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	//Add bricks to arrays
	dbricks.push_back(Brick(DESTRUCTABLE, -1.0, -1.0, 0.2, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, -1.0, 1.0, 0.2, 1, 0.8, 1)); 
	dbricks.push_back(Brick(DESTRUCTABLE, 1.0, 1.0, 0.2, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, 1.0, -1.0, 0.2, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, 0, -1.0, 0.2, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, 0, 1.0, 0.2, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, 1.0, 0, 0.2, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, -1.0, 0, 0.2, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, -0.5, 0.5, 0.1, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, -0.5, -0.5, 0.1, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, 0.5, 0.5, 0.1, 1, 0.8, 1));
	dbricks.push_back(Brick(DESTRUCTABLE, 0.5, -0.5, 0.1, 1, 0.8, 1));
	rbricks.push_back(Brick(REFLECTIVE, 0.5, 0, 0.2, 1, 0, 1));
	rbricks.push_back(Brick(REFLECTIVE, -0.5, 0, 0.2, 1, 0, 1));
	rbricks.push_back(Brick(REFLECTIVE, 0.0, -0.5, 0.2, 1, 0, 1));
	rbricks.push_back(Brick(REFLECTIVE, 0, 0.5, 0.2, 1, 0, 1));

	while (!glfwWindowShouldClose(window)) {
		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);

		//Movement


		for (int i = 0; i < world.size(); i++)
		{
			for (int j = 0; j < dbricks.size(); j++) {
				if (dbricks[j].hits < 5) { // if this brick has not been destroyed
				world[i].CheckCollision(&dbricks[j]);
			}
			}
			for (int m = 0; m < rbricks.size(); m++) {
				world[i].CheckCollision(&rbricks[m]);
			}
			for (int n = 0; n < world.size(); n++) {
				if ( i != n) { // Do not check balls for collision with themselves
				world[i].CheckBCollision(&world[n]);
				}
			}
			world[i].MoveOneStep(); // Move and check for wall collisions to contain objects within world
			world[i].DrawCircle();
		}
		for (int j = 0; j < dbricks.size(); j++) {
			dbricks[j].drawBrick();
		}
		for (int m = 0; m < rbricks.size(); m++) {
			rbricks[m].drawBrick();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
		time_t now = time(NULL);
		time_t elapsed = static_cast<time_t>(difftime(now, gStartTime));
		struct tm ptm;
		gmtime_s(&ptm, &elapsed);
		if ((gFirstTime == true) || (ptm.tm_sec > gExpireSecs)) {
			//creates circles of varying shades of purple
			double r, g, b, d;
			r = rand() % 30 + 30;
			r = r / 100;
			g = 0.0;
			b = rand() % 50 + 30;
			b = b / 100;
			d = rand() % 8 + 1; // Shoot in random directions
			Circle B(0, 0, 02, d, 0.05, r, g, b);
			world.push_back(B);
			gStartTime = time(NULL);
			gFirstTime = false;
		}
	}
}