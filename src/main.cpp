#include <QApplication>
#include <customizedview.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustomizedView *V=new CustomizedView;
    V->showFullScreen();
    
    return a.exec();
}
