#include<windows.h>
#include <GL/glut.h>
#include<bits/stdc++.h>
#include <stdlib.h>
#include<iostream>


using namespace std;

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    glFlush();
}

vector<vector<int> >polygon;
int MinY=480,MaxY = 0;


void plotPoint(int x,int y){
    glColor3d(1,0,0);
    glPointSize(3.0);
    glBegin(GL_POINTS);
        glVertex2d(x,y);
    glEnd();
    glFlush();
}

void plotPolygon(vector<vector<int> >&polygon){
    glColor3d(1,0,0);
    glBegin(GL_LINES);
        for(int i=0;i<polygon.size();i++){
            glVertex2d(polygon[i][0],polygon[i][1]);
            glVertex2d(polygon[(i+1)%polygon.size()][0],polygon[(i+1)%polygon.size()][1]);
        }
    glEnd();
    glFlush();
}

bool isEndPoints(int x,int y){
    for(int i=0;i<polygon.size();i++){
        if(polygon[i][0] == x && polygon[i][1] == y){
            return true;
        }
    }
    return false;
}

int posOfPoint(int x,int y){
    for(int i=0;i<polygon.size();i++){
        if(polygon[i][0] == x && polygon[i][1] == y){
            return i;
        }
    }
    return -1;
}

void YXAlgorithm(){

    cout<<"Polygon Points "<<endl;
    for(int i=0;i<polygon.size();i++){
        cout<<polygon[i][0]<<" "<<polygon[i][1]<<endl;
    }



    //just that white lines above and below the polygon
    glColor3d(1,1,1);
    glBegin(GL_LINES);
        glVertex2d(0,MinY-10);
        glVertex2d(640,MinY-10);
        glVertex2d(0,MaxY+10);
        glVertex2d(640,MaxY+10);
    glEnd();
    glFlush();

    vector<vector<float> >slopesAtPoints;
    vector<vector<float> >equations;

    for(int i=0;i<polygon.size();i++){
        vector<float>v(2);
        slopesAtPoints.push_back(v);
    }

    //cout<<"Edges and slope and c "<<endl;
    for(int i=0;i<polygon.size();i++){
        int x1 = polygon[i][0];
        int y1 = polygon[i][1];

        int x2 = polygon[(i+1)%polygon.size()][0];
        int y2 = polygon[(i+1)%polygon.size()][1];


        vector<float>v(4);
        v[0] = x1;v[1] = x2;

        float m = (y2-y1)/((x2-x1)*1.0);
        float c = y2 - m*x2;

        cout<<"The slope and intercept between ("<<x1<<","<<y1<<") and ("<<x2<<","<<y2<<" )  is "<<m<<" "<<c<<endl;

        v[2] = m;v[3] = c;
        equations.push_back(v);

        slopesAtPoints[i][1] = m;
        slopesAtPoints[(i+1)%polygon.size()][0] = m;
    }

    /*
    for(int i=0;i<slopesAtPoints.size();i++){
        cout<<slopesAtPoints[i][0] << " "<<slopesAtPoints[i][1]<<endl;
    }*/



    for(float i = MinY-10;i<MaxY+10;i=i+0.5){


        vector<int>intersectionPoints;

        cout<<"-------------"<<endl;
        cout<<i<<endl;
        cout<<"-------------"<<endl;
        for(int j=0;j<equations.size();j++){
            cout<<equations[j][2]<<" "<<equations[j][3]<<endl;
            float x = (i-equations[j][3])/(equations[j][2]*1.0);
            cout<<x<<" "<<equations[j][0]<<" "<<equations[j][1]<<endl;
            if(x >= min(equations[j][0],equations[j][1]) && x<= max(equations[j][0],equations[j][1])){
                //there is intersection with this line
                cout<<"There is intersection " <<endl;
                intersectionPoints.push_back(x);
            }
        }

        sort(intersectionPoints.begin(),intersectionPoints.end());

        vector<int>plotLinePoints;
        for(int j=0;j<intersectionPoints.size();j++){
            if(isEndPoints(intersectionPoints[j],i)){
                int pos = posOfPoint(intersectionPoints[j],i);
                if(slopesAtPoints[pos][0] > 0 && slopesAtPoints[pos][1]<0){
                    plotLinePoints.push_back(intersectionPoints[j]);
                    plotLinePoints.push_back(intersectionPoints[j]);
                }else{
                    plotLinePoints.push_back(intersectionPoints[j]);
                }
            }else{
                plotLinePoints.push_back(intersectionPoints[j]);
            }
        }
        cout<<"For the y value "<<i<<" ,the intersection points size is "<<plotLinePoints.size()<<endl;
        glColor3d(1,1,1);
        glBegin(GL_LINES);
            for(int j=0;j<plotLinePoints.size();j+=2){
                cout<<plotLinePoints[j]<<" "<<i<<endl;
                cout<<plotLinePoints[j+1]<<" "<<i<<endl;
                glVertex2d(plotLinePoints[j],i);
                glVertex2d(plotLinePoints[j+1],i);
            }
        glEnd();
        glFlush();
    }


}


void mouseHandler(int button,int state,int x,int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        vector<int>v(2);
        v[0] = x;v[1] = 480-y;
        MinY = min(MinY,480-y);
        MaxY = max(MaxY,480-y);
        plotPoint(x,480-y);
        polygon.push_back(v);
    }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        plotPolygon(polygon);
        YXAlgorithm();
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

    glutMainLoop();
    return EXIT_SUCCESS;
}
