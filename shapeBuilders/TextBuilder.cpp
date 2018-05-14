#include "TextBuilder.h"
#include "Text.h"
#include "GetTextDialog.h"

TextBuilder::TextBuilder()
{
}


TextBuilder::~TextBuilder()
{
}

void TextBuilder::mouseDown(QPointF p, bool bLeft)
{
}

void TextBuilder::mouseMove(QPointF p, bool bLeft)
{

}

void TextBuilder::mouseUp(QPointF p, bool bLeft)
{
	m_Shape = static_pointer_cast<GeneralShape>(shared_ptr<Text>(new Text));//new Text;

	m_Shape->appendPoint(p);
	this->publishShapeCreated();

	GetTextDialog* gtd = new GetTextDialog;
	gtd->setText("hello world!");

	if (gtd->exec() == QDialog::Accepted) {
		shared_ptr<Text> pm = dynamic_pointer_cast<Text>(m_Shape);
		if (pm) {
			pm->setText(gtd->text);
		}
	}
	delete gtd;

	this->publishShapeFinished();
	m_Shape = nullptr;
}
