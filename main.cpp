#include "mainwindow.h"
#include <QApplication>
#include <string>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
          if (translator.load( QLatin1String("draw_zh_cn.qm"),
                              QLatin1String(":/"))){
              a.installTranslator(&translator);
          }else{
              qDebug()<<"no translation";
          }
    MainWindow w;
    w.show();

//    MainWindow w2;
//    w2.show();

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
