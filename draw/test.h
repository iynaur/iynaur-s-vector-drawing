#ifndef TEST_H
#define TEST_H

class DrawAreaWidget;
int test(int argc, char *argv[]);

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void setRecord(std::string file_name);

void runScript(std::string file_name, DrawAreaWidget* draw, QApplication* a, QMainWindow* m);

#endif // TEST_H
