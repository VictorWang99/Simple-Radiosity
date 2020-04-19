#include "myglwidget.h"
#include<math.h>
#define pi 3.1415926535
#include<iostream>
#include"head.h"

using namespace std;
MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
    scene();
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

static Patch patches[20000];
int patch_num = 0;
void create_scene(){
    //墙壁和地板
    Surface(0, 50, 0, 50, 0, 0, tuple3f(0, 0, 0), tuple3f(1, 1, 1), Cvector(0, 0, 1), patches, patch_num);
    Surface(0, 0, 0, 50, 0, 50, tuple3f(0, 0, 0), tuple3f(1, 1, 1), Cvector(1, 0, 0), patches, patch_num);
    Surface(50, 50, 0, 50, 0, 50, tuple3f(0, 0, 0), tuple3f(1, 1, 1), Cvector(-1, 0, 0), patches, patch_num);
    Surface(0, 50, 0, 0, 0, 50, tuple3f(0, 0, 0), tuple3f(1, 1, 1), Cvector(0, 1, 0), patches, patch_num);
    //墙角的红色箱子
    Surface(0, 20, 20, 20, 0, 20, tuple3f(0, 0, 0), tuple3f(0.7, 0.4, 0), Cvector(0, 1, 0), patches, patch_num);
    Surface(20, 20, 0, 20, 0, 20, tuple3f(0, 0, 0), tuple3f(0.7, 0.4, 0), Cvector(1, 0, 0), patches, patch_num);
    Surface(0, 20, 0, 20, 20, 20, tuple3f(0, 0, 0), tuple3f(0.7, 0.4, 0), Cvector(0, 0, 1), patches, patch_num);

    //绿色箱子
    Surface(25, 45, 30, 30, 10, 40, tuple3f(0, 0, 0), tuple3f(0.0, 0.7, 0.1), Cvector(0, 1, 0), patches, patch_num);
    Surface(45, 45, 0, 30, 10, 40, tuple3f(0, 0, 0), tuple3f(0.0, 0.7, 0.1), Cvector(1, 0, 0), patches, patch_num);
    Surface(25, 25, 0, 30, 10, 40, tuple3f(0, 0, 0), tuple3f(0.0, 0.7, 0.1), Cvector(-1, 0, 0), patches, patch_num);
    Surface(25, 45, 0, 30, 40, 40, tuple3f(0, 0, 0), tuple3f(0.0, 0.7, 0.1), Cvector(0, 0, 1), patches, patch_num);
    Surface(25, 45, 0, 30, 10, 10, tuple3f(0, 0, 0), tuple3f(0.0, 0.7, 0.1), Cvector(0, 0, -1), patches, patch_num);
    //上方的光源
    Surface(20, 30, 30, 30, 20, 40, tuple3f(1, 1, 1), tuple3f(0, 0, 0), Cvector(0, -1, 0), patches, patch_num);

    //计算patch间的能量传输比例
    init_F();
    caculate_F(patches, patch_num);
    //store_F();
    //load_F();

    //迭代求解方程组
    Southwell(patches,patch_num);

    //3次均值滤波优化
    for(int i=0;i<3;i++){
        avg_filter(0,50,50,0,patches);
        avg_filter(2500,0,50,50,patches);
        avg_filter(5000,0,50,50,patches);
        avg_filter(7500,50,0,50,patches);

        avg_filter(10000,20,0,20,patches);
        avg_filter(10400,0,20,20,patches);
        avg_filter(10800,20,20,0,patches);

        avg_filter(11200,20,0,30,patches);
        avg_filter(11800,0,30,30,patches);
        avg_filter(12700,0,30,30,patches);
        avg_filter(13600,20,30,0,patches);
        avg_filter(14200,20,30,0,patches);
    }



}

void MyGLWidget::scene()
{
	glClear(GL_COLOR_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-25, 25, -25, 25, 50, 100);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(-25,-25,-100);
    //glRotatef(30,1,0,0);
    tuple3f ambient=tuple3f(0.4,0.4,0.4);

	for (int i = 0; i < patch_num; i++) {
        glColor3f(patches[i].color.r*4+patches[i].reflectance.r*ambient.r, patches[i].color.g*4+patches[i].reflectance.g*ambient.g, patches[i].color.b*4+patches[i].reflectance.b*ambient.b);
        glBegin(GL_QUADS);
        if(patches[i].x1!=patches[i].x2){
            glVertex3f(patches[i].x1, patches[i].y1, patches[i].z1);
            glVertex3f(patches[i].x2, patches[i].y1, patches[i].z1);
            glVertex3f(patches[i].x2, patches[i].y2, patches[i].z2);
            glVertex3f(patches[i].x1, patches[i].y2, patches[i].z2);
        }
        else{
            glVertex3f(patches[i].x1, patches[i].y1, patches[i].z1);
            glVertex3f(patches[i].x2, patches[i].y1, patches[i].z2);
            glVertex3f(patches[i].x2, patches[i].y2, patches[i].z2);
            glVertex3f(patches[i].x1, patches[i].y2, patches[i].z1);
        }

		glEnd();
	}

}

