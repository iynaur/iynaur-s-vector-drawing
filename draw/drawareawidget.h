#ifndef DRAWAREAWIDGET_H
#define DRAWAREAWIDGET_H
 
#include <QWidget>
#include <QPainter>
 
#include "generalshape.h"
#include "curve.h"
#include "closecurve.h"
#include "circle.h"
#include "polyline.h"
#include "polygon.h"
#include "rect.h"
#include "ellipse.h"
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
#include "topaction.h"
#include "bottomaction.h"
#include "setbrushaction.h"
#include"texteditaction.h"
#include"slightmoveaction.h"
#include <memory>
#include<QtPrintSupport/QPrinter>
#include<QtPrintSupport/QPrintDialog>
#include <QPrintPreviewDialog>
#include"codeeditdlg.h"
#include"setpenaction.h"
#include"mywidget.h"
#include"rotatewidget.h"
#include "dataserver.h"
#include "IShapeEditor.h"
#include "IShapeBuilder.h"

class DrawAreaWidget : public QWidget
{
    Q_OBJECT
public:
    //void setTool(Tool* tool);
    explicit DrawAreaWidget(QWidget *parent = 0);
    ~DrawAreaWidget();
    void initIShapeEditor();
    void init();
    void setCategory(Category c, QString name = "");
    void finishcurrentShape();
    QList<shared_ptr<GeneralShape>>  pickShape(QPointF point);
    QList<shared_ptr<GeneralShape>>  pickShape(QPointF p0, QPointF p1);
    //void  expand(QPoint point);
    void expand();
    void fitcanvas();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //bool event(QEvent *event);
    bool inRange(QPointF Point,QList<shared_ptr<GeneralShape>> sps);


    //void pickedMove(qreal x,qreal y);
    void save();
    void saveAs();
    void open();
    //void add(QString line);
    void addfile();
    void opennew();
    bool maybeSave();
    void zoom(double ratio);
    void zoomone();
    void dozoom();
    void undo();
    void redo();
    void addaction(AbstractAction* act);
    void addaction(QList<AbstractAction*> alist);
    void openfile(QString file);

    void copyPaste();
    QList<shared_ptr<GeneralShape>> copy();
    QList<shared_ptr<GeneralShape>> cut();
    void paste(QList<shared_ptr<GeneralShape>> *copyShapes);
    void combination();
    void divide();
    void divide(shared_ptr<GeneralShape> shape);
    void divideToEnd();
    void divideToEnd(shared_ptr<GeneralShape> shape);
    void test();
    void print();
    void printPreview();
	void creatShape();
	void finishShape();
	void editShape();
	shared_ptr<IShapeBuilder> getBuilderFrom(Category c);

    shared_ptr<Combo> fromFile(QString filename);

    QScrollBar *hBar, *vBar;
    qreal zoomRatio,previouszoomRatio;
    double windowwidth,windowheight;
    //QList <AbstractAction*> actionList;
    //int actionindex;
    void setBrush(shared_ptr<GeneralShape> sp);
    QString filename;
    QUndoStack undoStack;

//private:
    static int numOfFiles;
	dataserver* server;
    QThread *m_thread;
    bool m_ctrl; // ctrl key when mouse
    bool m_rect_pick;


public slots:
    void moveToTop();
    void moveToBottom();
    void setBrush();
    void setPen();
    void setPen(shared_ptr<GeneralShape> sp);
    void editText();
    void onPaintRequested(QPrinter* printer);
    void codeEdit();
    void contextMenuEvent( QContextMenuEvent * event );
	void needUpdate();
	void del();
protected:
    void paintEvent(QPaintEvent *);
    //void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    //void clicke
    void leaveEvent(QEvent *event);
    void rightMousePressEvent(QMouseEvent *event);
	void Update();
signals:
    void categoryChanged();
    void statusChanged();
    void mouseMoved(QMouseEvent *event,QPointF realPoint);
    void mouseLeave();
    void undoStack_push(AbstractAction* act);
	void shapes_append(shared_ptr<GeneralShape> currentShape);
public:
    QList<shared_ptr<GeneralShape> > shapes;
    shared_ptr<GeneralShape> currentShape;
    shared_ptr<Rect> pickRect;
    QList<shared_ptr<GeneralShape>> pickedShapes;
    QPointF startPoint,endPoint;
    QPoint startCursorPoint,endCursorPoint;
    Category currentCategory;
    QString currentName;
    //Tool* currentTool;
    MouseHanded currentMouseHanded;
    bool isLeftMouseButtonPressed;
    int dx,dy;//坐标原点的画布位置
    //bool isScrolling;

    bool saved;
    bool openasadd;
    //bool changed;
    int saveIndex;
    QColor backcolor;
    QAction* actionMoveToTop;
    QAction* actionMoveToBottom;
    QAction* actionSetBrush, *actionCodeEdit;
    QAction* actionSetPen;
	QAction* actionDelShape;
    QSize realSize;
    QPoint realDp;
    bool needKeyboard;
    //Combo* root;
	bool m_bDelayPaint;
	shared_ptr<IShapeBuilder> m_curIShapeBuilder;
	shared_ptr<IShapeEditor> m_curIShapeEditor;
	bool m_bIsDuringOperation;
};
 
#endif // DRAWAREAWIDGET_H
