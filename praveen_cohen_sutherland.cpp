#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int n=0;
int arrX[4];
int arrY[4];
const int INSIDE = 0; // 0000
const int LEFT = 1; // 0001
const int RIGHT = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8; // 1000

int x_max;
int y_max;
int x_min;
int y_min;

void PolygonDraw()
{
    glBegin(GL_LINES);
        glVertex2f(arrX[2],arrY[2]);
        glVertex2f(arrX[3],arrY[3]);
    glEnd();
    glFlush();
}

void PolygonDraw1()
{
    glColor3d(0,1,1);
    x_max = arrX[1];
    y_max = arrY[1];
    x_min = arrX[0];
    y_min = arrY[0];
    glBegin(GL_LINE_LOOP);
        glVertex2f(arrX[0],arrY[0]);
        glVertex2f(arrX[0],arrY[1]);
        glVertex2f(arrX[1],arrY[1]);
        glVertex2f(arrX[1],arrY[0]);
    glEnd();
    glFlush();
}


int computeCode(double x, double y)
{
	int code = INSIDE;
    if (x < x_min)
    {
        code |= LEFT;
    }
    else if (x > x_max)
    {
        code |= RIGHT;
    }
    if (y < y_min)
    {
        code |= BOTTOM;
    }
    else if (y > y_max)
    {
        code |= TOP;
    }
    return code;
}

void cohenSutherlandClip(double x1, double y1,double x2, double y2)
{
	int code1 = computeCode(x1, y1);
	int code2 = computeCode(x2, y2);
	bool accept = false;

	while (true)
	{
		if ((code1 == 0) && (code2 == 0))
        {
			accept = true;
			break;
		}
		else if (code1 & code2)
        {
			break;
		}
		else
        {
			int code_out;
			double x, y;
			if (code1 != 0)
            {
                code_out = code1;
            }
			else
            {
                code_out = code2;
            }
			if (code_out & TOP)
            {
				x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
				y = y_max;
			}
			else if (code_out & BOTTOM)
            {
				x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
				y = y_min;
			}
			else if (code_out & RIGHT)
            {
				y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
				x = x_max;
			}
			else if (code_out & LEFT)
            {
				y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
				x = x_min;
			}
			if (code_out == code1) {
				x1 = x;
				y1 = y;
				code1 = computeCode(x1, y1);
			}
			else {
				x2 = x;
				y2 = y;
				code2 = computeCode(x2, y2);
			}
		}
	}
	if (accept) {
		cout << "Line accepted from " << x1 << ", "<< y1 << " to " << x2 << ", " << y2 << endl;
		arrX[2]=x1;
		arrX[3]=x2;
		arrY[2]=y1;
		arrY[3]=y2;
		glColor3d(1,1,0);
		PolygonDraw();
	}
	else
		cout << "Line rejected" << endl;
}

void init()
{
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
}
void mouseHandler(int button,int state,int x,int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        arrX[n]=x;
        arrY[n]=480-y;
        n++;
    }
    if(n==2)
    {
            PolygonDraw1();
    }
    else if(n==4)
    {
        PolygonDraw();
        cohenSutherlandClip(arrX[2],arrY[2],arrX[3],arrY[3]);
        n=2;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("GLUT Shapes");
    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    PolygonDraw();
    glutMainLoop();
    return EXIT_SUCCESS;
}
