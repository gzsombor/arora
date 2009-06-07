#include "pluginapi.h"

#include "browserapplication.h"
#include "cookiedialog.h"
#include "cookieexceptionsdialog.h"

PluginApi::PluginApi()
{
}


void PluginApi::showCookiesDialog(BrowserMainWindow *window)
{
    CookieDialog *dialog = new CookieDialog(BrowserApplication::cookieJar(), window);
    dialog->exec();
}

void PluginApi::showCookieExceptionsDialog(BrowserMainWindow *window)
{
    CookieExceptionsDialog *dialog = new CookieExceptionsDialog(BrowserApplication::cookieJar(), window);
    dialog->exec();
}

QNetworkAccessManager *PluginApi::networkAccessManager()
{
    return BrowserApplication::networkAccessManager();
}
