#ifndef MYFONTDIALOG_H
#define MYFONTDIALOG_H

#include<QFontDialog>
class MyFontDialog : public QFontDialog
{
public:
    MyFontDialog();
    void keyPressEvent(QKeyEvent* e);
};

#endif // MYFONTDIALOG_H
