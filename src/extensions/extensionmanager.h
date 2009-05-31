#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include "qobject.h"
#include "qlist.h"
#include "qhash.h"
#include "aroraextension.h"

class ExtensionManager : public QObject
{
    Q_OBJECT

public:
    ExtensionManager();
    ~ExtensionManager();

    void newWindow(BrowserMainWindow *window, QMenu *extensionMenu);

    void closeWindow(BrowserMainWindow *window);

    void localize(BrowserMainWindow *window);

    void setEnabled(const QString &id, bool enabled);

    QList<QString> ids();
    bool isEnabled(const QString &id);

private:
    void loadPlugins();
    void initPlugin(QObject *plugin);
    bool isPluginEnabledBySetting(const QString &id) const;

    QList<AroraExtension*> extensions;
    QHash<QString,AroraExtension*> idToExtension;
    QList<AroraExtension*> enabledExtensions;
};

#endif // EXTENSIONMANAGER_H
