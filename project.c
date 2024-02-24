//Miguel Jose Maninang
//Lab 11: project.c
//Program:

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx.h"

#define MAX_SQUARE 10
#define MAX_POINTS 30

typedef struct
{
	float x;
	float y;
	float r;
	float dx;
	float dy;
} Shape;


typedef struct
{
	float x;
	float y;
	bool spawn;
} Point;

void startMenu();
char selection();
int playGame(Shape *ball, Shape square[], Point dots[], int squareCount, int height, int width);
void squareDraw(float x, float y, float r);
void points(Point arr[]);
void squareMovement(Shape square[], int height, int width, int squareCount);
void initializeSquare(Shape arr[], int squareCount);
void initializeDots(Point arr[]);
bool userLost(Shape *ball, Shape square[], int difficulty);
void gameOver(int score);



int main()
{
	//sets preliminary variables
	float width = 600, height = 600;
	char c;
	int  score, squareCount;
	bool keepgoing = true;

	Shape ball;	
	Shape square[MAX_SQUARE];	
	Point dots[MAX_POINTS]; 		

	//starting position

	gfx_open(width, height, "project");
	
	while(keepgoing)
	{
		//creates menu
		gfx_clear();
		startMenu();
		c = gfx_wait();	
		switch(c)
		{
			case '1': //starts game
				gfx_clear();
				squareCount = 2;
				score = playGame(&ball, square, dots, squareCount, height, width);
				gameOver(score);
				break;
			case '2':
				gfx_clear();
				squareCount = 4;
				score = playGame(&ball, square, dots, squareCount, height, width);
				gameOver(score);
				break;
			case '3':
				gfx_clear();
				squareCount = 6;
				score = playGame(&ball, square, dots, squareCount, height, width);
				gameOver(score);
				break;
		
			case '4'://quits game
				keepgoing = false;
				break;
			default:
				break;
		}
	}


return 0;
}


int playGame(Shape* ball, Shape square[], Point dots[], int squareCount, int height, int width)
{
	int score = 0;

	bool moveUp, moveDown, moveRight, moveLeft;
	bool go = true;
	int dx = 7, dy = 7;	

	//varaibles are the boundaries the ball can travel within
	int minX = ball->r;
	int maxX = width - ball->r;
	int minY = ball->r;
	int maxY = height- ball->r;
	

	//sets up structs
	initializeSquare(square, squareCount);
	initializeDots(dots);

		ball->x = width/2;
		ball->y = height/2;
		ball->r = 20;
	
		
	while(go)
	{	
		gfx_clear_color(0,0, 100);		
		gfx_clear();

		//prints necessar obojects on screen		
		gfx_color(255, 0, 255);	
		gfx_circle(ball->x, ball->y, ball->r);
	
		for (int i = 0; i < squareCount; i++)
			squareDraw(square[i].x, square[i].y, square[i].r);
	
		points(dots);	

		//prints score on screen
		char buff[MAX_POINTS];
		sprintf(buff, "Score: %d", score);
		gfx_text(550, 10, buff);	
		
		//controls square movmeents
		squareMovement(square, height, width, squareCount);

		//user movement and only moves if within the boundaries
		if (moveUp && (ball->y > minY)) ball->y -= dy;
		if (moveDown && (ball->y  < maxY))ball->y += dy;
		if (moveRight && (ball->x  < maxX))ball->x += dx;
		if (moveLeft && (ball->x  > minX))ball->x -= dx;



		if (gfx_event_waiting()) 
		{
			char c = gfx_wait();
			switch(c)
			{
			//movement in arrow keys for user/circle
				case 'w':
					moveUp = true;
					break;
				case 'a':
					moveLeft = true;
					break;
				case 's':
					moveDown = true;
					break;
				case 'd':
					moveRight = true;
					break;	
			//breaks loop and quits program
				case 'q':
					go = false;
					break;
				default:
					break;
			}	
		}
		else 
		{
			moveUp = false;
			moveDown= false;
			moveRight = false;
			moveLeft = false;
		}


		//checks if user has collided with a square
		go = userLost(ball, square, squareCount);
	
		for (int i = 0; i < MAX_POINTS; i ++)
			if ((ball->x- ball->r <= dots[i].x) && (ball->y + ball->r >= dots[i].y) && (ball->x + ball->r >= dots[i].x) && (ball->y -ball->r <= dots[i].y) && dots[i].spawn == true) //checks if the user has touched the point
			{
				dots[i].spawn = false;
				score++; //increments score
				ball->r = ball->r *1.1; //increments the size of the user
				break;
			}

		gfx_flush();
		usleep(10000);
	}
	return score;	
}


//creates the start menu
void startMenu()
{
	gfx_clear_color(25, 25, 112);
	gfx_clear();
	
	gfx_color(255, 255, 255);
	gfx_text(200, 300, "Welcome to Explosion!");
	gfx_text(200, 310, "Press 1 for EASY");
	gfx_text(200, 320, "Press 2 for MEDIUM ");
	gfx_text(200, 330, "Press 3 for HARD");
	gfx_text(200, 340, "Press 4 to quit ");
	
	
}


//creates boundaries along with movement for all the squares
void squareMovement(Shape square[], int height, int width, int squareCount)
{	
//logic from bouncing square
	for (int i =0; i < squareCount; i++)

		{
			square[i].x += square[i].dx;
			square[i].y += square[i].dy;
			if ((0 >= square[i].y - square[i].r) || (square[i].y+ square[i].r >= height))
				square[i].dy = - square[i].dy;

			if ((0 >=square[i].x - square[i].r) ||(square[i].x + square[i].r >=width)) 
				square[i].dx = - square[i].dx;
		}	


}


//creates square shape
void squareDraw(float x, float y, float r)
{
	gfx_color(255, 0, 0);
	gfx_line(x - r/2, y - r/2, x + r/2, y - r/2);
	gfx_line(x - r/2, y - r/2, x - r/2, y + r/2);
	gfx_line(x + r/2, y + r/2, x + r/2, y - r/2);
	gfx_line(x + r/2, y + r/2, x - r/2, y + r/2);
}

//gives squares initial poistions and velocity randomly
void initializeSquare(Shape square[], int squareCount)
{	

	for (int i = 0; i < squareCount; i++)
	{
		square[i].r = 20;
	

		square[i].x = rand() % 100;
		square[i].y = rand() % 100;
	
		while(square[i].x<= 21 || square[i].y <= 21) //ensures that the veolcity won't be changed to zero
		{
			square[i].x = rand() % 100; 
			square[i].y = rand() % 100;
		}
	 
		square[i].dx = rand() % 4 - 1;
		square[i].dy = rand() % 4 - 1;
		while(square[i].dx == 0 || square[i].dy == 0) //ensures that the veolcity won't be changed to zero
		{
			square[i].dx = rand() % 4 - 1; 
			square[i].dy = rand() % 4 - 1;
		}
	

	}
}


//creates a random position for the points 
void initializeDots(Point arr[])
{
	for(int i = 0; i < MAX_POINTS; i ++)
	{
		arr[i].x = rand()% 550;
		arr[i].y = rand()% 550;
		while(arr[i].x <= 10 || arr[i].y <=10)
		{
			arr[i].x = rand()% 550;
			arr[i].y = rand()% 550;	
		}
		arr[i].spawn = true;
	}

}

//creates points
void points(Point arr[])
{
	gfx_color(0, 255, 0);

	for( int i = 0; i < MAX_POINTS; i++)
		if(arr[i].spawn == true) //only spawns if its spawn variable is true
			gfx_point(arr[i].x, arr[i].y);

}

bool userLost(Shape *ball, Shape square[], int squareCount)
{
	//checks if the square and the ball have collided 
	for (int i = 0; i < squareCount; i++)
		if (( ball->x - ball->r <= square[i].x + square[i].r) && (square[i].y- square[i].r <= ball->y + ball->r) && (ball->x + ball->r >=square[i].x - square[i].r) && (square[i].y + square[i].r >= ball->y - ball->r )) 
			return false;

	return true;


}



//creates gameover screen and sends player back to home.
void gameOver(int score)
{
	gfx_clear();

	char scoreText[MAX_POINTS];
	sprintf(scoreText,"Your score: %d", score);
	while (1)
	{
		gfx_color(255, 255, 255);
		gfx_text(200, 300, scoreText);
		gfx_text(200, 310, "Press 1 to return.");

		char c = gfx_wait();
		if (c == '1') break; //returns user to main menu
	}

	gfx_clear();
}



 
