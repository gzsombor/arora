#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include "qobject.h"
#include "qlist.h"
#include "qhash.h"
#include "extensioninfo.h"
#include "aroraextension.h"
#include "windowextension.h"

class ExtensionManager : public QObject
{
    Q_OBJECT

public:
    ExtensionManager();
    ~ExtensionManager();

    void newWindow(BrowserMainWindow *window, QMenu *extensionMenu);

    void closeWindow(BrowserMainWindow *window);

    void localize(BrowserMainWindow *window);

    QList<QString> ids();

    ExtensionInfo* plugin(const QString &id);


    bool activatePlugin(ExtensionInfo *plugin);
    bool deactivatePlugin(ExtensionInfo *plugin);

private:
    void loadPlugins();
    void initPlugin(QObject *plugin);
    bool isPluginEnabledBySetting(const QString &id) const;


    PluginApi *api;
    QList<ExtensionInfo *> extensions;
    QHash<QString,ExtensionInfo *> idToExtension;
    QList<AroraExtension *> enabledExtensions;
    QList<WindowExtension *> enabledWindowExtensions;

};

#endif // EXTENSIONMANAGER_H
