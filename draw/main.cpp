#include "mainwindow.h"
#include <QApplication>
#include <string>
#include"stable.h"
//#include "windows.h"
//#include "fileapi.h"
//#include "dbghelp.h"

//int crashhandler(EXCEPTION_POINTERS *pexception) {
//	QDateTime curDateTime = QDateTime::currentDateTime();
//	QString timestamp = curDateTime.toString("yy_MM_dd_hh_mm_ss")+".dmp";
//	HANDLE hDumpFile = CreateFile(
//		(LPCWSTR)timestamp.utf16(),
//		GENERIC_WRITE,
//		0,
//		NULL,
//		CREATE_ALWAYS,
//		FILE_ATTRIBUTE_NORMAL,
//		NULL);
//	if (hDumpFile != INVALID_HANDLE_VALUE) {
//		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
//		dumpInfo.ExceptionPointers = pexception;
//		dumpInfo.ThreadId = GetCurrentThreadId();
//		dumpInfo.ClientPointers = true;
//		MiniDumpWriteDump(
//			GetCurrentProcess(),
//			GetCurrentProcessId(),
//			hDumpFile,
//			MiniDumpNormal,
//			&dumpInfo,
//			NULL,
//			NULL);
//	}
//	return 0;
//}

int main(int argc, char *argv[])
{
    //EnableMemLeakCheck();
    QApplication a(argc, argv);

    //SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)crashhandler);

    QTranslator translator;
    if (translator.load( QLatin1String("draw_zh_cn.qm"),
                         QLatin1String("./"))){
        a.installTranslator(&translator);
    }else{
        qDebug()<<"no translation";
    }

    MainWindow w;
    w.show();

    if (argc>1){
        std::string s(argv[1]);
        QString message =QString(QString::fromLocal8Bit(s.c_str()));
        w.handleMessage(message);
    }

    return a.exec();
}

