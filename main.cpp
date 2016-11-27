#include "mainwindow.h"
#include <QApplication>
#include <string>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if (argc>1){
        //QString message = s2q(argv[1]);
        std::string s(argv[1]);
            // do something with s
            //return s.c_str();   //BAD
        QString message =QString(QString::fromLocal8Bit(s.c_str()));
        w.handleMessage(message);
    }

//    QObject::connect(&a, SIGNAL(messageReceived( QString)),
//             &w, SLOT(handleMessage( QString)));

    return a.exec();
}
