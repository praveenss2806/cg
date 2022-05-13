#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include<bits/stdc++.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

using namespace std;

struct line{
    float x1, y1;
    float x2, y2;
    float m, c;
};

vector<pair<int, int> > polygon_points;
vector<line> polygon_sides;

void drawPoint(float x, float y)
{
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}


void insertPolygonSide(pair<int, int> p1, pair<int, int> p2, vector<line> &ps)
{
    struct line ln;
    ln.x1 = p1.first;
    ln.y1 = p1.second;
    ln.x2 = p2.first;
    ln.y2 = p2.second;

    ln.m = (ln.y2 - ln.y1)/(ln.x2 - ln.x1);
    ln.c = ln.y1 - ln.m*ln.x1;

    ps.push_back(ln);
}

void drawPolygon(vector<pair<int, int> > &points, vector<line> &ps)
{
    glBegin(GL_LINE_LOOP);
        for(int i=0;i<points.size();i++)
        {
            glVertex2f(points[i].first, points[i].second);
        }
    glEnd();
    int ymax=0 ,ymin = 1000;
    for(int i=0;i<points.size();i++)
    {
        ymax = max(max(points[i].second, points[i+1].second), ymax);
        ymin = min(min(points[i].second, points[i+1].second), ymin);
        insertPolygonSide(points[i], points[i+1], ps);
    }

    for(int y=ymin; y<ymax; y++)
    {
        vector<float> x;
        for(int i=0;i<ps.size();i++)
        {
            int y2 = max(ps[i].y1, ps[i].y2);
            int y1 = min(ps[i].y1, ps[i].y2);

            if(y1 <= y && y <= y2)
            {
                x.push_back((y - ps[i].c)/ps[i].m);
            }
        }
        for(int i=0;i<x.size()-1;i++)
        {
            if(i%2==0)
            {
                for(float j=x[i];j<=x[i+1];j++)
                {
                    drawPoint(j, y);
                }
            }
        }
    }
}

void mouseHandler(int button, int state, int x, int y)
{
    y = SCREEN_HEIGHT - y;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        pair<int, int> p;
        p.first = x;
        p.second = y;

        polygon_points.push_back(p);
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        polygon_points.push_back(polygon_points[0]);
        drawPolygon(polygon_points, polygon_sides);
    }
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void init()
{
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_SINGLE);

    glutCreateWindow("GLUT Shapes");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutMainLoop();

    return EXIT_SUCCESS;
}
