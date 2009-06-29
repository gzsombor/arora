#include "pluginapi.h"

#include "browserapplication.h"
#include "cookiedialog.h"
#include "cookieexceptionsdialog.h"
#include "networkaccessmanager.h"

PluginApi::PluginApi()
{
}

QNetworkAccessManager *PluginApi::networkAccessManager()
{
    return BrowserApplication::networkAccessManager();
}


QList<BrowserMainWindow *> allBrowserWindows()
{
    return BrowserApplication::instance()->mainWindows();
}
