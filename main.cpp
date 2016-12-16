#include "mainwindow.h"
//#include <QApplication>
//#include <string>

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

//    MyFontDialog dlg;
//    dlg.exec();
    MainWindow w;
    w.show();

    if (argc>1){
        std::string s(argv[1]);

        QString message =QString(QString::fromLocal8Bit(s.c_str()));
        w.handleMessage(message);
    }



    return a.exec();
}
