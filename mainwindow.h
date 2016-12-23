#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawareawidget.h"
#include "scroll.h"
#include <QScrollArea>
#include <QMdiArea>
#include <QToolButton>
#include <QSettings>
#include<cmath>
#include <algorithm>
#include<QLabel>
#include <QSlider>
#include <QMdiSubWindow>
#include <QString>
//enum Category {CurveCategory,CloseCurveCategory, PolylineCategory,PolygonCategory,PickCategory,CircleCategory,
//               RectCategory,EllipseCategory,TextCategory,PalmCategory};//Line, Rect, Text,
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Scroll* scrollArea;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);


    void readSettings();

    QList<shared_ptr<GeneralShape>>* copyShapes;
    Category currentCategory;
    //void updateToolBar();

    QList<QAction*> redoList;
    QList<QAction*> undoList;


private slots:
    void on_actionCurve_triggered();
    void on_actionCloseCurve_triggered();
    void on_actionPolyline_triggered();
    void on_actionPolygon_triggered();
    void on_actionPick_triggered();
    void on_actionCircle_triggered();
    void on_actionFixcanvas_triggered();
    void on_actionRect_triggered();
    void on_actionEllipse_triggered();
    void on_actionText_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionAdd_triggered();
    void  on_actionZoomIn_triggered();
    void  on_actionZoomOut_triggered();
    //void on_actionRotate_triggered();
    void  on_actionPalm_triggered();
    //void  on_actionOpenOldFileFormat_triggered();
    void  on_actionZoomOne_triggered();
    void  on_actionMoveToTop_triggered();
    void  on_actionMoveToBottom_triggered();
    void  on_actionCombination_triggered();
    void  on_actionDivide_triggered();
    void  on_actionDivideToEnd_triggered();
    void  on_actionNew_triggered();
    void  onSubWindowActivated(QMdiSubWindow* window);

    void   on_actionCopy_triggered();
    void   on_actionPaste_triggered();

    void on_actionExpand_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void updateToolBar();
    //void UndoTo(int i);
    void UndoTo(QAction* action);
    void RedoTo(QAction* action);
    void on_actionSaveAs_triggered();
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    void on_actionPrint_triggered();

    void on_actionPrintPreview_triggered();
    void updateStatusBar(QMouseEvent *, QPointF p);
    void clearStatusBar();
    void on_actionClearSettings_triggered();
public slots:
    void handleMessage(QString message);
    void zoom(int z);
    //void onscrollContentsBy(int dx, int dy);
signals:
    //scrollDone();
private:
    Ui::MainWindow *ui;
    //DrawAreaWidget* drawAreaWidget;
    QMdiArea *mdiArea;
    //Scroll*child;
    QMdiSubWindow*le;
    QToolButton * undoButton;
    QToolButton*redoButton;
    QMenu* menuUndoTo;
    QMenu* menuRedoTo;
    QSettings *settings;
    QSlider* sld;
    QLabel* lb;
};

#endif // MAINWINDOW_H
