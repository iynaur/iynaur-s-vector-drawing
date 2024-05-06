
#ifdef WIN32
#define _HAS_STD_BYTE 0
#include "windows.h"
#include "fileapi.h"
#include "dbghelp.h"
#pragma comment( lib, "Dbghelp.lib" )

#ifdef _DEBUG
// #define NEW_DEBUG new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new(...) new(__VA_ARGS__, _NORMAL_BLOCK, __FILE__, __LINE__ )

// #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

void EnableMemLeakCheck()
{
    int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
    tmpFlag |= _CRTDBG_ALLOC_MEM_DF;
    _CrtSetDbgFlag(tmpFlag);

    // _CrtSetBreakAlloc(28367);
    // _CrtSetBreakAlloc(9553);
    // _CrtSetBreakAlloc(9552);
}

int crashhandler(EXCEPTION_POINTERS *pexception) {
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString timestamp = curDateTime.toString("yy_MM_dd_hh_mm_ss")+".dmp";
    HANDLE hDumpFile = CreateFile(
        (LPCWSTR)timestamp.utf16(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pexception;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = true;
        MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hDumpFile,
            MiniDumpNormal,
            &dumpInfo,
            NULL,
            NULL);
    }
    return 0;
}
#endif

#include "mainwindow.h"
#include <QApplication>
#include <string>
#include "stable.h"
#include "test.h"

int main(int argc, char *argv[])
{
    if (argc > 1 && std::string(argv[1]) == "t") {
        return test(argc, argv);
    }
    setRecord("./record.txt");
#ifdef WIN32
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(30923);
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)crashhandler);
#endif
    QApplication a(argc, argv);

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

