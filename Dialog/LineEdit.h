#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include "Label.h"
class CLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CLineEdit(QWidget *parent = 0);
     bool eventFilter(QObject *target,QEvent *event);
signals:
    
public slots:

protected:
    void resizeEvent(QResizeEvent *);
private:
    void initWidgets();
private slots:
    void sltTextChanged(const QString &str);
private:
    CLabel   *m_qpClearLbl;
private:
    bool     m_bFocus;
    int lineHeight;
    
};

#endif // LINEEDIT_H
