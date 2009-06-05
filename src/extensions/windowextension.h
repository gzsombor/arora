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

#ifndef WINDOWEXTENSION_H
#define WINDOWEXTENSION_H

#include "aroraextension.h"
#include "pluginapi.h"

class WindowExtension
{

public:

    /**
      * this method called after a new application window is created.
      */
    virtual void newWindow(BrowserMainWindow *window, QMenu *extensionMenu) = 0;

    /**
      * this method called before a new application window is closed.
      */
    virtual void closeWindow(BrowserMainWindow *window) = 0;

    /**
      * this method called when language is changed, or during the initialization phase
      * and strings should be (re)translated.
      */
    virtual void localize(BrowserMainWindow *window) = 0;

};

Q_DECLARE_INTERFACE(WindowExtension, "org.arora-browser.WindowExtension/1.0")


#endif // WINDOWEXTENSION_H
