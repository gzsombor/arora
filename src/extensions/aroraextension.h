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

#ifndef ARORAEXTENSION_H
#define ARORAEXTENSION_H

#include "browsermainwindow.h"
#include "pluginapi.h"

class AroraExtension
{

public:
    virtual ~AroraExtension() {}

    /**
      * return a unique id for this plugin, this is used by the manager to
      * identifie the extension
      */
    virtual const QString id() const = 0;

    /**
      * Human readable/translated string, this will show up on the plugin
      * manager dialog.
      */
    virtual const QString name() const = 0;

    /**
      * this method called before the extension is activated, should return true
      * if succeeded.
      */
    virtual bool activate(PluginApi *api) = 0;

    /**
      * this method called before the extension is deactivated
      */
    virtual void deactivate() = 0;

    virtual void configure() = 0;
};

Q_DECLARE_INTERFACE(AroraExtension, "org.arora-browser.Extension/1.0")

#endif // ARORAEXTENSION_H
