// QmitkFctMediator.h: interface for the QmitkFctMediator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUSFUNCTIONALITYMANAGER_H__1DC0BA6E_9B8D_4D63_8A63_5B661CE33712__INCLUDED_)
#define AFX_QUSFUNCTIONALITYMANAGER_H__1DC0BA6E_9B8D_4D63_8A63_5B661CE33712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <QmitkBaseFctMediator.h>
#include <QmitkFunctionality.h>

#include <qptrlist.h> 

class QWidgetStack;
class QButtonGroup;
class QToolBar;
class QActionGroup;

class QmitkFctMediator : public QObject
{
	Q_OBJECT
protected:
	QWidgetStack* m_MainStack;
    QWidgetStack* m_ControlStack;
	QButtonGroup* m_ButtonMenu;
	QWidget*      m_ToolBar;

    QWidget* m_DefaultMain;

    int m_NumOfFuncs;
	int m_CurrentFunctionality;

    QActionGroup* m_FunctionalityActionGroup;

	QPtrList<QmitkFunctionality> qfl;
	QPtrList<QAction> qal;

protected slots:
	void slot_dummy_protected() {};
	virtual void selecting(int id);
	virtual void functionalitySelected(int id);
    virtual void raiseFunctionality(QAction* action);
public slots:
	void slot_dummy() {};

    virtual void hideControls(bool hide);
	virtual void hideMenu(bool hide);

    virtual void checkAvailability();

    virtual void raiseFunctionality(int id);
	virtual void raiseFunctionality(QmitkFunctionality* aFunctionality);
public:
    virtual void initialize(QWidget *aLayoutTemplate);

	virtual bool addFunctionality(QmitkFunctionality * functionality);

	virtual QmitkFunctionality* getFunctionalityByName(const char * name);
	virtual QWidget * getMainParent();
	virtual QWidget * getControlParent();
	virtual QButtonGroup * getButtonMenu();
	virtual QWidget * getToolBar();
    virtual QWidget * getDefaultMain();

    QmitkFctMediator(QObject *parent=0, const char *name=0);
	virtual ~QmitkFctMediator();

};

#endif // !defined(AFX_QUSFUNCTIONALITYMANAGER_H__1DC0BA6E_9B8D_4D63_8A63_5B661CE33712__INCLUDED_)
