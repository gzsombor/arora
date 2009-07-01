#include "arorawindowimpl.h"
#include "browserapplication.h"
#include "cookiedialog.h"
#include "cookieexceptionsdialog.h"

AroraWindowImpl::AroraWindowImpl(BrowserMainWindow *window) : m_window(window)
{
}

void AroraWindowImpl::showCookiesDialog()
{
    CookieDialog *dialog = new CookieDialog(BrowserApplication::cookieJar(), m_window);
    dialog->exec();
}

void AroraWindowImpl::showCookieExceptionsDialog()
{
    CookieExceptionsDialog *dialog = new CookieExceptionsDialog(BrowserApplication::cookieJar(), m_window);
    dialog->exec();
}

QMenu *AroraWindowImpl::toolsMenu()
{
    return m_window->toolsMenu();
}

QToolBar *AroraWindowImpl::navigationToolbar()
{
    return m_window->navigationToolbar();
}

WebView *AroraWindowImpl::currentTab()
{
    return m_window->currentTab();
}

TabWidget *AroraWindowImpl::tabWidget()
{
    return m_window->tabWidget();
}


QAction *AroraWindowImpl::action(ActionTypes actionType)
{
    switch(actionType) {
        case AroraWindow::ReloadStop :
            return m_window->actionReload();
        case AroraWindow::Back :
            return m_window->actionBack();
        case AroraWindow::Forward :
            return m_window->actionForward();
    }
    return 0;
}

QObject *AroraWindowImpl::get(QString &name)
{
    return m_properties[name];
}

QObject *AroraWindowImpl::remove(QString &name)
{
    QObject *obj = m_properties.value(name);
    m_properties.remove(name);
    return obj;
}

void AroraWindowImpl::set(QString &name,QObject *object)
{
    m_properties.insert(name, object);
}

