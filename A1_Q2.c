/**
 * Program written in C to simulate a simple Robot in OpenGL space.
 * 
 * @author Damian Angelone
 * @version March 06, 2017
 * @see <a href=https://github.com/TinkLyke/Robot>Original Project</a>
 *
 */

//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include<iostream>
#include<ctime>
using namespace std;

// Size of the screen for display
extern int MapSizeXY = 0;
// Robot's head position
extern int RHeadX = 0;
extern int RHeadY = 0;
// Robot's view direction
extern int RViewX = 0;
extern int RViewY = 0;
// Robot's head rotation 
extern int RHeadXC = 0;
extern int RHeadYC = 0;

/**
* The following will reset the state of the board back to it's initialized state.
*
* @see display()
*
*/
void reset(){
	
	RHeadX = RHeadXC;
	RHeadY = RHeadYC;
	RViewX = RHeadXC+1;
	RViewY = RHeadYC;
	glutPostRedisplay();
}

/**
* The following will check if the user's movement for the robot's view is valid while rotating, in accordance to the limitations of the board.
*
* @ para turn - Can either be 4 numbers, each representing the desired rotations of up, down, left, right.
* @ return canTurn - Bool (true/false), if the desired rotation is valid.
*
* @see display()
*/
bool EdgeCaseView(int turn)
{
	bool canTurn = true;

	if(turn == 0)
	{
		//turn left
		if(RViewX == 0 && RHeadX == 0 && (RViewY > RHeadY))
			canTurn = false;
		else if(RHeadY == 0 && RViewY == 0 && (RViewX< RHeadX))
			canTurn = false;
		else if(RViewX == MapSizeXY - 1 && RHeadX == MapSizeXY -1 && (RViewY<RHeadY))
			canTurn = false;
		else if(RViewY == MapSizeXY - 1 && RHeadY == MapSizeXY - 1 && (RViewX>RHeadX))
			canTurn = false;
	}
	else if(turn == 1)
	{
		//turn right
		if(RViewX == 0 && RHeadX == 0 && (RViewY < RHeadY))
			canTurn = false;
		else if(RHeadY == 0 && RViewY == 0 && (RViewX > RHeadX))
			canTurn = false;
		else if(RViewX == MapSizeXY - 1 && RHeadX == MapSizeXY -1 && (RViewY>RHeadY))
			canTurn = false;
		else if(RViewY == MapSizeXY - 1 && RHeadY == MapSizeXY - 1 && (RViewX<RHeadX))
			canTurn = false;
	}
	else if(turn == 2)
	{
		//turn up
		if(RHeadY == MapSizeXY - 1)
			canTurn = false;
	}
	else if(turn == 3)
	{
		//turn down
		if(RHeadY == 0)
			canTurn = false;
	}
	return canTurn;
}

/**
* The following will check if the user's movement for the robot's head is valid while translating, in accordance to the limitations of the board.
*
* @ para move - Can either be 4 numbers, each representing the desired directions of up, down, left, right.
* @ return canMove - Bool (true/false), if the desired movement is valid.
*
*/
bool EdgeCaseHead(int move)
{
	bool canMove = true;
	
	if(move == 0)
	{
		//to left
		if(RViewX == 0 || RHeadX == 0)
			canMove = false;
	}
	else if(move == 1)
	{
		//to right
		if(RViewX == MapSizeXY -1 || RHeadX == MapSizeXY - 1)
			canMove = false;
	}
	else if(move == 2)
	{
		//to up
		if(RViewY == MapSizeXY -1 || RHeadY == MapSizeXY - 1)
			canMove = false;
	}
	else if(move == 3)
	{
		//to up
		if(RViewY == 0 || RHeadY == 0)
			canMove = false;
	}
	return canMove;
} 

/**
* The following is a callback function for the keyboard that will either quit the pogram or reset the playerboard.
*
* @ para key - the key that the callback will use to choose it's action.
* @ para xIN - indicate the mouse location in window relative x-coordinates when the key was pressed. 
* @ para yIN - indicate the mouse location in window relative y-coordinates when the key was pressed.
*
*/
void keyboard(unsigned char key, int xIn, int yIn){
	
	switch(key){
		case 'Q':
		case 'q':
		case 27:
			exit(0);
			break;
		case 'r':
		case 'R':
			reset();
			break;
	}
}

/**
* The following will draw lines made up of single points throughout the player board.
*
* @ para x1 - acts as the x-coordinate of the start of the line. 
* @ para y1 - acts as the y-coordinate of the start of the line.
* @ para x2 - acts as the x-coordinate of the end of the line.
* @ para y2 - acts as the y-coordinate of the end of the line.
*
*/
void drawLine(int x1, int y1, int x2, int y2) 
{
	glPointSize(3.0f);
	
	glBegin(GL_POINTS);
		
		float XY = 1000.0f;

		float DeltaX = (x2 - x1) / XY;
		float DeltaY = (y2 - y1) / XY;

		for( int i = 0; i < XY; i++ )
		{
			glVertex2f( x1+(i*DeltaX), y1+(i*DeltaY) );
		} 

	glEnd();//end drawing of points
}

/**
* The following will draw squares made up of single points throughout the player board.
*
* @ para x1 - acts as the x-coordinate of bottom left corner of the square. 
* @ para y1 - acts as the y-coordinate of bottom left corner of the square.
* @ para x2 - acts as the x-coordinate of top right corner of the square.
* @ para y2 - acts as the y-coordinate of top right corner of the square.
*
*/
void drawSquare(float x1, float y1, float x2, float y2) 
{
	glPointSize(3.0f);
	
	glBegin(GL_POINTS);

		float XY = 100.0f;

		float DeltaX = (x2 - x1) / XY;
		float DeltaY = (y2 - y1) / XY;

		for( int i = 0; i < XY; i++ )
		{
			glVertex2f( x1+(i*DeltaX), y1+(i*DeltaY) );
		} 

	glEnd();//end drawing of points
}

/**
* The following will create the player board, which size varies based on user input.
*
* @ para row - amount of rows that make up the board (same as columns). 
* @ para column - amount of collumns that make up the board (same as rows).
*
* @see drawLine()
*
*/
void drawMap(int row, int column){
		
	glColor3f(0,1,0);	
	
	//draw rows, y is i, x is constant		
	for(int i = 0; i < row+1;i++){	
		drawLine(0,i,row,i);	
	}
	//draw column, x is j, y is constant
	for(int j = 0; j < column+1;j++){
		drawLine(j,0,j,column);	
	} 
}

/**
* The following will draw the robot's head on the screen. 
*
* @ para x - Used as the x-position for the head's location. 
* @ para y - Used as the y-position for the head's location.
*
*/
void RHead(int x, int y){
	
	glBegin(GL_POINTS);

		for(float i =(float) x; i < (float)x+1;i+=0.01){
			glColor3f(0,1,1);
			drawSquare(i,y,i,y+1);
		}

	glEnd();
}

/**
* The following will draw the robot's view on the screen. 
*
* @ para x - Used as the x-position for the view's location. 
* @ para y - Used as the y-position for the view's location.
*
* @see drawSquare()
*
*/
void RView(int x, int y){
	
	glBegin(GL_POINTS);

		for(float i =(float) x; i < (float)x+1;i+=0.01){
			glColor3f(1,1,0);
			drawSquare(i,y,i,y+1);
		}

	glEnd();
}

/** 
 * Acts as the display function (GLUT display callback function). Will clear the screen, draw a square, and display it.
 *
 * @see drawMap()
 * @see RHead()
 * @see RView()
 *
 */
void display(void)
{	
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawMap(MapSizeXY,MapSizeXY);
	RHead(RHeadX,RHeadY);
	RView(RViewX,RViewY);
	glEnd();
	glFlush();
}

/**
* The following will check the desired translation of the robot based on user input, then move the robot.
*
* @ para value - Can either be 4 numbers representing the 4 directions they robot can move.
*
* @see display()
* @see EdgeCaseHead()
*
*/
void movemenuProc(int Value)
{
	//Move left	
	if(Value == 0)
	{
		if(EdgeCaseHead(Value)){
			RHeadX -= 1;
			RViewX -= 1;
			glutPostRedisplay();
		}
	}
	//Move right
	else if(Value == 1) 
	{
		if(EdgeCaseHead(Value)){
			RHeadX += 1;
			RViewX += 1;
			glutPostRedisplay();
		}
	}
	//Move up
	else if(Value == 2) 
	{
		if(EdgeCaseHead(Value)){
			RHeadY += 1;
			RViewY += 1;
			glutPostRedisplay();
		}
	}
	//Move down
	else if(Value == 3) 
	{
		if(EdgeCaseHead(Value)){
			RHeadY -= 1;
			RViewY -= 1;
			glutPostRedisplay();
		}
	}
}

/**
* The following will check the desired rotation of the robot based on user input, then rotate the robot.
*
* @ para value - Can either be 4 numbers representing the 4 directions they robot can rotate.
*
* @see display()
* @see EdgeCaseView()
*
*/
void turnmenuProc(int Value)
{
	//Turn left	
	if(Value == 0 &&  EdgeCaseView(0))
	{
		if(RViewX == RHeadX && RViewY > RHeadY){
			RViewX -= 1;
			RViewY -= 1;
		}
		else if(RViewX == RHeadX && RViewY < RHeadY){
			RViewX +=1;
			RViewY +=1;		
		}
		else if(RViewX > RHeadX && RViewY == RHeadY){
			RViewX -=1;
			RViewY +=1;
		}
		else if(RViewX < RHeadX && RViewY == RHeadY){
			RViewX +=1;
			RViewY -=1;
		}
	}
	//Turn right
	else if(Value == 1 && EdgeCaseView(1))
	{
		if(RViewX == RHeadX && RViewY > RHeadY){
			RViewX += 1;
			RViewY -= 1;
		}
		else if(RViewX == RHeadX && RViewY < RHeadY){
			RViewX -=1;
			RViewY +=1;		
		}
		else if(RViewX > RHeadX && RViewY == RHeadY){
			RViewX -=1;
			RViewY -=1;
		}
		else if(RViewX < RHeadX && RViewY == RHeadY){
			RViewX +=1;
			RViewY +=1;
		}			
	}
	//Turn up
	else if(Value == 2 && EdgeCaseView(2))
	{
		RViewX = RHeadX;
		RViewY = RHeadY + 1;
	}
	//Turn down
	else if(Value == 3 && EdgeCaseView(3)){
		RViewX = RHeadX;
		RViewY = RHeadY - 1; 
	}
	glutPostRedisplay();
}

/** 
* The main function - the program's entry point.
*
*/
int main(int argc, char** argv)
{		
	//User initialization.
	printf("Enter the dimensions of the map: ");
	scanf("%i",&MapSizeXY);	

	//Initialized values.
	srand(time(NULL));
	RHeadX = rand()%(int)(MapSizeXY - 1);
	RHeadY = rand()%(int)(MapSizeXY - 1);
	RHeadXC = RHeadX;
	RHeadYC = RHeadY;
	RViewX = RHeadX + 1;
	RViewY = RHeadY;

	//User prompts.
	printf("Press 'q' to quit the app.\n");
	printf("Press 'r' to reset the position of robot.\n");
	
	//OpenGL initializations.
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);	
	glutCreateWindow("Robot");	//creates the window
	gluOrtho2D(-1,MapSizeXY+1,-1,MapSizeXY+1); //change the origin of the map

	//Callback functions
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	
	//Sub menu 1
	int MoveMenu = glutCreateMenu(movemenuProc);
	glutAddMenuEntry("Left",0);
	glutAddMenuEntry("Right",1);
	glutAddMenuEntry("Up",2);
	glutAddMenuEntry("Down",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Sub menu 2
	int TurnMenu = glutCreateMenu(turnmenuProc);
	glutAddMenuEntry("Left",0);
	glutAddMenuEntry("Right",1);
	glutAddMenuEntry("Up",2);
	glutAddMenuEntry("Down",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	
	
	//Main menu
	int MainMenu = glutCreateMenu(menuProc);
	glutAddSubMenu("Move",MoveMenu);
	glutAddSubMenu("Turn",TurnMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	
		
	//Loops main, exitting when necessary.	
	glutMainLoop(); //starts the event loop
	return(0); //return may not be necessary on all compilers
}