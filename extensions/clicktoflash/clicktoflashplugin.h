/**
 * Copyright (c) 2009, Benjamin C. Meyer
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Benjamin Meyer nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef CLICKTOFLASHPLUGIN_H
#define CLICKTOFLASHPLUGIN_H

#include "qobject.h"
#include "qwebpluginfactory.h"

#include "aroraapi.h"


class ClickToFlashPlugin :  public QObject, AroraExtension, AroraWebPlugin
{
    Q_OBJECT
    Q_INTERFACES(AroraExtension)
    Q_INTERFACES(AroraWebPlugin)


public:
    ClickToFlashPlugin();
    virtual ~ClickToFlashPlugin();

    // AroraWebPlugin methods
    virtual QWebPluginFactory::Plugin metaPlugin();
    virtual QWidget *create(const QString &mimeType, const QUrl &url,
                    const QStringList &argumentNames, const QStringList &argumentValues);

    // AroraExtension methods
    /**
      * return a unique id for this plugin, this is used by the manager to
      * identifie the extension
      */
    virtual const QString id() const;

    /**
      * Human readable/translated string, this will show up on the plugin
      * manager dialog.
      */
    virtual const QString name() const;

    /**
      * this method called before the extension is activated, should return true
      * if succeeded.
      */
    virtual bool activate(PluginApi *api);

    /**
      * this method called before the extension is deactivated
      */
    virtual void deactivate();

    virtual void configure();

    bool onWhitelist(const QString &host) const;
    void addToWhitelist(const QString &host);
    void removeFromWhitelist(const QString &host);

private:
    void load();
    void save();

    bool m_loaded;
    bool m_enabled;
    QStringList m_whitelist;
};

#endif // CLICKTOFLASHPLUGIN_H

