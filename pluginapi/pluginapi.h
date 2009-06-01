#ifndef PLUGINAPI_H
#define PLUGINAPI_H

// Include this file from the plugins, this contains the necessary includes.

#include "qplugin.h"
#include "qobject.h"

#include "qmenu.h"

#include "browsermainwindow.h"

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

};

#endif // PLUGINAPI_H
