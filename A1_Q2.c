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
/* display function - GLUT display callback function
 *		clears the screen, draws a square, and displays it
 */
extern int MapSizeXY = 0;
//Robot's head
extern int RHeadX = 0;
extern int RHeadY = 0;
//Robot's facing direction
extern int RViewX = 0;
extern int RViewY = 0;
//Robot's Head
extern int RHeadXC = 0;
extern int RHeadYC = 0;



//reset the position of robot
void reset(){
	RHeadX = RHeadXC;
	RHeadY = RHeadYC;
	RViewX = RHeadXC+1;
	RViewY = RHeadYC;
	glutPostRedisplay();

}

//Check if the robot is at the boarder, avoid its facing direction be out of the map
bool EdgeCaseView(int turn){
	bool canTurn = true;
	if(turn == 0){
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
	else if(turn == 1){
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
	else if(turn == 2){
		//turn up
		if(RHeadY == MapSizeXY - 1)
			canTurn = false;
	}
	else if(turn == 3){
		//turn down
		if(RHeadY == 0)
			canTurn = false;
	}
	return canTurn;


}

//Check if the robot is trying to move out of the map
bool EdgeCaseHead(int move){
	bool canMove = true;
	if(move == 0){
		//to left
		if(RViewX == 0 || RHeadX == 0)
			canMove = false;
	}
	else if(move == 1){
		//to right
		if(RViewX == MapSizeXY -1 || RHeadX == MapSizeXY - 1)
			canMove = false;
	}
	else if(move == 2){
		//to up
		if(RViewY == MapSizeXY -1 || RHeadY == MapSizeXY - 1)
			canMove = false;
	}
	else if(move == 3){
		//to up
		if(RViewY == 0 || RHeadY == 0)
			canMove = false;
	}
	return canMove;

}
//keyboard quit and reset 
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

//draw a line
void drawLine(int x1, int y1, int x2, int y2) {
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

//draw a square
void drawSquare(float x1, float y1, float x2, float y2) {
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

//draw the map
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

//Robot's head
void RHead(int x, int y){
	
	glBegin(GL_POINTS);

	for(float i =(float) x; i < (float)x+1;i+=0.01){
		glColor3f(0,1,1);
		drawSquare(i,y,i,y+1);
	}
	glEnd();
}
//Robot's view
void RView(int x, int y){
	
	glBegin(GL_POINTS);

	for(float i =(float) x; i < (float)x+1;i+=0.01){
		glColor3f(1,1,0);
		//FIX HERE, CANT USE INT
		drawSquare(i,y,i,y+1);
	}
	glEnd();
}

//display the windows and the content
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


void menuProc(int){
	

}
//Move submenu
void movemenuProc(int Value){
	//Move left	
	if(Value == 0){
		if(EdgeCaseHead(Value)){
			RHeadX -= 1;
			RViewX -= 1;
			glutPostRedisplay();
		}
	}
	//Move right
	else if(Value == 1) {
		if(EdgeCaseHead(Value)){
			RHeadX += 1;
			RViewX += 1;
			glutPostRedisplay();
		}
	}
	//Move up
	else if(Value == 2) {
		if(EdgeCaseHead(Value)){
			RHeadY += 1;
			RViewY += 1;
			glutPostRedisplay();
		}
	}
	//Move down
	else if(Value == 3) {
		if(EdgeCaseHead(Value)){
			RHeadY -= 1;
			RViewY -= 1;
			glutPostRedisplay();
		}
	}

}

//Turn submenu
void turnmenuProc(int Value){
	//Turn left	
	if(Value == 0 &&  EdgeCaseView(0)){
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
	else if(Value == 1 && EdgeCaseView(1)){
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
	else if(Value == 2 && EdgeCaseView(2)){
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


/* main function - program entry point */
int main(int argc, char** argv)
{		
	printf("enter the dimension of the map");
	scanf("%i",&MapSizeXY);	
	

	srand(time(NULL));
	RHeadX = rand()%(int)(MapSizeXY - 1);
	RHeadY = rand()%(int)(MapSizeXY - 1);
	RHeadXC = RHeadX;
	RHeadYC = RHeadY;
	RViewX = RHeadX + 1;
	RViewY = RHeadY;
	printf("press q to quit the app\n");
	printf("press r to reset the position of robot\n");
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
		
	
	glutCreateWindow("Robot");	//creates the window
	gluOrtho2D(-1,MapSizeXY+1,-1,MapSizeXY+1); //change the origin of the map
	glutDisplayFunc(display);	//registers "display" as the display callback function



	glutKeyboardFunc(keyboard);
	
	//submenu
	int MoveMenu = glutCreateMenu(movemenuProc);
	glutAddMenuEntry("Left",0);
	glutAddMenuEntry("Right",1);
	glutAddMenuEntry("Up",2);
	glutAddMenuEntry("Down",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//submenu
	int TurnMenu = glutCreateMenu(turnmenuProc);
	glutAddMenuEntry("Left",0);
	glutAddMenuEntry("Right",1);
	glutAddMenuEntry("Up",2);
	glutAddMenuEntry("Down",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	
	
	int MainMenu = glutCreateMenu(menuProc);
	glutAddSubMenu("Move",MoveMenu);
	glutAddSubMenu("Turn",TurnMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	
		
	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}