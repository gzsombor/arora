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

#include "ruleeditor.h"

#include "ruletablemodel.h"
#include "subscriptioneditor.h"

#include <qfiledialog.h>

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

RuleEditor::RuleEditor(NetworkAccessPolicy *datas, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    m_accessPolicy = datas;
    m_tableModel = new RuleTableModel(datas, this);
    int height = ruleListView->fontMetrics().height();
    ruleListView->verticalHeader()->setDefaultSectionSize(height);
    ruleListView->setModel(m_tableModel);
    ruleListView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ruleListView->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ruleListView->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    ruleListView->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
    ruleListView->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);

    adblockerGroupBox->setChecked(m_accessPolicy->isEnabled());

    connect(ruleListView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(selectionChanged(const QModelIndex &, const QModelIndex &)));

    QPushButton *importButton = new QPushButton(tr("&Import"));
    connect(importButton,SIGNAL(clicked()), this, SLOT(importRules()));
    buttonBox->addButton(importButton, QDialogButtonBox::ActionRole);

    QPushButton *subscriptionButton = new QPushButton(tr("&Subscriptions"));
    connect(subscriptionButton, SIGNAL(clicked()), this, SLOT(viewSubscriptions()));
    buttonBox->addButton(subscriptionButton, QDialogButtonBox::ActionRole);

    enableButtons(m_accessPolicy->isEnabled());

    connect(adblockerGroupBox, SIGNAL(clicked(bool)), this, SLOT(setAdblockEnabled(bool)));
}

void RuleEditor::addRule()
{
    UrlAccessRule *newRule = createRule(true);
    m_tableModel->append(newRule);
    int index = m_tableModel->currentRules().indexOf(newRule);
    if (index >= 0)
        ruleListView->selectRow(index);
}

UrlAccessRule *RuleEditor::createRule(bool newRule)
{
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "set rule " << ruleEdit->text() << " enabled " << enabledCheckBox->isChecked();
#endif
    return new UrlAccessRule(regExpCheckBox->isChecked(), ruleEdit->text(),
            exclusionCheckBox->isChecked(), 0, enabledCheckBox->isChecked(), newRule ? 0 : m_currentSubscription);
}

void RuleEditor::modifyRule()
{
    const QModelIndexList indexes = ruleListView->selectionModel()->selectedRows();
    if (indexes.size() > 0) {
        QModelIndex index = indexes.at(0);
        m_tableModel->setData(index.row(), createRule(false));
    }
}

void RuleEditor::deleteRule()
{
    const QModelIndexList indexes = ruleListView->selectionModel()->selectedRows();
    QModelIndex index;
    int row = -1;
    for (int i = indexes.size() - 1; i >= 0; --i) {
        index = indexes.at(i);
        row = index.row();
        m_tableModel->removeRow(row);
    }
    if (row >= 0) {
        if (row > m_tableModel->currentRules().size())
            row = m_tableModel->currentRules().size() - 1;
        ruleListView->selectRow(row);
    }
}

void RuleEditor::selectionChanged(const QModelIndex &current, const QModelIndex &prev)
{
    Q_UNUSED(prev);
    if (current.row() < 0 || m_tableModel->rowCount() <= current.row()) {
        return;
    }
    UrlAccessRule *rule = m_tableModel->currentRules().at(current.row());
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "rule " << rule->pattern() << " enabled " << rule->isEnabled() << " editable " << rule->isEditable();

#endif
    m_currentSubscription = rule->subscription();
    ruleEdit->setText(rule->pattern());
    exclusionCheckBox->setChecked(rule->isExceptionRule());
    regExpCheckBox->setChecked(rule->isRegexpRule());
    enabledCheckBox->setChecked(rule->isEnabled());
    bool editable = rule->isEditable();
    modifyButton->setEnabled(editable);
}

void RuleEditor::accept()
{
    save();
    QDialog::accept();
}

void RuleEditor::save()
{
    if (adblockerGroupBox->isChecked()) {
        QList<UrlAccessRule*> *cr = new QList<UrlAccessRule*>(m_tableModel->currentRules());
        m_accessPolicy->setAccessRules(*cr);
    }
}

void RuleEditor::importRules()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import AdBlock Plus File"),
                                 QDir::homePath(), tr("AdBlock Ruleset (*.txt)"));
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "file name selected " << fileName;
#endif
    if (fileName.length() > 0) {
        QList<UrlAccessRule*> *newRules = new QList<UrlAccessRule*>();
        QFile importFile (fileName);
        if (NetworkAccessPolicy::importAdBlockRulesFromFile(importFile, *newRules)) {
            m_tableModel->setCurrentRules(*newRules);
        }
    }
}

void RuleEditor::viewSubscriptions()
{
    save();
    SubscriptionEditor *editor = new SubscriptionEditor(m_accessPolicy, parentWidget());
    editor->show();
    QDialog::accept();
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "SubscriptionEditor closed";
#endif
}

void RuleEditor::setAdblockEnabled(bool flag)
{
    enableButtons(flag);
    m_accessPolicy->setEnabled(flag);
}

void RuleEditor::enableButtons(bool flag)
{
    foreach (QAbstractButton *button, buttonBox->buttons()) {
        QDialogButtonBox::StandardButton buttonType = buttonBox->standardButton(button);
        if (buttonType != QDialogButtonBox::Cancel)
            button->setEnabled(flag);
    }
}

