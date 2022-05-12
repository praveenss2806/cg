#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include<bits/stdc++.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

using namespace std;

vector<pair<int, int> > polygon;

map< int, vector<int> > allPolygonPoints;
map<pair<int, int> , int > visited;

struct lineCoord{
    float x1,y1;
    float x2,y2;
};

void drawPoint(int x, int y)
{
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}
int sign(int x)
{
    if (x>0)
    {
        return 1;
    }
    else if(x == 0)
    {
        return 0;
    }
    return -1;
}

void bresenhamDrawLine(lineCoord &ln, map< int, vector<int> > &allp)
{
    int x1, x2, y1, y2;
    x1 = ln.x1;
    x2 = ln.x2;
    y1 = ln.y1;
    y2 = ln.y2;

    int dx = abs(x1-x2);
    int dy = abs(y1-y2);
    int s1 = sign(x2-x1);
    int s2 = sign(y2-y1);

    int interchange = 0;
    if(dy>dx)
    {
        int temp = dx;
        dx = dy;
        dy = temp;
        interchange = 1;
    }
    int pk = 2*dy - dx;
    for(int i=0;i<=dx;i++)
    {
        drawPoint(x1, y1);
        if(pk>0)
        {
            if(interchange == 1)
            {
                x1 = x1 + s1;
            }
            else
            {
                y1 = y1 + s2;
            }
            pk -= 2*dx;
        }
        if(interchange == 1)
        {
            y1 = y1 + s2;
        }
        else
        {
            x1 = x1 + s1;
        }
        pk+= 2*dy;

        if (visited[make_pair(x1, y1)] == 0)
        {
            allp[y1].push_back(x1);
        }
        visited[make_pair(x1, y1)] = 5;
        drawPoint(x1, y1);
    }
}

void fillPolygon(vector<pair<int, int> > &pl, map< int, vector<int> > &allp)
{
    for (int i=0;i<pl.size()-1;i++)
    {
        struct lineCoord ln1;
        ln1.x1 = pl[i].first;
        ln1.x2 = pl[i+1].first;

        ln1.y1 = pl[i].second;
        ln1.y2 = pl[i+1].second;

        bresenhamDrawLine(ln1, allp);
    }
    int x2 = 0;
    for(auto i=allp.begin(); i!=allp.end();i++)
    {
        int y = i->first;
        vector<int> x = i->second;
        sort(x.begin(), x.end());

        cout<<y<<"--::--";
        for(int j=0;j<x.size();j++)
        {
            cout<<x[j]<<" ";
        }
        cout<<endl;

        int offset = 0;
        int cnt=0;
        for(int j=0;j<x.size()-1;j++)
        {
            while(j<x.size()-1 && x[j+1]-x[j] < 2)
            {
                j++;
                if(j>=x.size()-1)
                {
                    break;
                }
            }
            if(j>=x.size()-1)
            {
                break;
            }
            if(cnt%2 == 0 && x[j+1]-x[j] > 2)
            {
                for(int k=x[j];k<x[j+1];k++)
                {
                    drawPoint(k, y);
                }
            }
            cnt++;
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

        polygon.push_back(p);
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        polygon.push_back(polygon[0]);
        fillPolygon(polygon, allPolygonPoints);
        polygon.clear();
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
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_SINGLE);

    glutCreateWindow("GLUT Shapes");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutMainLoop();

    return EXIT_SUCCESS;
}
