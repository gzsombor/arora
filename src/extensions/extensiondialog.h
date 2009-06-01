#ifndef EXTENSIONDIALOG_H
#define EXTENSIONDIALOG_H

#include "qobject.h"
#include "qdialog.h"

#include "aroraextension.h"
#include "extensionmanager.h"

namespace Ui {
    class ExtensionDialog;
}

class ExtensionDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ExtensionDialog)
public:
    explicit ExtensionDialog(ExtensionManager *manager, QWidget *parent = 0);
    virtual ~ExtensionDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ExtensionDialog *m_ui;
    ExtensionManager *manager;

    void setupExtensionList();
};


#endif // EXTENSIONDIALOG_H
