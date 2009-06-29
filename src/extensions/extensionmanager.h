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

#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include "aroraextension.h"
#include "arorawindowimpl.h"
#include "extensioninfo.h"
#include "networkextension.h"
#include "windowextension.h"

#include "qobject.h"
#include "qlist.h"
#include "qhash.h"


class ExtensionManager : public QObject
{
    Q_OBJECT

public:
    ExtensionManager();
    ~ExtensionManager();

    void newWindow(BrowserMainWindow *window);

    void closeWindow(BrowserMainWindow *window);

    void localize(BrowserMainWindow *window);

    QNetworkReply *handleRequest(QNetworkAccessManager::Operation op,
                                         const QNetworkRequest &request,
                                         QIODevice *outgoingData = 0);

    QList<QString> ids();

    ExtensionInfo *plugin(const QString &id);

    QList<ExtensionInfo*> enabledExtensionInfo();

    bool activatePlugin(ExtensionInfo *plugin);
    bool deactivatePlugin(ExtensionInfo *plugin);

signals:
    void pluginStateChanged(ExtensionInfo *info);

private:
    void loadPlugins();
    void initPlugin(QObject *plugin);
    bool isPluginEnabledBySetting(const QString &id) const;
    AroraWindowImpl *getOrCreate(BrowserMainWindow *window);



    PluginApi *m_api;
    QList<ExtensionInfo *> m_extensions;
    QHash<QString,ExtensionInfo *> m_idToExtension;
    QList<AroraExtension *> m_enabledExtensions;
    QList<WindowExtension *> m_enabledWindowExtensions;
    QList<NetworkExtension *> m_enabledNetworkExtensions;
    QHash<BrowserMainWindow*,AroraWindowImpl *> m_windowWrappers;

};

#endif // EXTENSIONMANAGER_H
