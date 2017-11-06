#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <math.h>
#include "func.h"
#include <time.h>
#include "image.h"
using namespace std;

float x = 6.0f;
float y = 1.5f;
float z = 6.0f;
GLuint texid=0;
int windowWidth = 700;
int windowHeight = 700;
const char* texture_filename = "wildtextures-old-wood-original-file.bmp";

GLfloat sideLength = 0.5;

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Jacob's OpenGL project");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	initTexture();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
	return 0;

}

void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
	
}
void drawAllNumbers()
{

		for(int i=0; i<10; i++)
		{
			glPushMatrix();
			glTranslatef(-3.0f + i, i%2==0? 3.0f : 1.5f, (float)-i);

			glPushMatrix();
			glTranslatef(-3.0f, 3.0f, 0.0f);
			glRotatef(45.0, 0.0, 1.0, 0.0);
			drawNumber(i);
			glPopMatrix();
			glPopMatrix();
		}
}


void display()
{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the color buffer
		glLoadIdentity();
		glPushMatrix();
			//glRotatef(0, 1.0, 0, 0);
		//glRotatef(0, 0, 1.0, 0);
		gluLookAt(x, y, z,				// object point 
					0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f); // viewing point	
		//glRotatef(0, 1.0, 0.0, 0.0);
		//glRotatef(0, 0.0, 1.0, 0.0);
		
		drawAllNumbers();


	
		
		// First cube
		glPushMatrix();
		glTranslatef(-3.0f, -3.0f, 0.0f);
		glRotatef(0, 0, 1.0, 0);
		drawCube(1.0f);
		drawCubeEdges(1.0f, 0.3, 1.0, .8);
		drawLetterOnCube(1, 'D');
		//drawNumOnCube(1,1);
		drawNumOnCube(2, 2);
		drawNumOnCube(3, 3);
		drawNumOnCube(4, 2);
		drawNumOnCube(5, 3);
		drawNumOnCube(6, 3);
		glPopMatrix();

		// second cube

		glPushMatrix();
		glScalef(0.5f, 0.5f, 0.5f);
		drawSolidCube(1.0f, 0.0f, 0.0f, 1.0f);;
		drawCubeEdges(1.0f, 1.0, 0.2, 0.3);
		drawLetterOnCube(1, 'A');
		drawLetterOnCube(2, 'C');
		//drawNumOnCube(2, 3);
		drawNumOnCube(3, 3);
		drawNumOnCube(4, 3);
		drawLetterOnCube(5, 'C');
		drawNumOnCube(6, 3);
		glPopMatrix();
		

			// third cube
		glPushMatrix();
		glRotatef(0, 0.0, 0, 2);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(5.0f, 0.0f, 0.0f);
		drawSolidCube(1.0f, 1.0f, 0.0f, 0.0f);;
		drawLetterOnCube(1, 'C');
		drawNumOnCube(2, 2);
		drawLetterOnCube(3, 'A');
		drawNumOnCube(4, 2);
		drawLetterOnCube(5, 'D');
		drawLetterOnCube(6, 'A');
		drawCubeEdges(1.0f, 0.2, 0.0, 0.0);
		glPopMatrix();
		
	// fourth cube
		glPushMatrix();
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(1.0f, 1.0f, 7.0f);
		glRotatef(0, 0, 1.0, 0);
		drawSolidCube(1.0f, 1.0f, 1.0f, 0.0f);;
		drawCubeEdges(1.0f, 0.0, 0.2, 0.3f);
		drawNumOnCube(1, 1);
		drawNumOnCube(2, 2);
		drawNumOnCube(3, 3);
		drawNumOnCube(4, 2);
		drawNumOnCube(5, 3);
		drawNumOnCube(6, 3);
		glPopMatrix();
		// fifth cube
		glPushMatrix();
		glScalef(0.3f, 0.3f, 0.3f);
		glTranslatef(6.0f, -7.0f, 5.0f);
		drawSolidCube(1.0f, 0.0f, 0.3f, 0.0f);;
		drawCubeEdges(1.0f, 0.0, 0.2, 0.3);
		drawNumOnCube(1, 2);
		drawLetterOnCube(2, 'A');
		drawNumOnCube(3, 3);
		drawNumOnCube(4, 1);
		drawLetterOnCube(5, 'A');
		drawNumOnCube(6, 3);
		glPopMatrix();

		
		



		glPopMatrix();
		
		glFlush();
		glutSwapBuffers();
}
/*
void drawCubeOutline(GLfloat cSide, GLfloat Rout, GLfloat Gout, GLfloat Bout, 
								GLfloat Rin,GLfloat Gin, GLfloat Bin)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);
	glColor3f(Rout, Gout, Bout);
	drawCube(cSide);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(Rin, Gin, Bin);

}
*/


void drawCubeEdges(float side, float r, float g, float b)
{
	float t = side/2 + 0.5;
	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);
	drawTextureCube(2.0f);
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(0.0f, t, t);
	glScalef(1.1f, 0.1f,.1f);
	glColor3f(0.0,0.0,1.0);
	drawSolidCube(1.0f, r,g,b);
	glPopMatrix();
	glPushMatrix();
	glScalef(0.5f,0.5f,0.5f);
	drawTextureCube(2.0f);
	glPopMatrix();

// top front edge
glPushMatrix();
glTranslatef(0.0f, t, t);
glScalef(1.1f, 0.1f, 0.1f);
glColor3f(0.0, 0.0, 1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

// bottom front edge
glPushMatrix();
glTranslatef(0.0f, -t, t);
glScalef(1.1f, 0.1f, 0.1f);
glColor3f(0.0, 0.0, 1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

// bottom back edge
glPushMatrix();
glTranslatef(0.0f, -t, -t);
glScalef(1.1f, 0.1f, 0.1f);
glColor3f(0.0, 0.0, 1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

// top back edge
glPushMatrix();
glTranslatef(0.0f, t, -t);
glScalef(1.1f, 0.1f, 0.1f);
glColor3f(0.0, 0.0, 1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

// left front edge
glPushMatrix();
glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
glTranslatef(0.0f, t, t);
glScalef(1.1f, 0.1f, 0.1f);
glColor3f(0.0, 0.0, 1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();


//left back edge
glPushMatrix();
glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
glTranslatef(0.0f, t, -t);
glScalef(1.1f, .1f, .1f);
glColor3f(0.0,0.0,1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

//right back edge
glPushMatrix();
glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
glTranslatef(0.0f, t, -t);
glScalef(1.1f, .1f, 0.1f);
glColor3f(0.0,0.0,1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

//right front edge
glPushMatrix();
glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
glTranslatef(0.0f, t, t);
glScalef(1.1f, .1f, 0.1f);
glColor3f(0.0,0.0,1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

//left side top edge
glPushMatrix();
glTranslatef(-t, t, 0.0f);
glScalef(.1f, .1f, 1.1f);
glColor3f(0.0,0.0,1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

//left side bottom edge
glPushMatrix();
glTranslatef(-t, -t, 0.0f);
glScalef(.1f, .1f, 1.1f);
glColor3f(0.0,0.0,1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

//right side top edge
glPushMatrix();
glTranslatef(t, t, 0.0f);
glScalef(.1f, .1f, 1.1f);
glColor3f(0.0,0.0,1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();

//rightside bottom edge
glPushMatrix();
glTranslatef(t, -t, 0.0f);
glScalef(.1f, .1f, 1.1f);
glColor3f(0.0,0.0,1.0);
drawSolidCube(1.0f, r, g, b);
glPopMatrix();
}

void drawSolidCube(GLfloat side, float r, float g, float b)
{
		glEnable(GL_NORMALIZE);

		glPushMatrix();
		// Front
		glBegin(GL_POLYGON);
			glNormal3f(0, -1, 0);
			//glColor3f(r,g,b); 
			glVertex3f(side, -side, side);    // x, y
			glVertex3f(side,  side, side);
			glVertex3f( -side, side, side);
			glVertex3f( -side,  -side, side);
		glEnd();


		// Back
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, -1);
			//glColor3f(r,g,b); 
			glVertex3f(  side, -side, -side );
			glVertex3f(  side,  side, -side );
			glVertex3f( -side,  side, -side );
			glVertex3f( -side, -side, -side );
		glEnd();
	
		
		// Purple side - RIGHT
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
			//glColor3f(r,g,b); 
		glVertex3f( -side, -side, side );
		glVertex3f( -side, -side,  -side );
		glVertex3f( side, -side,  -side);
		glVertex3f( side, -side, side );
		glEnd();

		// Green side - LEFT
		glBegin(GL_POLYGON);
		glNormal3f(-1, 0, 0);
			//glColor3f(r,g,b); 
		glVertex3f( -side, -side,  side );
		glVertex3f( -side,  -side,  -side );
		glVertex3f( -side,  side, -side );
		glVertex3f( -side, side, side );
		glEnd();

		// Blue side - TOP
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
			//glColor3f(r,g,b); 
		glVertex3f(  -side,  side,  side );
		glVertex3f(  -side,  side, -side );
		glVertex3f( side,  side, -side );
		glVertex3f( side,  side,  side );
		glEnd();
		

		// Red side - BOTTOM
		glBegin(GL_POLYGON);
			glNormal3f(1, 0, 0);
			//glColor3f(r,g,b); 
			glVertex3f( side, -side, side );
			glVertex3f( side,  -side, -side );
			glVertex3f( side,  side,  -side );
			glVertex3f( side, side,  side );
		glEnd();
glPopMatrix();


}

void init()
{
	glShadeModel(GL_SMOOTH);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

}


void drawCube(GLfloat side)
{
		glEnable(GL_NORMALIZE);
		glPushMatrix();

		// Front -- Blue
		glBegin(GL_POLYGON);
			glNormal3f(0, -1, 0);
			glColor3f(   0.0,  1.0,  1.0 );
			glVertex3f(side, -side, side);    // x, y
			glVertex3f(side,  side, side);
			glVertex3f( -side, side, side);
			glVertex3f( -side,  -side, side);
		glEnd();


		// Back
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, -1);
			glColor3f(   1.0,  1.0, 0 );
			glVertex3f(  side, -side, -side );
			glVertex3f(  side,  side, -side );
			glVertex3f( -side,  side, -side );
			glVertex3f( -side, -side, -side );
		glEnd();
	
		
		// Purple side - RIGHT
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glColor3f(  1.0,  0.0,  1.0 );
		glVertex3f( -side, -side, side );
		glVertex3f( -side, -side,  -side );
		glVertex3f( side, -side,  -side);
		glVertex3f( side, -side, side );
		glEnd();

		// Green side - LEFT
		glBegin(GL_POLYGON);
		glNormal3f(-1, 0, 0);
		glColor3f(   0.0,  1.0,  0.0 );
		glVertex3f( -side, -side,  side );
		glVertex3f( -side,  -side,  -side );
		glVertex3f( -side,  side, -side );
		glVertex3f( -side, side, side );
		glEnd();

		// Yellow side - TOP
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
			glColor3f(1.0, 1, 0.0);
		glVertex3f(  -side,  side,  side );
		glVertex3f(  -side,  side, -side );
		glVertex3f( side,  side, -side );
		glVertex3f( side,  side,  side );
		glEnd();
		

		// Red side - BOTTOM
		glBegin(GL_POLYGON);
			glNormal3f(1, 0, 0);
			glColor3f( 1.0,  0.0,  0.0 );
			glVertex3f( side, -side, side );
			glVertex3f( side,  -side, -side );
			glVertex3f( side,  side,  -side );
			glVertex3f( side, side,  side );
		glEnd();
glPopMatrix();

}

void drawNumber(int number)
{
	
		glPushMatrix();
	 number%2==0 ? glColor3f(1.0f, 0.0f, 0.0f) : 
								glColor3f(0.0f, 0.0f, 1.0f);
	switch(number)
	{
	  case 0:
				drawZero();
				break;
		case 1:
				drawOne();
				break;
		case 2:
				drawTwo();
				break;
		case 3:
				drawThree();
				break;
		case 4:
				drawFour();
				break;
		case 5:
				drawFive();
				break;
		case 6:
				drawSix();
				break;
		case 7:
				drawSeven();
				break;
		case 8:
				drawEight();
				break;
		case 9:
				drawNine();
				break;
		default:
				break;
	}
	glPopMatrix();

}

void drawLetter(char letter)
{
	switch(letter)
	{
		case 'A':
			drawA();
						break;
	 case 'B':
			drawB();
			break;
	 case 'C':
			drawC();
			break;
		case 'D':
			drawD();
			break;
	
	}

}

void drawA()
{
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
			glTranslatef(-.19f, 0.0f, 0.0f);
			glRotatef(75.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.66f, .1f, 0.1f);
			drawSolidCube(1.0f, 0.0f, 1.0f, 1.0f);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(0.19f, 0.03f, 0.0f);
			glRotatef(-147.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(75.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.7f, 0.1f, 0.1f);
			drawSolidCube(1.0f, 0.0f, 1.0f, 1.0f);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(0.0f, -0.1f, 0.0f);
			glScalef(0.3f, 0.1f, 0.0f);
			drawSolidCube(1.0f, 0.0f, 1.0f, 1.0f);
			glPopMatrix();
			

}

void drawB()
{
}

void drawC()
{
  glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	glScalef(1.0f, 1.2f, 1.0f);
	glRotatef(45.0f, 0.0f, 0.0f, 2.0f);
	glTranslatef(0.0f, 0.0f, 0.1f);
	drawRing(270.0f, 0.1f, 0.5, .25);
	glPopMatrix();

}

void drawD()
{
	glColor3f(1.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(-0.1f, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(-0.2f, 0.0f, 0.0f);
	glScalef(0.1f, 0.65f, 0.1f);
	drawSolidCube(1.0f,1.0f, 1.0f, 0.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.1f, 0.546, 0.0f);
	glScalef(0.2f, 0.1f, 0.1f);
	drawSolidCube(1.0f,1.0f, 1.0f, 0.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.1f, -0.554, 0.0f);
	glScalef(0.2f, 0.1f, 0.1f);
	drawSolidCube(1.0f,1.0f, 1.0f, 0.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.01f, 0.1f, 0.1f);
	glScalef(0.7f, 0.7f, 2.0f);
	drawRing(180.0f, 0.1f, 0.93, 0.64);
	glPopMatrix();
	glPopMatrix();




}

void configureSide(int side)
{
	switch(side)
	{
		case 1:
			glTranslatef(0.0f, 0.0f, 1.05f);
			break;
		case 2:
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			break;
		case 3:
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, 1.05f);
			break;
		case 4:
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, 1.05f);
			break;
		case 5:
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, 1.05f);
			break;
		case 6:
			glRotatef(90.0, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, 1.05f);
			break;
		 default:
				cout << "INVALID INPUT: ONLY SIX SIDES ON A CUBE, IDIOT!" << endl;
				break;
	}
	



}
void drawNumOnCube(int side, int number)
{
	glPushMatrix();
	configureSide(side);
	drawNumber(number);
	glPopMatrix();
}

void drawLetterOnCube(int side, char letter)
{
	glPushMatrix();
	configureSide(side);
	drawLetter(letter);
	glPopMatrix();
}

void drawZero()
{
	glPushMatrix();
	glScalef(0.9f, 1.0f, 0.002f);
	drawRingOutLine(360.0f, 100.0f, 0.6, .4, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();
}

void drawOne()
{
	glPushMatrix();
	glScalef(0.1f, 0.65f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(2 * 0.07f, 2 * 0.21f, 0.0f);
	glScalef(0.24f, 0.1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 2 * -.27f, 0.0f);
	glScalef(0.3f, 0.1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();
}

void drawTwo()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.274f, 0.1f);
	glRotatef(0.0f,0.0f,0.0f,1.0f);
	glScalef(0.6f, 0.6f, 2.0f);
	drawRing(200.0f, 0.1f, 0.632, .29);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2 * 0.02f, -.1f, 0.0f);
	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
	glScalef(0.5f, 0.1f, 0.1f);
	drawSolidCube(1.0f,0.0f, 0.0f, 1.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -0.550f, 0.0f);
	glScalef(0.29f, 0.1f, 0.1f);
	drawSolidCube(1.0f, 0.0f, 0.0f, 1.0f);
	glPopMatrix();
}
void drawThree()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.274f, 0.2f);
	glRotatef(-100.0f, 0.0f, 0.0f, 1.0f);
	glScalef(.6f,.6f, 2.0f);
	drawRing(270.0f, 0.1f, 0.632, .29);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, -.274f, 0.2f);
	glRotatef(-170.0f, 0.0f, 0.0f, 1.0f);
	glScalef(.6f, .6f, 2.0f);
	drawRing(270.0f, 0.1f, .632, .29);
	glPopMatrix();
}

void drawFour()
{
	glPushMatrix();
	glTranslatef(2*0.1f, 0.0f, 0.0f);
	glScalef(0.1f, .702f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-.1f, 0.02f, 0.0f);
	glScalef(0.28f, 0.1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(50.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.21f, 0.26f, 0.0f);
	glScalef(0.36f, 0.1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
	glPopMatrix();

	
}
void drawFive()
{
	glPushMatrix();
glTranslatef(2 * 0.01f, 2 * 0.273f, 0.0f);
glScalef(.33f, .1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(2 * -0.1f, 2 * 0.128f, 0.0f);
glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
glScalef(.35f, .1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(0.0f, 2 * -.137f, 0.1f);
glRotatef(-170.0f, 0.0f, 0.0f, 1.0f);
glScalef(.66f, .66f, 2.0f);
drawRing(290.0f, .1f, .632, .29);
glPopMatrix();

}

void drawSix()
{
     glPushMatrix();
glTranslatef(0.0f, 2 * .137f, 0.0f);
glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
glScalef(.6f, .6f, 2.0f);
drawRing(160.0f, .1f, .632, .29);
glPopMatrix();
glPushMatrix();
glTranslatef(0.0f, 2 * -.137f, 0.0f);
glScalef(.6f, .6f, 2.0f);
drawRing(360.0f, .1f, .632, .29);
glPopMatrix();
glPushMatrix();
glTranslatef(2 * -0.14f, 0.0f, -0.1f);
glScalef(.1f, .3f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
glPopMatrix();

}

void drawEight()
{
glPushMatrix();
 glTranslatef(0.0f, 2 * .137f, 0.0f);
 glScalef(.6f, .6f, 2.0f);
 drawRing(360.0f, .1f, .632, .29);
 glPopMatrix();
 glPushMatrix();
 glTranslatef(0.0f, 2 * -.137f, 0.0f);
 glScalef(.6f, .6f, 2.0f);
 drawRing(360.0f, .1f, .632, .29);
 glPopMatrix();
	
}
void drawSeven()
{
	glPushMatrix();
glTranslatef(2 * 0.03f, 2 * -0.05f, 0.0f);
glRotatef(68.0f, 0.0f, 0.0f, 1.0f);
glScalef(.6f, .1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
glPopMatrix();
glPushMatrix();
glTranslatef(0.0f, 2 * 0.25f, 0.0f);
glScalef(.38f, .1f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
glPopMatrix();

}
void drawNine()
{
	glPushMatrix();
glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
glPushMatrix();
glTranslatef(0.0f, 2 * .137f, 0.1f);
glRotatef(70.0f, 0.0f, 0.0f, 1.0f);
glScalef(.6f, .6f, 2.0f);
drawRing(120.0f, .1f, .632, .29);
glPopMatrix();
glPushMatrix();
glTranslatef(0.0f, 2 * -.137f, 0.1f);
glScalef(.6f, .6f, 2.0f);
drawRing(360.0f, .1f, .632, .29);
glPopMatrix();
glPushMatrix();
glTranslatef(2 * -0.14f, 0.0f, 0.0f);
glScalef(.1f, .3f, 0.1f);
	drawSolidCube(1.0f, 1.0f, 0.0f, 1.0f);
glPopMatrix();
glPopMatrix();

}

void drawRingOutLine(GLfloat angle, GLfloat depth, GLfloat oRadius, GLfloat iRadius, GLfloat Rout, GLfloat Gout, GLfloat Bout, GLfloat Rin, GLfloat Gin, GLfloat Bin)
{
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);
	//glColor3f(Rout, Gout, Bout);
	drawRing(angle, depth, oRadius, iRadius);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glColor3f(Rin, Gin, Bin);
	drawRing(angle,depth,oRadius, iRadius);
	glPopMatrix();
}

void drawRing(GLfloat angle, GLfloat depth, GLfloat oRadius, GLfloat iRadius)
{
	 glEnable(GL_NORMALIZE);
	 glPushMatrix();
	 drawSemiCircle(angle, oRadius, iRadius);
	 drawDepthOfCircle(angle, depth, oRadius);
	 drawDepthOfCircle(angle, depth, iRadius);
	 // square at end of semi-circle
	 glBegin(GL_POLYGON);
	 	glVertex3f(oRadius, 0.0f, 0.0f);
	 	glVertex3f(oRadius, 0.0f,-depth);
	 	glVertex3f(iRadius, 0.0f,-depth);
	 	glVertex3f(iRadius, 0.0f,0.0f);
	 glEnd();
	 glPushMatrix();
	 glRotatef(angle*3.14 / 180, 0.0f, 0.0f, 1.0f);
	 glBegin(GL_POLYGON);
	 	glVertex3f(oRadius, 0.0f, 0.0f);
	 	glVertex3f(oRadius, 0.0f, -depth);
	 	glVertex3f(iRadius, 0.0f, -depth);
	 	glVertex3f(iRadius, 0.0f, 0.0f);
	 glEnd();
	 glPopMatrix();
	 // back of circle
	 glTranslatef(0.0f, 0.0f, -depth);
	 drawFlipSemiCircle(angle, oRadius, iRadius);
	 glPopMatrix();

}

void drawSemiCircle(GLfloat angle, GLfloat oRadius, GLfloat iRadius)
{
	glEnable(GL_NORMALIZE);
	angle = angle * 3.14 / 180;
	GLint numSegments = 30;
	angle = angle/numSegments;
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, 1);
	for(int i = 0; i < numSegments; i++)
	{
		glVertex3f(oRadius*cos(i*angle), oRadius*sin(i*angle), 0.0f);
		glVertex3f(iRadius*cos(i*angle), iRadius*sin(i*angle), 0.0f);
	}
	glEnd();

}

void drawFlipSemiCircle(GLfloat angle, GLfloat oRadius, GLfloat iRadius)
{
				glEnable(GL_NORMALIZE);
					angle = angle * 3.14 / 180;
					GLint numSegments = 30;
					angle = angle/numSegments;
					glBegin(GL_TRIANGLE_STRIP);
					glNormal3f(0, 0, -1);
					for(int i = 0; i < numSegments; i++)
					{
						glVertex3f(oRadius*cos(i*angle), oRadius*sin(i*angle), 0.0f);
						glVertex3f(iRadius*cos(i*angle), iRadius*sin(i*angle), 0.0f);
					}
					glEnd();



				}

				void drawDepthOfCircle(GLfloat angle, GLfloat depth, GLfloat oRadius)
				{
					glEnable(GL_NORMALIZE);
	angle =angle * 3.14 / 180;
	GLint numSegments = 30;
	angle = angle / numSegments;
	glBegin(GL_TRIANGLE_STRIP);
	GLfloat u1, u2, u3, v1, v2, v3;
	for(int i = 0; i <= numSegments; i++)
	{
		u1 = oRadius * cos(i*angle);
		u2 = oRadius * sin(i*angle);
		u3 = 0.0f;
		v1 = oRadius*cos(i*angle);
		v2 = oRadius*sin(i*angle);
		v3 = -depth;
		glNormal3f(cos(i*angle), sin(i*angle), 0);
		glVertex3f(u1,u2,u3);
		glVertex3f(v1,v2,v3);
	}
	glEnd();
}

void initTexture()
{
// read in texture image
Image *image = new Image();
image->read(texture_filename);
if (!image->good())
{
fprintf(stderr, "ERROR: couldn't load %s\n", texture_filename);

fgetc(stdin);
exit(1);
}

// generate texture handle
glGenTextures(1, &texid);

// set current texture to our handle
glBindTexture(GL_TEXTURE_2D, texid);

// set the magnification / minification filter to linear sampling
// (instead of GL_NEAREST for point sampling)
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

// set the texture coordinate wrap mode to repeat (instead of GL_CLAMP)
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// pixels will be packed on the byte
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// load image data into texture object
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
image->getWidth(), image->getHeight(), 0,
GL_RGB, GL_UNSIGNED_BYTE, image->getPixels());

// set current texture to none
glBindTexture(GL_TEXTURE_2D, 0);

// free the image memory - no need to store it anymore
delete image;

}
void drawTextureCube(GLfloat side)
{
glEnable(GL_NORMALIZE);
glColor3f(1, 1, 1);
glBindTexture(GL_TEXTURE_2D, texid);
glEnable(GL_TEXTURE_2D);


GLfloat halfSide = side;

 glPushMatrix();
 
glBegin(GL_POLYGON); // Green/Blue side - FRONT
glNormal3f(0, 0, 1);
glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSide, -1 * halfSide, halfSide);
glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSide, halfSide, halfSide);
glTexCoord2f(0.0f, 0.0f); glVertex3f(-1 * halfSide, halfSide, halfSide);
glTexCoord2f(1.0f, 0.0f); glVertex3f(-1 * halfSide, -1 * halfSide, halfSide);
glEnd();
glBegin(GL_POLYGON); // Green/Blue side - BACK
glNormal3f(0, 0, -1);
glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSide, -1 * halfSide, -halfSide);
glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSide, halfSide, -halfSide);
glTexCoord2f(1.0f, 1.0f); glVertex3f(-1 * halfSide, halfSide, -halfSide);
glTexCoord2f(0.0f, 1.0f); glVertex3f(-1 * halfSide, -1 * halfSide, -halfSide);
glEnd();

glBegin(GL_POLYGON); // Green/Blue side - FRONT
glNormal3f(1, 0, 0);
glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSide, -1 * halfSide, halfSide);
glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSide, -halfSide, -halfSide);
glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSide, halfSide, -halfSide);
glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSide, halfSide, halfSide);
glEnd();

glBegin(GL_POLYGON);
glNormal3f(-1, 0, 0);
glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSide, -halfSide, halfSide);
glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSide, -halfSide, -halfSide);
glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSide, halfSide, -halfSide);
glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSide, halfSide, halfSide);
glEnd();

glBegin(GL_POLYGON);
glNormal3f(0, 1, 0);
glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSide, halfSide, halfSide);
glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSide, halfSide, -halfSide);
glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSide, halfSide, -halfSide);
glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSide, halfSide, halfSide);
glEnd();

glBegin(GL_POLYGON);
glNormal3f(0, 0, -1);
glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSide, -halfSide, halfSide);
glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSide, -halfSide, -halfSide);
glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSide, -halfSide, -halfSide);
glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSide, -halfSide, halfSide);
glEnd();
glPopMatrix();

glBindTexture(GL_TEXTURE_2D, 0);
glDisable(GL_TEXTURE_2D);

}






