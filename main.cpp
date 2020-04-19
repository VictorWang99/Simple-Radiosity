#include "myglwidget.h"
#include<iostream>
#include <QApplication>

using namespace std;
int main(int argc, char *argv[])
{
    create_scene();
    QApplication a(argc, argv);
    MyGLWidget w;
    w.setWindowTitle("Project");
    w.show();
    return a.exec();
}
