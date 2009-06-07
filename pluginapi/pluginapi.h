#ifndef PLUGINAPI_H
#define PLUGINAPI_H

#include "browsermainwindow.h"

#include <qobject.h>
#include <qmenu.h>
#include <qplugin.h>
#include <qnetworkaccessmanager.h>


#if defined(PLUGINAPI_LIBRARY)
 #  define PLUGINAPI_EXPORT Q_DECL_EXPORT
 #else
 #  define PLUGINAPI_EXPORT Q_DECL_IMPORT
 #endif

class PLUGINAPI_EXPORT PluginApi : public QObject
{
    Q_OBJECT

public:
    PLUGINAPI_EXPORT PluginApi();

    PLUGINAPI_EXPORT void showCookiesDialog(BrowserMainWindow *window);
    PLUGINAPI_EXPORT void showCookieExceptionsDialog(BrowserMainWindow *window);

    PLUGINAPI_EXPORT QNetworkAccessManager *networkAccessManager();

};

#endif // PLUGINAPI_H
