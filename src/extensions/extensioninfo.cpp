#include "extensioninfo.h"

#include "qdebug.h"

ExtensionInfo::ExtensionInfo(ExtensionManager *manager, AroraExtension *extension)
{
    this->manager = manager;
    this->extension = extension;
}

void ExtensionInfo::checked(bool flag)
{
    qDebug() << "checked " << flag;
    bool result = this->manager->setEnabled(this->extension->id(), flag);
    qDebug() << "result " << result;

}

void ExtensionInfo::configure()
{
    this->extension->configure();
}
