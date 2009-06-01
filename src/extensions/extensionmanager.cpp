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
        plugin->close();
    }
    //delete enabledExtensions;
    //delete extensions;
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
        this->idToExtension[id] = plugin;
        qDebug() << "init plugin " << id;
        extensions.append(plugin);
        if (isPluginEnabledBySetting(plugin->id())) {
            if (plugin->init(this->api)) {
                enabledExtensions.append(plugin);
            }
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

bool ExtensionManager::setEnabled(const QString &id, bool enabled)
{
    bool oldStatus = isEnabled(id);
    if (oldStatus == enabled) {
        return oldStatus;
    }
    AroraExtension *plugin = idToExtension[id];
    if (plugin) {
        if (enabled) {
            if (plugin->init(api)) {
                enabledExtensions.append(plugin);
                return enabled;
            }
            return false;
        } else {
            if (enabledExtensions.removeOne(plugin)) {
                plugin->close();
                return false;
            }
            return true;
        }
    }
    return false;
}

QList<QString> ExtensionManager::ids()
{
    return this->idToExtension.keys();
}

bool ExtensionManager::isEnabled(const QString &id)
{
    return enabledExtensions.contains(idToExtension[id]);
}

AroraExtension* ExtensionManager::plugin(const QString &id)
{
    return idToExtension[id];
}


void ExtensionManager::newWindow(BrowserMainWindow *window, QMenu *extensionMenu)
{
    foreach(AroraExtension *plugin, this->enabledExtensions) {
        plugin->newWindow(window, extensionMenu);
    }
}

void ExtensionManager::closeWindow(BrowserMainWindow *window)
{
    foreach(AroraExtension *plugin, this->enabledExtensions) {
        plugin->closeWindow(window);
    }
}

void ExtensionManager::localize(BrowserMainWindow *window)
{
    foreach(AroraExtension *plugin, this->enabledExtensions) {
        plugin->localize(window);
    }
}
