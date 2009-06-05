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

#ifndef EXTENSIONINFO_H
#define EXTENSIONINFO_H

#include "aroraextension.h"
#include "windowextension.h"
#include "arorawebplugin.h"

#include "qobject.h"

class ExtensionManager;

class ExtensionInfo : public QObject
{
    Q_OBJECT
public:
    ExtensionInfo(ExtensionManager *manager, QObject *extensionObject, bool initialState);

    AroraExtension *extension() const;
    WindowExtension *windowExtension() const;
    AroraWebPlugin *webPlugin() const;

    inline const QString id() const;

    bool isEnabled() const { return m_enabled; }

public slots:
    void setEnabled(bool flag);
    void callConfigure();

private:
    ExtensionManager *m_manager;
    QObject *m_extensionObject;
    bool m_enabled;
};

#endif // EXTENSIONINFO_H
