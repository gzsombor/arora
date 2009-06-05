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

#include "extensioninfo.h"
#include "extensionmanager.h"
#include "qdebug.h"

ExtensionInfo::ExtensionInfo(ExtensionManager *manager, QObject *extensionObject, bool initialState)
{
    this->m_manager = manager;
    this->m_extensionObject = extensionObject;
    this->m_enabled = initialState;
}

void ExtensionInfo::setEnabled(bool newState)
{
    if (this->m_enabled==newState) {
        return;
    }
    // yes, it can be written in one line only ...
    bool success = false;
    if (newState) {
        success = this->m_manager->activatePlugin(this);
    } else {
        success = this->m_manager->deactivatePlugin(this);
    }
    if (success) {
        this->m_enabled = newState;
    }
}

void ExtensionInfo::callConfigure()
{
    this->extension()->configure();
}


AroraExtension *ExtensionInfo::extension() const
{
    return qobject_cast<AroraExtension*> (this->m_extensionObject);
}

WindowExtension *ExtensionInfo::windowExtension() const
{
    return qobject_cast<WindowExtension*> (this->m_extensionObject);
}

AroraWebPlugin *ExtensionInfo::webPlugin() const
{
    return qobject_cast<AroraWebPlugin*> (this->m_extensionObject);
}

const QString ExtensionInfo::id() const
{
    return this->extension()->id();
}
