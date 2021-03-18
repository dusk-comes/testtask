#include "window.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    int width = 800;
    int height = 400;
    w.setMinimumSize(width, height);
    w.show();
    return a.exec();
}
