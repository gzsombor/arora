#ifndef EXTENSIONINFO_H
#define EXTENSIONINFO_H

#include "aroraextension.h"
#include "extensionmanager.h"

#include "qobject.h"

class ExtensionInfo : public QObject
{
    Q_OBJECT
public:
    ExtensionInfo(ExtensionManager *manager, AroraExtension *extension);

public slots:
    void checked(bool flag);
    void configure();

private:
    ExtensionManager *manager;
    AroraExtension *extension;

};

#endif // EXTENSIONINFO_H
