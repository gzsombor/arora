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
    this->m_manager = manager;
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
    foreach (QString pluginId, this->m_manager->ids()) {
        ExtensionInfo *info = this->m_manager->plugin(pluginId);

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
