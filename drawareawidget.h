#ifndef DRAWAREAWIDGET_H
#define DRAWAREAWIDGET_H
 
#include <QWidget>
#include <QPainter>
 
#include "GeneralShape.h"
#include "curve.h"
#include "closecurve.h"
#include "circle.h"
#include "Polyline.h"
#include "Polygon.h"
#include "Rect.h"
#include "Ellipse.h"
#include "text.h"
#include <QList>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QScrollArea>
#include <QScrollBar>
#include "combo.h"
#include "addaction.h"
#include "deleteaction.h"
#include "editaction.h"
#include "combineaction.h"
#include "divideaction.h"
enum MouseHanded {None, RotationPoint,ScalePoint};
enum Category {CurveCategory,CloseCurveCategory, PolylineCategory,PolygonCategory,PickCategory,CircleCategory,
               RectCategory,EllipseCategory,TextCategory,PalmCategory};//Line, Rect, Text,
class DrawAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawAreaWidget(QWidget *parent = 0);
    void init();
    void setCategory(Category c);
    void finishcurrentShape();
    QList<GeneralShape *>  pickShape(QPointF point);
    QList<GeneralShape *>  pickShape(QPointF p0, QPointF p1);
    //void  expand(QPoint point);
    void expand();
    void fitcanvas();
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool inRange(QPointF Point,QList<GeneralShape *> sps);

    void del(GeneralShape * sp);
    void pickedMove(qreal x,qreal y);
    void save();
    void open();
    void add(QString line);
    void addfile();
    void opennew();
    bool maybeSave();
    void addshape(GeneralShape * shape);
    void zoom(double ratio);
    void rotate();
    bool isRotationHandlePoint(QPointF realPoint,GeneralShape* pickedShape);
    bool isScaleHandlePoint(QPointF realPoint,GeneralShape* pickedShape);
//    void openOldFileFormat();
//    void openold();
    void zoomone();
    void dozoom();
    void undo();
    void redo();
    void addaction(AbstractAction* act);
    void openfile(QString file);

    void changeToClose();
    void copyPaste();
    QList<GeneralShape *> copy();
    QList<GeneralShape *> cut();
    void paste(QList<GeneralShape *> *copyShapes);
    void combination();
    void divide();
    void divide(GeneralShape* shape);
    void divideToEnd();
    void divideToEnd(GeneralShape* shape);
    void test();
    void getOutOfCombo(GeneralShape* sp,Combo* tmp);
    void getIntoCombo(GeneralShape* sp,Combo* tmp);

    //void closeEvent(QCloseEvent *event);

    QScrollBar *hBar, *vBar;
    qreal zoomRatio,previouszoomRatio;
    double windowwidth,windowheight;
    QList <AbstractAction*> actionList;
    int actionindex;
    void setBrush(GeneralShape* sp);
public slots:
    void moveToTop();
    void moveToBottom();
    void setBrush();
protected:
    void paintEvent(QPaintEvent *);
    //void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent( QContextMenuEvent * event );


     

private:
    QList<GeneralShape *> shapes;
    GeneralShape *currentShape;
    Rect* pickRect;
    QList<GeneralShape *> pickedShapes;
    QPointF startPoint,endPoint;
    QPoint startCursorPoint,endCursorPoint;
    Category currentCategory;
    MouseHanded currentMouseHanded;
    bool isLeftButtonPressed;
    int dx,dy;//坐标原点的画布位置
    //bool isScrolling;

    bool saved;
    bool openasadd;
    bool changed;
    qreal pickedShapestartsx,pickedShapestartsy;
    qreal pickedShapestartRotationangle;
    QColor backcolor;
    QAction*  actionMoveToTop;
    QAction* actionMoveToBottom;
    QAction* actionSetBrush;

};
 
#endif // DRAWAREAWIDGET_H
