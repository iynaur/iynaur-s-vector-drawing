#include "TextEditor.h"
#include "text.h"
#include "editaction.h"
#include "texteditaction.h"
#include "edittextdialog.h"
TextEditor::TextEditor()
{
	currentLine = new MyLineEdit;
	//    currentLine->show();
	//    currentLine->lower();
	currentRotateLine = new RotateWidget;
	//currentLine->setParent(this);
	//currentRotateLine->setParent(this);
	currentRotateLine->setBaseWidget(currentLine);
	connect(currentLine, SIGNAL(textChanged(QString)), this, SLOT(currentTextChanged(QString)));
	m_isEditText = false;
	m_hasPicked = false;
}
void TextEditor::currentTextChanged(QString s) {
	TextEditAction* action = new TextEditAction;
	action->shapes.append(m_shape);
	auto currentText = dynamic_pointer_cast<Text>(m_shape);
	action->oldfont = currentText->myfont;
	action->newfont = action->oldfont;
	action->oldtext = currentText->mytext;
	action->newtext = currentLine->text();
	m_action = action;
	if (action->oldtext != action->newtext) publishEditFinished();
}

TextEditor::~TextEditor()
{
	delete currentLine;
	delete currentRotateLine;
}

bool TextEditor::contextMenu(QContextMenuEvent * event)
{
	if (m_isEditText) {
		currentLine->contextMenuEvent(event);
		return true;
	}
	else {
		return false;
	}
}

QList<QAction*> TextEditor::QActions(QPointF p, QContextMenuEvent * event)
{
	auto actionEditText = new QAction(tr("EditText"), this);
	actionEditText->setIcon(QIcon(QString::fromUtf8("./image/applixware.png")));
	connect(actionEditText, SIGNAL(triggered()), this, SLOT(editText()));
	return QList<QAction*>({ actionEditText });
}

void TextEditor::editText()
{
	shared_ptr<Text > pm = dynamic_pointer_cast<Text>(m_shape);
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
			m_action = action;
			publishEditFinished();
		}
		else {
			delete action;
		}
	}
	delete etd;
}

shared_ptr<GeneralShape>& TextEditor::shape()
{
	return m_shape;
}

AbstractAction *& TextEditor::action()
{
	return m_action;
}

double & TextEditor::zoomRatio()
{
	return m_zoomRatio;
}

bool TextEditor::mouseDown(QPointF realPoint, bool bLeft, QMouseEvent * event)
{
	if (m_isEditText ) {
		if (m_shape->minDistance(realPoint) == 0) {
			currentRotateLine->mousePress(event, m_zoomRatio);
			return 1;
		}
		else {
			m_isEditText = false;
		}
	}
	m_hasPicked++;
	if (isRotationHandlePoint(realPoint, m_shape, m_zoomRatio) && m_hasPicked>1) {//rotate pickedShape

																 //qDebug()<<"rotate pickedShape";
		currentMouseHanded = RotationPoint;
		endPoint = startPoint = realPoint;
		pickedShapestartRotationangle = m_shape->Rotationangle;
		return 1;

	}
	else if (scaleHandlePointId(realPoint, m_shape, m_zoomRatio)!=-1 && m_hasPicked>1) {//scale pickedShape
																   //qDebug()<<"scale pickedShape";
		startScale(realPoint);
		return 1;
	}
	else {
		currentMouseHanded = None;
		endPoint = startPoint = realPoint;
		//edit text
		if (m_shape->minDistance(realPoint) == 0) {
			if (m_hasPicked==1) {
				return 1;
			}
			m_isEditText = true;
			auto currentText = dynamic_pointer_cast<Text>(m_shape);
			currentLine->setText(currentText->mytext);
			return 1;
		}//end edit
		else {
			m_isEditText = false;
			return 0;
		}
	}
}

void TextEditor::mouseMove(QPointF p, bool bLeft, QMouseEvent * event)
{
	if (m_isEditText/* && m_shape->minDistance(p) == 0*/) {
		//currentLine->grabMouse();
		currentRotateLine->mouseMove(event, m_zoomRatio);
		//currentLine->releaseMouse();
	}
	else {
		if (currentMouseHanded == RotationPoint) {//Rotation
												  //qDebug()<<"RotationPoint";
			QPointF centralpoint = QPointF((m_shape->minx + m_shape->maxx) / 2,
				(m_shape->miny + m_shape->maxy) / 2);

			qreal sita = acos(-(p.y() - centralpoint.y()) / sqrt(
				(p.x() - centralpoint.x())*
				(p.x() - centralpoint.x()) +
				(p.y() - centralpoint.y())*
				(p.y() - centralpoint.y())));
			if (p.x() - centralpoint.x() < 0) {
				sita = -sita;
			}
			if (m_shape->getsy() < 0) {
				sita = sita + M_PI;//解决图形倒置时的bug
			}
			m_shape->setRotationangle(sita / M_PI * 180);
			return;
		}
		else if (currentMouseHanded == ScalePoint) {//scale,NEED REWRITE!!!
													//qDebug()<<"ScalePoint";
			doScale(p);
			return;
		}
		else {

			m_shape->drag(p - endPoint);
			endPoint = p;
			return;
		}
	}
}

void TextEditor::mouseUp(QPointF realPoint, bool bLeft, QMouseEvent * event)
{
	if (m_isEditText && m_shape->minDistance(realPoint) == 0) {
		//currentLine->grabMouse();
		//currentRotateLine->mouseMove(event,zoomRatio);
		//currentLine->releaseMouse();
		return;
	}
	else {
		if (currentMouseHanded == RotationPoint) {//Rotation
			currentMouseHanded = None;
			//qDebug()<<"RotationPoint";
			QPointF centralpoint = QPointF((m_shape->minx + m_shape->maxx) / 2,
				(m_shape->miny + m_shape->maxy) / 2);

			qreal sita = acos(-(realPoint.y() - centralpoint.y()) / sqrt(
				(realPoint.x() - centralpoint.x())*
				(realPoint.x() - centralpoint.x()) +
				(realPoint.y() - centralpoint.y())*
				(realPoint.y() - centralpoint.y())));
			if (realPoint.x() - centralpoint.x() < 0) {
				sita = -sita;
			}
			if (m_shape->getsy() < 0) {
				sita = sita + M_PI;//解决图形倒置时的bug
			}
			m_shape->setRotationangle(sita / M_PI * 180);

			EditAction* action = new EditAction();
			action->shapes.append(m_shape);
			action->oldRotationangle = pickedShapestartRotationangle;
			action->newRotationangle = m_shape->Rotationangle;
			AbstractAction* tmp = static_cast<AbstractAction*>(action);
			//tmp->pickedShapes=&pickedShapes;
			tmp->m_bHasDone = true;
			m_action = tmp;
			publishEditFinished();
			//expand();
			//Update();
			return;
		}
		else if (currentMouseHanded == ScalePoint) {//scale,NEED REWRITE!!!
			finishScale(realPoint);
			return;
		}
		else {
			EditAction* action = new EditAction();
			action->shapes.append(m_shape);
			action->dpoint = endPoint - startPoint;
			//AbstractAction* tmp=static_cast<AbstractAction*>(action);
			if (action->dpoint != QPointF(0, 0)) {
				//tmp->pickedShapes=&pickedShapes;
				action->m_bHasDone = true;
				m_action = action;
				publishEditFinished();
			}
			else {
				delete action;
			}
			endPoint = startPoint;
			return;
		}
	}
}

bool TextEditor::keyDown(QKeyEvent * event)
{
	if (m_isEditText) {
		currentLine->keyPressEvent(event);
		return true;
	}
	else {
		return false;
	}
}

bool TextEditor::keyUp(QKeyEvent * event)
{
	return false;
}

void TextEditor::publishEditFinished()
{
	m_callBack->call("editFinished");
}

void TextEditor::setCallBack(ICallBack * callBack)
{
	m_callBack = callBack;
}

void TextEditor::drawControlPoints(QPainter & painter, qreal zoomRatio)
{
	if (!m_isEditText) return;
	auto currentText = dynamic_pointer_cast<Text>(m_shape);
	if (currentText->mytext != currentLine->text()) currentLine->setText(currentText->mytext);


	shared_ptr<Text> tmp = dynamic_pointer_cast<Text>(currentText->copyPaste());
	tmp->setRotationangle(0);
	currentLine->move(tmp->centralPoint().x()*zoomRatio,
		tmp->centralPoint().y()*zoomRatio);
	//qDebug()<<currentLine->pos();
	currentLine->setFixedHeight((currentText->maxy - currentText->miny)*qAbs(currentText->getsx()));
	currentLine->setFixedWidth((currentText->maxx - currentText->minx)*qAbs(currentText->getsx()));
	currentLine->setReverse(currentText->getsx() < 0);
	currentLine->move(currentLine->x() - currentLine->width() / 2.0, currentLine->y() - currentLine->height() / 2.0);
	currentLine->setFont(currentText->myfont);
	currentLine->setStyleSheet("color: " + currentText->brush.color().name() + ";");
	//currentLine->show();
	//qDebug()<<currentLine->pos();
	currentRotateLine->setRotation(currentText->rotationangle());
	currentRotateLine->draw(painter, zoomRatio);
	//currentLine->grabKeyboard();
}
