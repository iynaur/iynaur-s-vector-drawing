#ifndef TEST_H
#define TEST_H

int test(int argc, char *argv[]);

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void setRecord(std::string file_name);

#endif // TEST_H
