#include <QDebug>
#include <algorithm>
#include <QTextStream>
#include <QApplication>
#include <sstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QtXml>
#include <QColorDialog>
#include "gettextdialog.h"
#include <QFontDialog>
#include <QMenu>
#include"edittextdialog.h"
#include"codeeditdlg.h"
#include "drawareawidget.h"
#include "CallBack.h"
#include "IShapeBuilder.h"

#include "polyline.h"
#include "polygon.h"
#include "rect.h"
#include "ellipse.h"
using namespace std;
int DrawAreaWidget::numOfFiles = 0;
DrawAreaWidget::DrawAreaWidget(/*QWidget *parent*/) :
	QWidget(/*parent*/),
	m_bDelayPaint(false),
	m_bIsDuringOperation(false)
{
	needKeyboard = false;
	realSize = QSize(900, 500);
	realDp = QPoint(0, 0);
	actionMoveToTop = new QAction(tr("MoveToTop"), this);
	actionMoveToTop->setIcon(QIcon(QString::fromUtf8(":image/arrowup.PNG")));
	actionMoveToBottom = new QAction(tr("MoveToBottom"), this);
	actionMoveToBottom->setIcon(QIcon(QString::fromUtf8(":image/arrow.png")));
	actionSetBrush = new QAction(tr("SetBrush"), this);
	actionSetBrush->setIcon(QIcon(QString::fromUtf8(":image/pallet.png")));
	actionSetPen = new QAction(tr("SetPen"), this);
	actionSetPen->setIcon(QIcon(QString::fromUtf8(":image/pencapstyle.png")));
	
	actionCodeEdit = new QAction(tr("CodeEdit"), this);
	actionCodeEdit->setIcon(QIcon(QString::fromUtf8(":image/edit-set-9-512.png")));
	actionDelShape = new QAction(tr("DelShape"), this);
	actionDelShape->setIcon(QIcon(QString::fromUtf8(":image/blue-del-sign-md.png")));

	connect(actionMoveToTop, SIGNAL(triggered()), this, SLOT(moveToTop()), Qt::QueuedConnection);
	connect(actionMoveToBottom, SIGNAL(triggered()), this, SLOT(moveToBottom()));
	connect(actionSetBrush, SIGNAL(triggered()), this, SLOT(setBrush()));
	connect(actionSetPen, SIGNAL(triggered()), this, SLOT(setPen()));
	connect(actionCodeEdit, SIGNAL(triggered()), this, SLOT(codeEdit()));
	connect(actionDelShape, SIGNAL(triggered()), this, SLOT(del()));

	numOfFiles++;
	openasadd = false;
	pickRect = 0;
	setMouseTracking(true);
	isLeftMouseButtonPressed = false;
	server = new dataserver(this);
	QThread *thread = new QThread;
	server->moveToThread(thread);
	thread->start();
	auto type = static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection);
	connect(this, &DrawAreaWidget::undoStack_push, server, &dataserver::undoStack_push, type);
	connect(this, &DrawAreaWidget::shapes_append, server, &dataserver::shapes_append, Qt::BlockingQueuedConnection);
	connect(server, SIGNAL(update()), this, SLOT(needUpdate()));
	init();
}

void DrawAreaWidget::needUpdate()
{
	QUndoCommand* tmp = const_cast<QUndoCommand*>(undoStack.command(undoStack.index() - 1));
	if (undoStack.canUndo() //to avoid marco not end
		&& dynamic_cast<AbstractAction*>(tmp)) {
		pickedShapes = ((dynamic_cast<AbstractAction*>(tmp))->pickedShapes);
	}

	emit categoryChanged();
	m_bDelayPaint = false;
	Update();
}
//void DrawAreaWidget::setTool(Tool* tool) {
//	currentTool = tool;
//	currentTool->canvas = this;
//}

void DrawAreaWidget::codeEdit() {
	if (pickedShapes.size() == 1) {
		DeleteAction *daction = new DeleteAction();
		foreach(shared_ptr<GeneralShape> sp, pickedShapes) {
			daction->shapes.append(sp);
			daction->indexOfShapes.append(shapes.indexOf(sp));
		}
		shared_ptr<GeneralShape> sp = pickedShapes.at(0);
		shared_ptr<Combo> com = shared_ptr<Combo>(new Combo);
		com->shapes.append(sp);
		QDomDocument doc = com->toDocument();

		CodeEditDlg editor;
		editor.textEdit->setText(doc.toString());

		if (editor.exec() == QDialog::Accepted && editor.textEdit->toPlainText() != doc.toString()) {
			QDomDocument doc;
			doc.setContent("<root>" + editor.textEdit->toPlainText() + "</root>");
			QDomElement root = doc.documentElement(); //返回根节点
			shared_ptr<Combo> tmp = shared_ptr<Combo>(new Combo);//mem leak
			tmp->setShapes(root);
			if (!tmp->shapes.isEmpty()) {

				AddAction *aaction = new AddAction();
				for (int i = 1; i <= tmp->shapes.size(); i++) {
					aaction->shapes.append(tmp->shapes.at(i - 1));
				}
				QList<AbstractAction*> alist;
				alist.append(daction);
				alist.append(aaction);
				addaction(alist);
				pickedShapes.clear();
				pickedShapes = tmp->shapes;
			}
		}
	}
}


DrawAreaWidget::~DrawAreaWidget() {
	delete actionMoveToTop;
	delete actionMoveToBottom;
	delete actionSetBrush;
}
void DrawAreaWidget::printPreview() {
	QPrinter printer;

	QPrintPreviewDialog printPreviewDialog(&printer);
	printPreviewDialog.setWindowState(Qt::WindowMaximized);
	connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(onPaintRequested(QPrinter*)));
	printPreviewDialog.exec();
}
void DrawAreaWidget::onPaintRequested(QPrinter* printer) {
	QPainter painter;
	painter.begin(printer);

    double xscale = printer->pageRect(QPrinter::DevicePixel).width() / double(width());
    double yscale = printer->pageRect(QPrinter::DevicePixel).height() / double(height());
	double scale = qMin(xscale, yscale);
	zoom(scale);
    painter.translate(printer->paperRect(QPrinter::DevicePixel).x() + printer->pageRect(QPrinter::DevicePixel).width() / 2,
        printer->paperRect(QPrinter::DevicePixel).y() + printer->pageRect(QPrinter::DevicePixel).height() / 2);
	painter.translate(-width() / 2, -height() / 2);
	painter.setRenderHints(QPainter::Antialiasing |
		QPainter::TextAntialiasing |
		QPainter::SmoothPixmapTransform, true);//QPainter::TextAntialiasing,

	render(&painter);
	zoom(1 / scale);
	painter.end();
}

void DrawAreaWidget::print() {
	QPrinter printer;
	printer.setOutputFormat(QPrinter::NativeFormat);
	QPrintDialog* pd = new QPrintDialog(&printer);//&printer);
	if (pd->exec() == QDialog::Accepted) {
		// print ...
		onPaintRequested(&printer);
	}
}

void DrawAreaWidget::init()
{
	currentShape = NULL;
	pickedShapes.clear();
	pickRect = NULL;
	currentCategory = PickCategory;
	isLeftMouseButtonPressed = false;
	shapes.clear();
	dx = dy = 0;

	zoomRatio = 1;
	previouszoomRatio = 1;
	backcolor = Qt::white;
	currentMouseHanded = None;
	saveIndex = 0;
	undoStack.clear();
	filename = tr("New Pic ") + QString::number(numOfFiles, 10);

	emit categoryChanged();
}

void DrawAreaWidget::combination()
{
	if (pickedShapes.size() <= 1) return;
	shared_ptr<Combo> tmp = shared_ptr<Combo>(new Combo);//mem leak;
	CombineAction* action = new CombineAction();
	foreach(shared_ptr<GeneralShape> sp, pickedShapes) {
		tmp->shapes.append(sp);
		action->shapes.append(sp);
	}
	action->com = tmp;
	tmp->updateRange();
	pickedShapes.clear();
	pickedShapes.append(tmp);
	addaction(static_cast<AbstractAction*>(action));
}
void DrawAreaWidget::divide() {
	if (pickedShapes.size() != 1 || pickedShapes.at(0)->name() != "Combo") return;
	shared_ptr<GeneralShape> tmp = pickedShapes.at(0);
	pickedShapes.clear();
	divide(tmp);
	Update();
	emit categoryChanged();
}
void DrawAreaWidget::divide(shared_ptr<GeneralShape> shape) {
	shared_ptr<Combo> tmp = dynamic_pointer_cast<Combo>(shape);
	DivideAction* action = new DivideAction();
	action->com = tmp;
	action->indexOfCom = shapes.indexOf(shape);
	foreach(shared_ptr<GeneralShape> sp, tmp->shapes) {
		action->shapes.append(sp);
	}
	addaction(static_cast<AbstractAction*>(action));
}

void DrawAreaWidget::divideToEnd() {
	if (pickedShapes.size() != 1 || pickedShapes.at(0)->name() != "Combo") return;
	shared_ptr<GeneralShape> tmp = pickedShapes.at(0);
	pickedShapes.clear();

	divideToEnd(tmp);
	Update();
	emit categoryChanged();
}
void DrawAreaWidget::divideToEnd(shared_ptr<GeneralShape> shape) {
	if (shape->name() != "Combo") return;
	undoStack.beginMacro("divideToEnd Combo");
	pickedShapes.removeOne(shape);
	divide(shape);
	foreach(shared_ptr<GeneralShape> sp, dynamic_pointer_cast<Combo>(shape)->shapes) {
		divideToEnd(sp);
	}
	undoStack.endMacro();
}


void DrawAreaWidget::copyPaste() {

	if (pickedShapes.size() > 0) {
		QList<shared_ptr<GeneralShape>> copiedshapes;
		foreach(shared_ptr<GeneralShape> sp, pickedShapes) {
			shared_ptr<GeneralShape>tmp = sp->copyPaste();
			tmp->drag(QPointF(10, 10));
			shapes.append(tmp);
			copiedshapes.append(tmp);
		}

		pickedShapes = copiedshapes;
		expand(); Update();
	}
}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::copy() {
	//qDebug()<<"copy";
	QList<shared_ptr<GeneralShape>> copiedshapes;
	if (pickedShapes.size() > 0) {
		foreach(shared_ptr<GeneralShape> sp, pickedShapes) {
			shared_ptr<GeneralShape>tmp = sp->copyPaste();
			copiedshapes.append(tmp);
		}
	}
	return copiedshapes;
}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::cut() {
	//qDebug()<<"cut";
	QList<shared_ptr<GeneralShape>> copiedshapes;
	if (pickedShapes.size() > 0) {
		DeleteAction *action = new DeleteAction();
		foreach(shared_ptr<GeneralShape> sp, pickedShapes) {
			shared_ptr<GeneralShape>tmp = sp->copyPaste();
			action->shapes.append(sp);
			action->indexOfShapes.append(shapes.indexOf(sp));
			copiedshapes.append(tmp);
		}
		addaction(static_cast<AbstractAction*>(action));
		pickedShapes.clear();
		Update();
	}
	return copiedshapes;
}
void DrawAreaWidget::paste(QList<shared_ptr<GeneralShape>>*copyShapes) {

	if (copyShapes->size() > 0) {
		QList<shared_ptr<GeneralShape>> copiedshapes;
		AbstractAction *action = new AddAction();
		for (int i = 1; i <= copyShapes->size(); i++) {
			shared_ptr<GeneralShape>tmp = copyShapes->at(i - 1)->copyPaste();
			tmp->drag(QPointF(10, 10));
			action->shapes.append(tmp);

			copiedshapes.append(tmp);
			copyShapes->removeAt(i - 1);
			copyShapes->insert(i - 1, tmp);
		}
		addaction(action);
		pickedShapes = copiedshapes;
		expand(); Update();
	}
	emit categoryChanged();
}

void DrawAreaWidget::moveToTop() {
	if (pickedShapes.size() == 0) return;
	TopAction* action = new TopAction();
	action->shapes = pickedShapes;
	addaction(static_cast<AbstractAction*>(action));
	Update();
}

void DrawAreaWidget::moveToBottom() {
	if (pickedShapes.size() == 0) return;
	BottomAction* action = new BottomAction();
	for (int i = pickedShapes.size() - 1; i >= 0; i--) {
		action->shapes.append(pickedShapes.at(i));
	}
	addaction(static_cast<AbstractAction*>(action));
	Update();
}

void DrawAreaWidget::addfile() {
	openasadd = true;
	open();
}
void DrawAreaWidget::opennew() {
	openasadd = false;
	open();
}

bool DrawAreaWidget::maybeSave() {//成功保存，或明确放弃保存，或根本没什么可以保存的，返回true。
	if (saveIndex == undoStack.index()) return true;
	QMessageBox msgBox;
	msgBox.setText(tr("Unsaved changes in file: \"") + filename + tr("\" will be lost!"));
	msgBox.setInformativeText(tr("Do you want to save your changes?"));
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	//QMessageBox::Discard
	int ret = msgBox.exec();
	switch (ret) {
	case QMessageBox::Save: {
		save();
		if (!saved) return false;
		else return true;
		// Save was clicked
		break;
	}
	case QMessageBox::Discard: {
		return true;
		// Don't Save was clicked
		break;
	}
	case QMessageBox::Cancel: {
		return false;
		// Cancel was clicked
		break;
	}
	default:
		// should never be reached
		break;
	}
	return true;
}

void DrawAreaWidget::openfile(QString file) {
	QFile openFile(file);

	if (!openFile.open(QFile::ReadOnly))
		return;

	QDomDocument doc;
	if (!doc.setContent(&openFile))
	{
		openFile.close();
		QMessageBox msg;
		msg.setText(tr("The document is unknown."));
		msg.exec();
		return;
	}
	openFile.close();

	QDomElement root = doc.documentElement(); //返回根节点
	QDomElement e = root.toElement();
	if (e.nodeName() != "shapes") {
		QMessageBox msg;
		msg.setText(tr("The XML_document is unknown."));
		msg.exec();
		return;
	}
	if (e.hasAttribute("red")) {
		int r, g, b;
		r = e.attribute("red").toInt();
		g = e.attribute("green").toInt();
		b = e.attribute("blue").toInt();
		backcolor = QColor(r, g, b);
	}
	////
	shared_ptr<Combo> tmp = shared_ptr<Combo>(new Combo);//mem leak
	tmp->setShapes(root);

	if (openasadd) {
		AbstractAction* action = new AddAction;
		action->shapes = static_pointer_cast<Combo>(tmp->copyPaste())->shapes;
		if (action->shapes.size() > 0) addaction(action);
		openasadd = false;
	}
	else {
		filename = file;
		shapes = static_pointer_cast<Combo>(tmp->copyPaste())->shapes;
		emit categoryChanged();
	}
	// tmp;
	expand(); Update();
}


void DrawAreaWidget::open() {
	if (!openasadd) {
		if (!maybeSave()) return;
	}

	QStringList files;
	QFileDialog dlg(this, "Open file");
	dlg.setAcceptMode(QFileDialog::AcceptOpen);
	dlg.setNameFilter("XML (*.xml   *.ini   *.inf)");
	dlg.exec();
	if (dlg.result() == QDialog::Accepted)
		files = dlg.selectedFiles();
	else
		return;
	if (files.isEmpty())
		return;
	if (!openasadd) init();
	openfile(files.at(0));

}



void DrawAreaWidget::saveAs() {
	saved = false;
	QStringList files;
	QFileDialog dlg(this, "Save file");
	dlg.setAcceptMode(QFileDialog::AcceptSave);
	dlg.setNameFilter("XML (*.xml   *.inf   *.ini )");
	dlg.exec();
	if (dlg.result() == QDialog::Accepted)
		files = dlg.selectedFiles();
	else
		return;
	if (files.isEmpty())    return;
	QFile saveFile(files.at(0));
	if (!saveFile.open(QFile::WriteOnly | QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
		return;
	QDomDocument doc;
	QDomProcessingInstruction instruction; //添加处理命令
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);
	//添加根节点

	Combo* tmp = new Combo;
	tmp->shapes = shapes;
	QDomElement draw = tmp->toElement();
	//QDomElement draw=doc.createElement("draw");
	draw.setAttribute("red", backcolor.red());
	draw.setAttribute("green", backcolor.green());
	draw.setAttribute("blue", backcolor.blue());
	doc.appendChild(draw);

	QTextStream out_stream(&saveFile);
	doc.save(out_stream, 4); //缩进4格
	saveFile.close();

	saved = true;
	//changed=false;
	saveIndex = undoStack.index();
	filename = files.at(0);
	emit categoryChanged();

}

void DrawAreaWidget::save() {
	saved = false;
	QString savefilename = filename;

	if (!QFile(savefilename).exists()) {

		QStringList files;
		QFileDialog dlg(this, "Save file");
		dlg.setAcceptMode(QFileDialog::AcceptSave);
		dlg.setNameFilter("XML (*.xml   *.inf   *.ini )");
		dlg.exec();
		if (dlg.result() == QDialog::Accepted)
			files = dlg.selectedFiles();
		else
			return;
		if (files.isEmpty())    return;

		savefilename = files.at(0);
	}
	QFile saveFile(savefilename);
	if (!saveFile.open(QFile::WriteOnly | QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
		return;
	QDomDocument doc;
	QDomProcessingInstruction instruction; //添加处理命令
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);
	//添加根节点

	Combo* tmp = new Combo;
	tmp->shapes = shapes;
	QDomElement draw = tmp->toElement();
	//QDomElement draw=doc.createElement("draw");
	draw.setAttribute("red", backcolor.red());
	draw.setAttribute("green", backcolor.green());
	draw.setAttribute("blue", backcolor.blue());
	doc.appendChild(draw);

	QTextStream out_stream(&saveFile);
	doc.save(out_stream, 4); //缩进4格
	saveFile.close();

	saved = true;
	//changed=false;
	saveIndex = undoStack.index();
	filename = savefilename;
	emit categoryChanged();
}


void DrawAreaWidget::fitcanvas() {
	//zoom();
	if (shapes.size() == 0) return;
	//qDebug()<<"shapes.size()="<<shapes.size();
	qreal MINX, MINY, MAXX, MAXY;
	MINX = shapes.at(0)->left;
	MINY = shapes.at(0)->top;
	MAXX = shapes.at(0)->right;
	MAXY = shapes.at(0)->bottom;
	foreach(shared_ptr<GeneralShape> sp, shapes) {
		MINX = min(MINX, sp->left);
		MINY = min(MINY, sp->top);
		MAXX = max(MAXX, sp->right);
		MAXY = max(MAXY, sp->bottom);
	}
	MINX = MINX*zoomRatio;
	MINY = MINY*zoomRatio;
	MAXX = MAXX*zoomRatio;
	MAXY = MAXY*zoomRatio;
	dx = -MINX;
	dy = -MINY;
	setFixedSize(MAXX - MINX, MAXY - MINY);
	realSize = size() / zoomRatio;
	realDp = QPoint(dx, dy) / zoomRatio;
	Update();
}

void DrawAreaWidget::paintEvent(QPaintEvent *)
{
	QPalette palette;

	palette.setColor(QPalette::Base, QColor(backcolor));

	this->setPalette(palette);
	//expand();
	QPainter painter(this);
	//抗锯齿
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);//QPainter::TextAntialiasing,
	painter.translate(dx, dy);
	foreach(shared_ptr<GeneralShape> sp, shapes) {
		sp->draw(painter, zoomRatio);
		if (pickedShapes.indexOf(sp) >= 0) {
			sp->drawClosure(painter, zoomRatio);
		}
	}

	if (pickRect != NULL) {
		QPen pen;  // creates a default pen

		pen.setStyle(Qt::DashDotLine);
		pickRect->setPen(pen);
		pickRect->draw(painter, zoomRatio);
	}
	if (m_curIShapeEditor && shapes.indexOf(m_curIShapeEditor->shape()) >= 0) {
		m_curIShapeEditor->drawControlPoints(painter, zoomRatio);
	}
	painter.translate(-dx, -dy);
	painter.end();
	emit statusChanged();
}
void DrawAreaWidget::contextMenuEvent(QContextMenuEvent * event) {
	QPointF realPoint = (event->pos() - QPointF(dx, dy)) / zoomRatio;
	if (currentCategory == PickCategory &&   pickedShapes.size() == 1) {
		if (m_curIShapeEditor->contextMenu(event)) {
			Update();
			return;
		}
		QMenu *menu = new QMenu();

		menu->addAction(actionMoveToTop);

		menu->addAction(actionMoveToBottom);

		menu->addAction(actionSetBrush);
		menu->addAction(actionSetPen);
		//add action from custom editor
		for (auto act : m_curIShapeEditor->QActions(realPoint, event))
		{
			menu->addAction(act);
		}
		menu->addAction(actionCodeEdit);
		menu->addAction(actionDelShape);
		menu->exec(event->globalPos());
		delete menu;
	}

}
void DrawAreaWidget::editText() {
	if (pickedShapes.size() == 1) {
		if (pickedShapes.at(0)->name() == "Text") {
			shared_ptr<Text > pm = dynamic_pointer_cast<Text>(pickedShapes.at(0));
			TextEditAction* action = new TextEditAction;
			action->shapes.append(pm);
			action->oldfont = pm->myfont;
			action->newfont = action->oldfont;
			action->oldtext = pm->mytext;
			action->newtext = action->oldtext;
			EditTextDialog* etd = new EditTextDialog;
			etd->tw->fdlg->setCurrentFont(pm->myfont);
			etd->tw->gtdlg->setText(pm->mytext);
			if (etd->exec() == QDialog::Accepted) {
				action->newfont = etd->tw->fdlg->currentFont();
				action->newtext = etd->tw->gtdlg->text;

				if (action->newfont != action->oldfont || action->newtext != action->oldtext) {
					addaction(action);
				}
				else {
					delete action;
				}
			}
			delete etd;
		}
	}
}

void DrawAreaWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	//qDebug()<<"mouseDoubleClickEvent";
	if (currentCategory != PickCategory) {
		mousePressEvent(event);
		return;
	}
	if (currentCategory == PickCategory) {
		if (pickedShapes.size() == 1) {
			//editText();

			setBrush();


		}
		else {//设置背景颜色
			QColorDialog* gtd = new QColorDialog;
			//qDebug()<<pickedShapes.at(0)->brush;
			gtd->setCurrentColor(backcolor);
			if (gtd->exec() == QDialog::Accepted) {
				backcolor = gtd->selectedColor();
			}
			//pickedShapes.at(0)->setBrush(gtd->getColor());
			Update();
		}
	}
}
void DrawAreaWidget::setBrush() {
	if (pickedShapes.size() == 1) {
		setBrush(pickedShapes.at(0));
	}
}
void DrawAreaWidget::setPen() {
	if (pickedShapes.size() == 1) {
		setPen(pickedShapes.at(0));
	}
}
void DrawAreaWidget::setPen(shared_ptr<GeneralShape> sp) {
	QColorDialog* gtd = new QColorDialog;
	//qDebug()<<pickedShapes.at(0)->brush;
	//gtd->setCurrentColor(sp->brush.color());
	//if (sp->brush.style()!=Qt::NoBrush){
	gtd->setCurrentColor(sp->pen.color());
	//}
	SetPenAction* action = new SetPenAction;
	action->oldpen = sp->pen;
	action->shapes.append(sp);
	action->allShapes = &shapes;
	if (gtd->exec() == QDialog::Accepted) {
		//sp->setBrush(gtd->selectedColor() );
		action->newpen = gtd->selectedColor();
		addaction(action);
	}
	else {
		delete action;
	}
	delete gtd;
	//pickedShapes.at(0)->setBrush(gtd->getColor());
	Update();
}
void DrawAreaWidget::setBrush(shared_ptr<GeneralShape> sp) {
	QColorDialog* gtd = new QColorDialog;
	//qDebug()<<pickedShapes.at(0)->brush;
	//gtd->setCurrentColor(sp->brush.color());
	if (sp->brush.style() != Qt::NoBrush) {
		gtd->setCurrentColor(sp->brush.color());
	}
	SetBrushAction* action = new SetBrushAction;
	action->oldbrush = sp->brush;
	action->shapes.append(sp);
	action->allShapes = &shapes;
	if (gtd->exec() == QDialog::Accepted) {
		//sp->setBrush(gtd->selectedColor() );
		action->newbrush = gtd->selectedColor();
		addaction(action);
	}
	else {
		delete action;
	}
	delete gtd;
	//pickedShapes.at(0)->setBrush(gtd->getColor());
	Update();
}
void DrawAreaWidget::rightMousePressEvent(QMouseEvent *event) {

}


void DrawAreaWidget::mousePressEvent(QMouseEvent *event)
{
	QPointF realPoint = (event->pos() - QPointF(dx, dy)) / zoomRatio;
	m_bIsDuringOperation = true;
	grabMouse();
	//qDebug()<<__FUNCTION__;
	if (event->button() == Qt::LeftButton) { //return;
		isLeftMouseButtonPressed = true;
	}

	switch (currentCategory)
	{
	case PalmCategory: {
		//setMouseTracking(false);
		startCursorPoint = event->pos();
		endCursorPoint = event->pos();
		setCursor(Qt::ClosedHandCursor);
		break;
	}
	case PickCategory:
	{
		if (event->button() == Qt::RightButton) {
			pickedShapes.clear();
		}

		if (pickedShapes.size() > 0) {//rotate or scale pickedShape
			initIShapeEditor();
			if (m_curIShapeEditor->mouseDown(realPoint, isLeftMouseButtonPressed, event)) break;
			else {
				m_curIShapeEditor = nullptr;
			}
		}

		if (pickShape(realPoint).size() == 0) {//框选模式
			pickedShapes.clear();
			//qDebug()<<"pickedShape==NULL";
			if (!isLeftMouseButtonPressed) break;
			startPoint = realPoint;
			// pickRect;
			pickRect = shared_ptr<Rect>(new Rect);//mem leak
			pickRect->appendPoint(realPoint);
			pickRect->appendPoint(realPoint);
		}
		else {
			if (pickedShapes.indexOf(pickShape(realPoint).at(0)) < 0) {//另选并拖动
				pickedShapes = pickShape(realPoint);
			}
			initIShapeEditor();
			m_curIShapeEditor->mouseDown(realPoint, true, event);
			endPoint = startPoint = realPoint;
		}
		break;
	}
	case FaceCategory:
	case RectCategory:
	case EllipseCategory:
	case CircleCategory:
	case TextCategory:
	case CloseCurveCategory:
	case CurveCategory: {

		m_curIShapeBuilder->mouseDown(realPoint);
		break;
	}
	}
	Update();
}
void DrawAreaWidget::addaction(AbstractAction* act) {
	act->allShapes = &shapes;
	act->setText(act->name() + " " + (act->shapes.size() > 1 ?
		tr("multipal shapes")
		:
		act->shapes.at(0)->name()));
	if (act->name() == "Divide") {
		act->setText(act->name() + " Combo");
	}
	//act->pickedShapes=&pickedShapes;
	m_bDelayPaint = true;

	emit undoStack_push(act);
}
void DrawAreaWidget::addaction(QList<AbstractAction*> alist) {
	undoStack.beginMacro("CodeEdit " + alist.at(0)->shapes.at(0)->name());
	foreach(AbstractAction* act, alist) {
		act->allShapes = &shapes;
		//act->pickedShapes=&pickedShapes;
		undoStack.push(act);//
	}


	undoStack.endMacro();
	Update();
	emit categoryChanged();

}

void DrawAreaWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_bIsDuringOperation = false;
	releaseMouse();
	QPointF realPoint = (event->pos() - QPointF(dx, dy)) / zoomRatio;
	//qDebug()<<__FUNCTION__;
	isLeftMouseButtonPressed = false;

	m_bDelayPaint = false;
	switch (currentCategory)
	{
	case PolygonCategory:
	case PolylineCategory:
	case FaceCategory:
	case EllipseCategory:
	case RectCategory:
	case CircleCategory:
	case TextCategory:
	case CloseCurveCategory:
	case CurveCategory: {
		m_curIShapeBuilder->mouseUp(realPoint, event->button() == Qt::LeftButton);
		break;
	}
	case PalmCategory: {
		//startCursorPoint=endCursorPoint;
		endCursorPoint = event->pos();
		int vvalue = vBar->value();
		int hvalue = hBar->value();
		vBar->setValue(vvalue - (endCursorPoint - startCursorPoint).y());
		hBar->setValue(hvalue - (endCursorPoint - startCursorPoint).x());
		setCursor(Qt::OpenHandCursor);
		Update();
		break;
	}
	case PickCategory: {

		if (pickedShapes.size() == 0) {//框选
			if (pickRect == 0) break;
			pickRect = NULL;
			endPoint = realPoint;
			pickedShapes = pickShape(startPoint, endPoint);
			break;
		}
		if (m_curIShapeEditor) {
			m_curIShapeEditor->mouseUp(realPoint);
			//m_curIShapeEditor = nullptr;
			break;
		}
		if (pickedShapes.size() > 0) {
			EditAction* action = new EditAction();
			action->shapes = pickedShapes;
			action->dpoint = endPoint - startPoint;
			//AbstractAction* tmp=static_cast<AbstractAction*>(action);
			if (action->dpoint != QPointF(0, 0)) {
				//tmp->pickedShapes=&pickedShapes;
				action->m_bHasDone = true;
				addaction(action);
			}
			else {
				delete action;
			}
			endPoint = startPoint;
			break;
		}
		break;
	}
	}
	Update();
	emit categoryChanged();
}
void DrawAreaWidget::keyPressEvent(QKeyEvent *event) {
	qDebug() << "DrawAreaWidget " << __FUNCTION__ << event->text() << event->key();
	if (m_bIsDuringOperation) return;
	if (m_curIShapeEditor) {
		if (m_curIShapeEditor->keyDown(event))
		{
			Update();
			return;
		}
	}
	if (event->matches(QKeySequence::Undo))
	{
		undo();
		return;
	}
	if (event->matches(QKeySequence::Redo))
	{
		redo();
		return;
	}

	if (pickedShapes.size() > 0) {
		if (event->key() == Qt::Key_Delete) {
			del();
			return;
		}

		switch (event->key()) {
		case Qt::Key_W:
		case Qt::Key_S:
		case Qt::Key_A:
		case Qt::Key_D: {
			SlightMoveAction* action = new SlightMoveAction();
			action->shapes = pickedShapes;
			if (event->key() == Qt::Key_W) {
				action->dpoint = QPointF(0, -1 / zoomRatio);
			}
			else if (event->key() == Qt::Key_S) {
				action->dpoint = QPointF(0, 1 / zoomRatio);
			}
			else if (event->key() == Qt::Key_A) {
				action->dpoint = QPointF(-1 / zoomRatio, 0);
			}
			else if (event->key() == Qt::Key_D) {
				action->dpoint = QPointF(1 / zoomRatio, 0);
			}

			if (!event->isAutoRepeat()) {
				action->_id = -1;
			}
			addaction(action);
		}
		}
	}
	QWidget::keyPressEvent(event);
	event->ignore();
}
void DrawAreaWidget::keyReleaseEvent(QKeyEvent *event) {
	//qDebug()<<"DrawAreaWidget "<<__FUNCTION__<<event->text()<<event->key();
	QWidget::keyReleaseEvent(event);
	event->ignore();

}

void DrawAreaWidget::del() {
	DeleteAction *action = new DeleteAction();
	foreach(shared_ptr<GeneralShape> sp, pickedShapes) {
		action->shapes.append(sp);
		action->indexOfShapes.append(shapes.indexOf(sp));
	}
	addaction(static_cast<AbstractAction*>(action));
	pickedShapes.clear();
	Update();
}
void DrawAreaWidget::leaveEvent(QEvent *event) {
	emit mouseLeave();
}

void DrawAreaWidget::mouseMoveEvent(QMouseEvent *event)
{
	//qDebug()<<__FUNCTION__;
	QPointF realPoint = (event->pos() - QPointF(dx, dy)) / zoomRatio;

	switch (currentCategory)
	{
	case PolygonCategory:
	case PolylineCategory:
	case FaceCategory:
	case RectCategory:
	case EllipseCategory:
	case CircleCategory:
	case TextCategory:
	case CloseCurveCategory:
	case CurveCategory: {
		m_curIShapeBuilder->mouseMove(realPoint);
		break;
	}
	case PalmCategory: {
		if (!isLeftMouseButtonPressed) break;
		//startCursorPoint=endCursorPoint;
		//鼠标移动了，画布暂时还没跟上。
		endCursorPoint = event->pos();


		vBar->setValue(vBar->value() - (endCursorPoint - startCursorPoint).y());
		hBar->setValue(hBar->value() - (endCursorPoint - startCursorPoint).x());
		//滚动条拖动后鼠标在窗口的位置没变，但在画布上的位置变了
		//startCursorPoint=pos();
		realPoint = (startCursorPoint - QPointF(dx, dy)) / zoomRatio;
		Update();
		break;
	}
	case PickCategory: {
		if (isLeftMouseButtonPressed) {

			if (pickedShapes.size() == 0) {//框选模式
				pickRect->removeLastPoint();
				pickRect->appendPoint(realPoint);
				//expand();
				Update();
				break;
			}
			if (m_curIShapeEditor) {
				m_curIShapeEditor->mouseMove(realPoint, true, event);
				break;
			}

			if (pickedShapes.size() > 0) {

				foreach(shared_ptr<GeneralShape> sp, pickedShapes) {
					sp->drag(realPoint - endPoint);
				}
				endPoint = realPoint;

				Update();
				break;
			}
		}

	}
	}
	Update();
	emit mouseMoved(event, realPoint);
}
void DrawAreaWidget::Update() {
	if (!m_bDelayPaint)update();
}

void DrawAreaWidget::setCategory(Category c) {
	//setMouseTracking(false);
	finishcurrentShape();
	//currentLine->hide();
	if (c == PalmCategory) {
		setCursor(Qt::OpenHandCursor);
	}
	else {
		setCursor(Qt::ArrowCursor);
	}
	if (c != PickCategory) {
		pickedShapes.clear();
		Update();
	}
	currentCategory = c;
	m_curIShapeBuilder = getBuilderFrom(c);
	if (m_curIShapeBuilder)
	{
		CallBack* cb = new CallBack();
		cb->m_canvas = this;
		m_curIShapeBuilder->setCallBack(cb);
	}
	//mInstrumentHandler=CurveLineInstrument;
	emit categoryChanged();
}
void DrawAreaWidget::finishcurrentShape() {
	if (currentShape != NULL) {
		shapes.removeLast();
		if (currentShape->isEmpty()) {


			// currentShape;
		}
		else {
			AbstractAction *action = new AddAction;//(currentShape);
			action->shapes.append(currentShape);
			addaction(action);
		}
	}

	currentShape = NULL;
	//m_curIShapeBuilder = nullptr;
	Update();
}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::pickShape(QPointF point) {

	QList<shared_ptr<GeneralShape>> tmp;
	for (int i = shapes.size() - 1; i >= 0; i--) {//这种方式选取顺序为从上面往底下。
		if (shapes.at(i)->minDistance(point)*zoomRatio < 4) {
			tmp.append(shapes.at(i));
			break;
		}
	}
	return tmp;
}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::pickShape(QPointF p0, QPointF p1) {
	QList<shared_ptr<GeneralShape>> tmp;
	foreach(shared_ptr<GeneralShape> sp, shapes) {
		if (sp->inRange(p0, p1)) tmp.append(sp);
	}
	return tmp;
}
void DrawAreaWidget::zoomone() {
	zoomRatio = 1;
	dozoom();
}

void DrawAreaWidget::dozoom() {
	if ((width()*zoomRatio / previouszoomRatio)*(height()*zoomRatio / previouszoomRatio) > 4
		&& (width()*zoomRatio / previouszoomRatio)*(height()*zoomRatio / previouszoomRatio) < 10000 * 10000) {


		//    double pdx=dx;
		//    double pdy=dy;

		dx = (realDp*zoomRatio).x();
		dy = (realDp*zoomRatio).y();
		double hv = hBar->value();
		double vv = vBar->value();

		setFixedSize(realSize*zoomRatio); /*width()*zoomRatio/previouszoomRatio ,
					   height()*zoomRatio/previouszoomRatio);*/
		hBar->setValue((hv + windowwidth / 2)*zoomRatio / previouszoomRatio - windowwidth / 2);
		vBar->setValue((vv + windowheight / 2)*zoomRatio / previouszoomRatio - windowheight / 2);
		//expand();
		Update();
		previouszoomRatio = zoomRatio;
	}
	else {
		zoomRatio = previouszoomRatio;
	}
}
void DrawAreaWidget::zoom(double ratio) {
	zoomRatio = zoomRatio*ratio;
	dozoom();

}

void DrawAreaWidget::undo() {
	undoStack.undo();
	QUndoCommand* tmp = const_cast<QUndoCommand*>(undoStack.command(undoStack.index()));
	if (dynamic_cast<AbstractAction*>(tmp)) {
		pickedShapes = ((dynamic_cast<AbstractAction*>(tmp))->pickedShapes);
		if (pickedShapes.isEmpty() && undoStack.index() > 0) {
			QUndoCommand* tmp = const_cast<QUndoCommand*>(undoStack.command(undoStack.index() - 1));
			if (dynamic_cast<AbstractAction*>(tmp)) {
				pickedShapes = (dynamic_cast<AbstractAction*>(tmp))->pickedShapes;
			}
		}
	}
	else {
		pickedShapes.clear();
	}
	emit categoryChanged();
	Update();
}
void DrawAreaWidget::redo() {
	undoStack.redo();
	QUndoCommand* tmp = const_cast<QUndoCommand*>(undoStack.command(undoStack.index() - 1));
	if (dynamic_cast<AbstractAction*>(tmp)) {
		pickedShapes = ((dynamic_cast<AbstractAction*>(tmp))->pickedShapes);
	}
	else {
		pickedShapes.clear();
	}

	emit categoryChanged();
	Update();
}
void DrawAreaWidget::expand() {
	foreach(shared_ptr<GeneralShape> sp, shapes) {
		sp->updateBand();
		int ddx, ddy;//
		ddx = ddy = 0;
		if (sp->left*zoomRatio < -dx) { ddx = -sp->left*zoomRatio - dx; dx = dx + ddx; }
		if (sp->top*zoomRatio < -dy) { ddy = -sp->top*zoomRatio - dy; dy = dy + ddy; }
		setFixedSize(width() + ddx, height() + ddy);
		vBar->setValue(vBar->value() + ddy);
		hBar->setValue(hBar->value() + ddx);

		if (sp->right*zoomRatio + dx > width()) setFixedWidth(sp->right*zoomRatio + dx);
		if (sp->bottom*zoomRatio + dy > height()) setFixedHeight(sp->bottom*zoomRatio + dy);
	}
	realSize = size() / zoomRatio;
	realDp = QPoint(dx, dy) / zoomRatio;
}

bool DrawAreaWidget::inRange(QPointF Point, QList<shared_ptr<GeneralShape>> sps) {
	foreach(shared_ptr<GeneralShape> sp, sps) {
		if (sp->minDistance(Point)*zoomRatio < 4) return true;
	}
	return false;
}
void DrawAreaWidget::test() {
	//    pickedShapes.at(0)->zoom(2);
	//    Update();
}

void DrawAreaWidget::creatShape()
{
	currentShape = m_curIShapeBuilder->shape();
	emit shapes_append(currentShape);
}

void DrawAreaWidget::finishShape()
{
	finishcurrentShape();
}

void DrawAreaWidget::editShape()
{
	addaction(m_curIShapeEditor->action());
}

shared_ptr<IShapeBuilder> DrawAreaWidget::getBuilderFrom(Category c)
{
	return getBuilder(c);
}

void DrawAreaWidget::initIShapeEditor() {
    updateIShapeEditor(pickedShapes, m_curIShapeEditor);
	
	m_curIShapeEditor->zoomRatio() = zoomRatio;
	CallBack* cb = new CallBack();
	cb->m_canvas = this;
	m_curIShapeEditor->setCallBack(cb);
}
