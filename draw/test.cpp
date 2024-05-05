#define protected public
#include "mainwindow.h"
#undef protected
#include "test.h"
int test(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DrawAreaWidget draw;

    draw.setCategory(RectCategory);
    // QMouseEvent(Type type, const QPointF &localPos, const QPointF &globalPos,
    //             Qt::MouseButton button, Qt::MouseButtons buttons,
    //             Qt::KeyboardModifiers modifiers,
    {
        QMouseEvent e(QEvent::MouseButtonPress,
                      QPointF(100, 100),
                      QPointF(100, 100),
                      Qt::MouseButton::LeftButton,
                      Qt::MouseButtons(Qt::MouseButton::LeftButton),
                      Qt::KeyboardModifiers(Qt::KeyboardModifier::NoModifier));
        draw.mousePressEvent(&e);

        draw.mouseMoveEvent(&e);
    }

    {
        QMouseEvent e(QEvent::MouseButtonRelease,
                      QPointF(200, 200),
                      QPointF(100, 100),
                      Qt::MouseButton::LeftButton,
                      Qt::MouseButtons(Qt::MouseButton::LeftButton),
                      Qt::KeyboardModifiers(Qt::KeyboardModifier::NoModifier));
        draw.mouseReleaseEvent(&e);
    }

    draw.show();
    return a.exec();
}
