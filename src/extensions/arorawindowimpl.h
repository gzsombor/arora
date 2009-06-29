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

private:
    BrowserMainWindow *m_window;

};

#endif // ARORAWINDOWIMPL_H
