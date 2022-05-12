#include<windows.h>
#include <GL/glut.h>
#include<bits/stdc++.h>

#include <stdlib.h>
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 240

using namespace std;

struct lineCoord{
    float x1,y1;
    float x2,y2;
};

vector<pair<int, int> > allPoints;
map< pair<int, int>, int> allPolygonPoints, visited;


void drawPoint(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

int sign(float x)
{
    if(x>0)
    {
        return 1;
    }
    else if(x==0)
    {
        return 0;
    }
    return -1;
}

void dda_algo(lineCoord &ln)
{
    float x = ln.x1;
    float y = ln.y1;
    float m = abs(ln.y2-ln.y1)/abs(ln.x2-ln.x1);

    float s1 = sign(ln.x2 - ln.x1);
    float s2 = sign(ln.y2 - ln.y1);
    int interchange = 0;

    int count1 = abs(ln.x2-ln.x1);
    if(m>1)
    {
        m=1/m;
        interchange = 1;
        count1 = abs(ln.y2-ln.y1);
    }

    for (int i=0;i<=count1;i++)
    {
        pair<int, int> points;
        points.first = x;
        points.second = y;
        allPolygonPoints[points] = 5;

        cout<<x<<" "<<y<<endl;
        drawPoint(x, y);

        if(interchange)
        {
            y=y+s2;
            x=x+ s1*m;
        }
        else
        {
            x=x+s1;
            y=y+s2*m;
        }
    }
}

void floodFill(int x, int y, int depth)
{
    glFlush();
    pair<int, int> points;
    points.first = x;
    points.second = y;

    if(allPolygonPoints[points] == 5 || visited[points] == 5)
    {
        return;
    }
    if(x<0 || x>=SCREEN_WIDTH)
    {
        return;
    }
    if(y<0 || y>=SCREEN_HEIGHT)
    {
        return;
    }

    visited[points] = 5;

    drawPoint(x, y+1);
    floodFill(x, y+1, depth+1);

    drawPoint(x+1, y);
    floodFill(x+1, y, depth+1);

    drawPoint(x, y-1);
    floodFill(x, y-1, depth+1);

    drawPoint(x-1, y);
    floodFill(x-1, y, depth+1);
}


void drawPolygon()
{

    for(int i=0;i<allPoints.size()-1;i++)
    {

        struct lineCoord ln;
        ln.x1 = allPoints[i].first;
        ln.y1 = allPoints[i].second;

        ln.x2 = allPoints[i+1].first;
        ln.y2 = allPoints[i+1].second;

        allPolygonPoints[allPoints[i]] = 5;
        allPolygonPoints[allPoints[i+1]] = 5;

        dda_algo(ln);
    }

}

int flag=0;
void keyBoardHandler(unsigned char button, int x, int y)
{
    if(button == 's')
    {
        flag=1;
    }
}

void mouseHandler(int button, int state, int x, int y)
{
    y = SCREEN_HEIGHT - y;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(flag == 1)
        {
            floodFill(x, y, 0);
        }
        else if(flag ==0)
        {
            pair<int, int> points;
            points.first = x;
            points.second = y;

            allPoints.push_back(points);
        }
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        allPoints.push_back(allPoints[0]);
        drawPolygon();
        allPoints.clear();
    }
    glFlush();
}

void display()
{
    glFlush();
}


void init()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glMatrixMode(GL_POSITION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_SINGLE);

    glutCreateWindow("GLUT Shapes");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutKeyboardFunc(keyBoardHandler);
    glutMainLoop();

    return EXIT_SUCCESS;
}
