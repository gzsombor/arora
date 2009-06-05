/*
* Copyright 2009 Zsombor Gegesy <gzsombor@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301 USA
*/

#include "extensionmanager.h"
#include "qpluginloader.h"
#include "qdir.h"
#include "qapplication.h"
#include "qdebug.h"

ExtensionManager::ExtensionManager()
{
    this->m_api = new PluginApi();
    this->loadPlugins();
}

ExtensionManager::~ExtensionManager()
{
    foreach (AroraExtension *plugin, this->m_enabledExtensions) {
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
    AroraExtension *plugin =  qobject_cast<AroraExtension*> (pluginObject);
    qDebug() << "plugin acquired " << (plugin!=NULL);
    if (plugin) {
        QString id = plugin->id();
        if (!this->m_idToExtension.contains(id)) {
            bool initialState = isPluginEnabledBySetting(id);
            ExtensionInfo *info = new ExtensionInfo(this, pluginObject, initialState);
            this->m_idToExtension.insert(id,info);
            qDebug() << "init plugin " << id;
            this->m_extensions.append(info);
            if (initialState) {
                activatePlugin(info);
            }
        } else {
            qDebug() << "plugin with the same id already exists : " << id;
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
    if (extension->activate(this->m_api)) {
        this->m_enabledExtensions.append(extension);
        WindowExtension *windowPlugin = info->windowExtension();
        if (windowPlugin) {
            this->m_enabledWindowExtensions.append(windowPlugin);
        }
        emit pluginStateChanged(info);
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
        this->m_enabledWindowExtensions.removeOne(windowPlugin);
    }
    AroraExtension *extension = info->extension();
    if (this->m_enabledExtensions.removeOne(extension)) {
        extension->deactivate();
        emit pluginStateChanged(info);
        return true;
    }
    return false;
}

QList<QString> ExtensionManager::ids()
{
    return this->m_idToExtension.keys();
}

QList<ExtensionInfo*> ExtensionManager::enabledExtensionInfo()
{
    QList<ExtensionInfo*> result;
    foreach (ExtensionInfo *info, this->m_extensions) {
        if (info->isEnabled()) {
            result.append(info);
        }
    }
    return result;
}

ExtensionInfo *ExtensionManager::plugin(const QString &id)
{
    return m_idToExtension[id];
}

void ExtensionManager::newWindow(BrowserMainWindow *window, QMenu *extensionMenu)
{
    foreach (WindowExtension *plugin, this->m_enabledWindowExtensions) {
        plugin->newWindow(window, extensionMenu);
    }
}

void ExtensionManager::closeWindow(BrowserMainWindow *window)
{
    foreach (WindowExtension *plugin, this->m_enabledWindowExtensions) {
        plugin->closeWindow(window);
    }
}

void ExtensionManager::localize(BrowserMainWindow *window)
{
    foreach (WindowExtension *plugin, this->m_enabledWindowExtensions) {
        plugin->localize(window);
    }
}
