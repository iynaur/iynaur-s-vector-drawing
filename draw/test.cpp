#define protected public
#include "mainwindow.h"
#undef protected
#include "test.h"
#include <QApplication>
#include <fstream>
#include <sstream>
#include <cstring>

std::string record_file;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    FILE* file = fopen(record_file.c_str(), "a");
    switch (type) {
    case QtDebugMsg:
        fprintf(file, "%s\n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
    fclose(file);
}


void setRecord(std::string file_name)
{
    record_file = file_name;
    FILE* file = fopen(record_file.c_str(), "w");
    fclose(file);
    qInstallMessageHandler(myMessageOutput); // Install the handler
}

void runScript(std::string file_name, DrawAreaWidget* draw, QApplication* a, QMainWindow* m)
{
    std::ifstream infile(file_name.c_str());
    std::string line;

    QPointF qp;
    Qt::MouseButton mb;
    QEvent::Type tp;
    QFlags<Qt::KeyboardModifier> mod;

    const std::string cat_str = "setCategory";
    const std::string mod_str = "QFlags<Qt::KeyboardModifier>";

    while (std::getline(infile, line))
    {
        if (line.substr(0, cat_str.size()) == cat_str) {
            auto cat = line.substr(cat_str.size());
            std::istringstream iss(cat);
            int cat_i;
            iss >> cat_i;
            draw->setCategory(Category(cat_i));
        }

        if (line.substr(0, 4) == "Qt::") {
            auto but_str = line.substr(4);
            if (but_str == "RightButton") {
                mb = Qt::RightButton;
            } else {
                mb = Qt::LeftButton;
            }
        }

        if (line.substr(0, mod_str.size()) == mod_str) {
            auto mods = line.substr(mod_str.size()+1, line.size() - mod_str.size() -2);
            if (mods == "ControlModifier") {
                mod = QFlags<Qt::KeyboardModifier>(Qt::ControlModifier);
            } else {
                mod = QFlags<Qt::KeyboardModifier>(Qt::NoModifier);
            }
        }

        if (line.substr(0, 6) == "QPoint") {
            auto pos_str = line.substr(7, line.size() - 8);
            auto sp = pos_str.find(',');
            pos_str[sp] = ' ';
            std::istringstream iss(pos_str);
            iss >> qp.rx() >> qp.ry();
        }

        if (line.substr(0, 6) == "QEvent") { // must be last line of an event
            auto evt_str = line.substr(8);
            if (evt_str == "MouseButtonPress") {
                tp = QEvent::MouseButtonPress;
            } else if (evt_str == "MouseMove") {
                tp = QEvent::MouseMove;
            } else if (evt_str == "MouseButtonRelease") {
                tp = QEvent::MouseButtonRelease;
            }

            // run
            QMouseEvent e(tp,
                          qp,
                          qp,
                          mb,
                          Qt::MouseButtons(mb),
                          mod);

            if (evt_str == "MouseButtonPress") {
                draw->mousePressEvent(&e);
            } else if (evt_str == "MouseMove") {
                draw->mouseMoveEvent(&e);
            } else if (evt_str == "MouseButtonRelease") {
                draw->mouseReleaseEvent(&e);
            }
            if (tp != QEvent::MouseMove && a && m)
            {
            m->show();
            draw->repaint();
            a->exec();
            }
        }
    }
}

bool fileSame(std::string lFilePath, std::string rFilePath)
{
    std::ifstream lFile(lFilePath.c_str(), std::ios::in | std::ios::binary);
    std::ifstream rFile(rFilePath.c_str(), std::ios::in | std::ios::binary);


    if(!lFile.good() || !rFile.good())
    {
        return false;
    }

    std::streamsize lReadBytesCount = 0;
    std::streamsize rReadBytesCount = 0;
    int BUFFER_SIZE = 4096;

    char *p_lBuffer = new char[BUFFER_SIZE]();
    char *p_rBuffer = new char[BUFFER_SIZE]();

    do {
        lFile.read(p_lBuffer, BUFFER_SIZE);
        rFile.read(p_rBuffer, BUFFER_SIZE);
        lReadBytesCount = lFile.gcount();
        rReadBytesCount = rFile.gcount();

        if (lReadBytesCount != rReadBytesCount || std::memcmp(p_lBuffer, p_rBuffer, lReadBytesCount) != 0)
        {
            return false;
        }
    } while (lFile.good() || rFile.good());

    return true;
}


int test(int argc, char *argv[])
{
    bool interactive = false;
    QApplication a(argc, argv);
    for (const std::string& test_case : {".", "ctrl_show_editor", "ctrl_pick_move_no_act"}) {
        QMainWindow m;
        DrawAreaWidget* draw = new DrawAreaWidget(&m);
        m.setCentralWidget(draw);
        m.setFixedSize(900, 600);

        if (interactive) {
            runScript("./" + test_case + "/record.txt", draw, &a, &m);
        } else {
            runScript("./" + test_case + "/record.txt", draw, 0, 0);
        }
        draw->saveToFile("./result.xml");

        if (!fileSame("./result.xml", "./" + test_case + "/record.xml")) {
            return -1;
        }
        m.show();
        a.exec();
//        delete draw;
    }
    return 0;
}