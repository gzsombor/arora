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

