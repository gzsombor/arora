#ifndef ARORAWINDOW_H
#define ARORAWINDOW_H

#include "webview.h"

#include "qobject.h"
#include "qmenu.h"
#include "qtoolbar.h"

class AroraWindow : public QObject
{
    Q_OBJECT

public:
    virtual void showCookiesDialog() = 0;

    virtual void showCookieExceptionsDialog() = 0;

    virtual QMenu *toolsMenu() = 0;

    virtual QToolBar *navigationToolbar() = 0;

    virtual TabWidget *tabWidget() = 0;

    virtual WebView *currentTab() = 0;

    enum ActionTypes {
        Back, Forward, ReloadStop
    };

    virtual QAction *action(ActionTypes actionType) = 0;

    virtual QObject *get(QString name) = 0;

    virtual QObject *remove(QString name) = 0;

    virtual void set(QString name,QObject *object) = 0;
};

#endif // ARORAWINDOW_H
