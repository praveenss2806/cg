#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include<bits/stdc++.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

using namespace std;

struct pointPair{
    float x1, y1;
    float x2, y2;
};

struct line_eq{
    float m, c;
};
struct pointPair window, line;
int cnt=0;
int flag = 0;

void drawWindow(struct pointPair &wn)
{
    glBegin(GL_LINE_LOOP);
        glVertex2f(wn.x1, wn.y1);
        glVertex2f(wn.x1, wn.y2);
        glVertex2f(wn.x2, wn.y2);
        glVertex2f(wn.x2, wn.y1);
    glEnd();
}

void drawLine(struct pointPair &ln)
{
    glBegin(GL_LINES);
        glVertex2f(ln.x1, ln.y1);
        glVertex2f(ln.x2, ln.y2);
    glEnd();
}

int check_side_of_line(struct pointPair &ln, float x, float y)
{
    float p = 1.0*(ln.x2 - ln.x1)*(y - ln.y1) -  1.0*(ln.y2 - ln.y1)*(x - ln.x1);
    if(p<0)
    {
        return -1;
    }
    return 1;
}
// TBRL
vector<int> assign_codes(struct pointPair &wn, float x, float y)
{
    vector<int> code(4, 0);
    struct pointPair ln;
    int side = 0;

    ln.x1 = wn.x1;
    ln.y1 = wn.y1;
    ln.x2 = wn.x1;
    ln.y2 = wn.y2;

    side = check_side_of_line(ln, x, y);
    if (side > 0)
    {
        code[3] = 1;
    }

    ln.x1 = wn.x1;
    ln.y1 = wn.y2;
    ln.x2 = wn.x2;
    ln.y2 = wn.y2;

    side = check_side_of_line(ln, x, y);
    if (side > 0)
    {
        code[0] = 1;
    }

    ln.x1 = wn.x2;
    ln.y1 = wn.y2;
    ln.x2 = wn.x2;
    ln.y2 = wn.y1;

    side = check_side_of_line(ln, x, y);
    if (side > 0)
    {
        code[2] = 1;
    }

    ln.x1 = wn.x2;
    ln.y1 = wn.y1;
    ln.x2 = wn.x1;
    ln.y2 = wn.y1;

    side = check_side_of_line(ln, x, y);
    if (side > 0)
    {
        code[1] = 1;
    }

    return code;
}

bool and_fun(vector<int> &code1, vector<int> &code2)
{
    for(int i=0;i<4;i++)
    {
        if(code1[i] == 1 && code2[i] == 1)
        {
            return true;
        }
    }
    return false;
}

bool or_fun(vector<int> &code1, vector<int> &code2)
{
    for(int i=0;i<4;i++)
    {
        if(code1[i] == 1 || code2[i] == 1)
        {
            return false;
        }
    }
    return true;
}

void calIntersect(struct pointPair &wn, vector<int> &code, struct line_eq &eq, float &x, float &y)
{
    if(code[0] == 1)
    {
        y = wn.y2;
        x = (y - eq.c)/(eq.m);
    }
    else if(code[1] == 1)
    {
        y = wn.y1;
        x = (y - eq.c)/(eq.m);
    }
    else if(code[2] == 1)
    {
        x = wn.x2;
        y = eq.m * x + eq.c;
    }
    else if(code[3] == 1)
    {
        x = wn.x1;
        y = eq.m * x + eq.c;
    }
}

void cohen_sutherland(struct pointPair &wn, struct pointPair &ln)
{
    vector<int> code1 = assign_codes(wn, ln.x1, ln.y1);
    vector<int> code2 = assign_codes(wn, ln.x2, ln.y2);

    for(int i=0;i<4;i++)
    {
        cout<<code1[i];
    }
    cout<<endl;

    for(int i=0;i<4;i++)
    {
        cout<<code2[i];
    }
    cout<<endl;

    if(or_fun(code1, code2))
    {
        cout<<"completely inside"<<endl;
    }
    else if(and_fun(code1, code2))
    {
        cout<<"completely outside"<<endl;
    }
    else
    {
        struct line_eq eq;
        eq.m = (line.y2 - line.y1)/(line.x2 - line.x1);
        eq.c = line.y1 - eq.m * line.x1;

        float x1, y1, x2, y2;
        calIntersect(wn, code1, eq, x1, y1);
        calIntersect(wn, code2, eq, x2, y2);

        glColor3f(0, 1, 0);
        if(wn.x1<=x1 && x1<=wn.x2 && wn.y1<=y1 && y1<=wn.y2)
        {
            glBegin(GL_LINES);
                glVertex2f(x1, y1);
                glVertex2f(x2, y2);
            glEnd();
        }
        glColor3f(1, 1, 1);
    }
}

void keyboardFunct(unsigned char button, int x, int y)
{
    if(button == 'c')
    {
        glutPostRedisplay();
        glFlush();
    }
    if(button == 's')
    {
        flag++;
        flag = flag%3;
        cnt=0;
    }
}

void mouseHandler(int button, int state, int x, int y)
{
    y = SCREEN_HEIGHT - y;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(flag == 0)
        {
            if(cnt == 0)
            {
                window.x1 = x;
                window.y1 = y;
                cnt++;
            }
            else
            {
                window.x2 = x;
                window.y2 = y;
                cnt++;
            }
            cnt = cnt%2;
        }
        else if(flag == 1)
        {
            if(cnt == 0)
            {
                line.x1 = x;
                line.y1 = y;
                cnt++;
            }
            else
            {
                line.x2 = x;
                line.y2 = y;
                cnt++;
            }
            cnt = cnt%2;
        }
        else if(flag == 2)
        {
            vector<int> code1 = assign_codes(window, x, y);
            for(int i=0;i<4;i++)
            {
                cout<<code1[i];
            }
            cout<<endl;
        }

    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        if(flag == 0)
        {
            drawWindow(window);
        }
        else if(flag == 1)
        {
            int x1, x2, y1, y2;
            x1 = min(window.x1, window.x2);
            y1 = min(window.y1, window.y2);
            x2 = max(window.x1, window.x2);
            y2 = max(window.y1, window.y2);

            window.x1 = x1;
            window.x2 = x2;
            window.y1 = y1;
            window.y2 = y2;

            drawLine(line);
            cohen_sutherland(window, line);
        }
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
    glutKeyboardFunc(keyboardFunct);
    glutMainLoop();

    return EXIT_SUCCESS;
}
