#include "extensiondialog.h"
#include "ui_extensiondialog.h"

#include "extensioninfo.h"

#include "qcheckbox.h"
#include "qpushbutton.h"
#include "qformlayout.h"


ExtensionDialog::ExtensionDialog(ExtensionManager *manager, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ExtensionDialog)
{
    this->manager = manager;
    m_ui->setupUi(this);

    setupExtensionList();
}

ExtensionDialog::~ExtensionDialog()
{
    delete m_ui;
}

void ExtensionDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ExtensionDialog::setupExtensionList()
{
    int position = 0;
    foreach (QString pluginId, this->manager->ids()) {
        ExtensionInfo *info = this->manager->plugin(pluginId);

        QString name = info->extension()->name();

        QCheckBox *checkBox = new QCheckBox(m_ui->extGroupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox-") + pluginId);
        checkBox->setText(name);
        checkBox->setToolTip(name);

        checkBox->setChecked(info->isEnabled());

        m_ui->formLayout->setWidget(position, QFormLayout::LabelRole, checkBox);

        QPushButton *pushButton = new QPushButton(m_ui->extGroupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton-") + pluginId);
        pushButton->setText(tr("Configure"));

        m_ui->formLayout->setWidget(position, QFormLayout::FieldRole, pushButton);

        connect(checkBox, SIGNAL(toggled(bool)), info, SLOT(setEnabled(bool)));
        connect(pushButton, SIGNAL(clicked()), info, SLOT(callConfigure()));
        position++;
    }
}
