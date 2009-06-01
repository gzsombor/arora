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
