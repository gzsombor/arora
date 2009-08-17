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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef RULEEDITOR_H
#define RULEEDITOR_H

#include <qdialog.h>
#include "ui_ruleeditor.h"

#include "ruletablemodel.h"

class RuleEditor: public QDialog, public Ui_RuleEditor
{
    Q_OBJECT

public:
    RuleEditor(NetworkAccessPolicy *datas, QWidget *parent = 0);

protected:
    void save();

protected slots:
    void addRule();
    void modifyRule();
    void deleteRule();
    void accept();
    void selectionChanged(const QModelIndex &current, const QModelIndex &prev);
    void importRules();
    void viewSubscriptions();
    void setAdblockEnabled(bool flag);

private:
    RuleTableModel *m_tableModel;
    NetworkAccessPolicy *m_accessPolicy;
    AdBlockSubscription *m_currentSubscription;

    UrlAccessRule *createRule(bool newRule);
    void enableButtons(bool flag);
};

#endif // RULEEDITOR_H
