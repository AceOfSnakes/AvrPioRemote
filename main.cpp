#include "avrpioremote.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AVRPioRemote w;
    w.show();
    
    return a.exec();
}
