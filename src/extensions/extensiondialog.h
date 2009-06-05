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
    ExtensionManager *m_manager;

    void setupExtensionList();
};


#endif // EXTENSIONDIALOG_H
