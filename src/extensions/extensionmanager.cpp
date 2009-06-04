#include "extensionmanager.h"
#include "qpluginloader.h"
#include "qdir.h"
#include "qapplication.h"
#include "qdebug.h"

ExtensionManager::ExtensionManager()
{
    this->api = new PluginApi();
    this->loadPlugins();
}

ExtensionManager::~ExtensionManager()
{
    foreach (AroraExtension *plugin, this->enabledExtensions) {
        plugin->deactivate();
    }
}

void ExtensionManager::loadPlugins()
{
    foreach (QObject *plugin, QPluginLoader::staticInstances()) {
        initPlugin(plugin);
    }
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    #if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
    #elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
         pluginsDir.cdUp();
         pluginsDir.cdUp();
         pluginsDir.cdUp();
    }
    #endif
    pluginsDir.cd(QLatin1String("plugins"));
    qDebug() << "checking " << pluginsDir;
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        qDebug() << "loading " << fileName;
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            initPlugin(plugin);
        }
    }
}

void ExtensionManager::initPlugin(QObject *pluginObject)
{
    AroraExtension *plugin =  qobject_cast<AroraExtension *>(pluginObject);
    qDebug() << "plugin acquired " << (plugin!=NULL);
    if (plugin) {
        QString id = plugin->id();
        bool initialState = isPluginEnabledBySetting(id);
        ExtensionInfo *info = new ExtensionInfo(this, pluginObject, initialState);
        this->idToExtension.insert(id,info);
        qDebug() << "init plugin " << id;
        extensions.append(info);
        if (initialState) {
            activatePlugin(info);
        }
    } else {
        qDebug() << "-> not an Arora extension";
    }
}

bool ExtensionManager::isPluginEnabledBySetting(const QString &id) const
{
    // TODO : decide is this plugin is enabled, pretend yes
    return true;
}

bool ExtensionManager::activatePlugin(ExtensionInfo *info)
{
    AroraExtension *extension = info->extension();
    if (extension->activate(this->api)) {
        enabledExtensions.append(extension);
        WindowExtension *windowPlugin = info->windowExtension();
        if (windowPlugin) {
            enabledWindowExtensions.append(windowPlugin);
        }
        return true;
    }
    return false;
}

/**
  * return true if deactivation succeeded.
  */
bool ExtensionManager::deactivatePlugin(ExtensionInfo *info)
{
    WindowExtension *windowPlugin = info->windowExtension();
    if (windowPlugin) {
        enabledWindowExtensions.removeOne(windowPlugin);
    }
    AroraExtension *extension = info->extension();
    if (enabledExtensions.removeOne(extension)) {
        extension->deactivate();
        return true;
    }
    return false;
}

QList<QString> ExtensionManager::ids()
{
    return this->idToExtension.keys();
}

QList<ExtensionInfo *> ExtensionManager::enabledExtensionInfo()
{
    QList<ExtensionInfo *> result;
    foreach(ExtensionInfo *info, this->extensions) {
        if (info->isEnabled()) {
            result.append(info);
        }
    }
    return result;
}

ExtensionInfo* ExtensionManager::plugin(const QString &id)
{
    return idToExtension[id];
}

void ExtensionManager::newWindow(BrowserMainWindow *window, QMenu *extensionMenu)
{
    foreach(WindowExtension *plugin, this->enabledWindowExtensions) {
        plugin->newWindow(window, extensionMenu);
    }
}

void ExtensionManager::closeWindow(BrowserMainWindow *window)
{
    foreach(WindowExtension *plugin, this->enabledWindowExtensions) {
        plugin->closeWindow(window);
    }
}

void ExtensionManager::localize(BrowserMainWindow *window)
{
    foreach(WindowExtension *plugin, this->enabledWindowExtensions) {
        plugin->localize(window);
    }
}
