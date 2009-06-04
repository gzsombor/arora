#include "extensioninfo.h"
#include "extensionmanager.h"
#include "qdebug.h"

ExtensionInfo::ExtensionInfo(ExtensionManager *manager, QObject *extensionObject, bool initialState)
{
    this->manager = manager;
    this->extensionObject = extensionObject;
    this->enabled = initialState;
}

void ExtensionInfo::setEnabled(bool newState)
{
    if (this->enabled==newState) {
        return;
    }
    bool success = false;
    if (newState) {
        success = this->manager->activatePlugin(this);
    } else {
        success = this->manager->deactivatePlugin(this);
    }
    if (success) {
        this->enabled = newState;
    }
}

void ExtensionInfo::callConfigure()
{
    this->extension()->configure();
}


AroraExtension *ExtensionInfo::extension() const
{
    return qobject_cast<AroraExtension *> (extensionObject);
}

WindowExtension *ExtensionInfo::windowExtension() const
{
    return qobject_cast<WindowExtension *> (extensionObject);
}

AroraWebPlugin *ExtensionInfo::webPlugin() const
{
    return qobject_cast<AroraWebPlugin *> (extensionObject);
}

const QString ExtensionInfo::id() const
{
    return this->extension()->id();
}
