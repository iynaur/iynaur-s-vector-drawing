#ifndef DRAWACTION_H
#define DRAWACTION_H
#include "actions_global.h"
#include "generalshape.h"

#include <QList>
#include <memory>
#include <QUndoCommand>
#include <QString>
#include <string>
#include <QTranslator>

class ACTIONS_EXPORT AbstractAction : public QUndoCommand//, public DrawAreaWidget
{
public:
	AbstractAction();
	virtual ~AbstractAction();
	QList<shared_ptr<GeneralShape> > shapes;
	virtual QString name() = 0;
	QList<shared_ptr<GeneralShape> >* allShapes;
	QList<shared_ptr<GeneralShape> > pickedShapes;
	bool m_bHasDone;
	virtual void reallyDo()=0;
	void redo();
	virtual void setPickedShapes();
};

#endif // DRAWACTION_H
