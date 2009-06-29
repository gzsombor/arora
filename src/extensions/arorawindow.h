#ifndef ARORAWINDOW_H
#define ARORAWINDOW_H

#include "qobject.h"
#include "qmenu.h"


class AroraWindow : public QObject
{
    Q_OBJECT

public:
    virtual void showCookiesDialog() = 0;
    virtual void showCookieExceptionsDialog() = 0;
    virtual QMenu *toolsMenu() = 0;

};

#endif // ARORAWINDOW_H
