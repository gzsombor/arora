#ifndef ARORAWINDOWIMPL_H
#define ARORAWINDOWIMPL_H

#include "arorawindow.h"
#include "browsermainwindow.h"

class AroraWindowImpl : public AroraWindow
{
    Q_OBJECT

public:
    AroraWindowImpl(BrowserMainWindow *window);

    void showCookiesDialog();
    void showCookieExceptionsDialog();
    QMenu *toolsMenu();
    QToolBar *navigationToolbar();
    WebView *currentTab();
    TabWidget *tabWidget();


    QAction *action(ActionTypes actionType);

    QObject *get(QString name);

    QObject *remove(QString name);

    void set(QString name,QObject *object);

private:
    BrowserMainWindow *m_window;
    QHash<QString,QObject *> m_properties;
};

#endif // ARORAWINDOWIMPL_H
