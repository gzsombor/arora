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
    ExtensionInfo(ExtensionManager *manager, QObject *extensionObject);

    AroraExtension *extension() const;
    WindowExtension *windowExtension() const;
    AroraWebPlugin *webPlugin() const;

    inline const QString id() const;

    bool isEnabled() const { return enabled; }
public slots:
    void setEnabled(bool flag);
    void callConfigure();


private:
    ExtensionManager *manager;
    QObject *extensionObject;
    bool enabled;

};

#endif // EXTENSIONINFO_H
