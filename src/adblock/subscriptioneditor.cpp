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

#include "subscriptioneditor.h"

#include "subscriptiontablemodel.h"
#include "subscriptiondownload.h"

#include <qtableview.h>

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

SubscriptionEditor::SubscriptionEditor(NetworkAccessPolicy *accessPolicy, QWidget *parent)
    : QDialog(parent)
{
    m_accessPolicy = accessPolicy;
    setupUi(this);
    m_model = new SubscriptionTableModel(m_accessPolicy->subscriptions());

    int height = tableView->fontMetrics().height();
    tableView->verticalHeader()->setDefaultSectionSize(height);
    tableView->setModel(m_model);
    tableView->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

    connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(selectionChanged(const QModelIndex &, const QModelIndex &)));
    connect(enableCheckBox, SIGNAL(clicked()),
            this, SLOT(enableCheckBoxChanged()));
    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), m_accessPolicy, SLOT(subscriptionStateChanged()));
}

void SubscriptionEditor::selectionChanged(const QModelIndex &current, const QModelIndex &prev)
{
    Q_UNUSED(prev);
    if (current.row() < 0 || m_model->rowCount() <= current.row())
        return;

    AdBlockSubscription *filterSubs = m_model->at(current.row());
    nameField->setText(filterSubs->name());
    urlField->setText(QString::fromUtf8(filterSubs->url().toEncoded()));
    dateEdit->setDate(filterSubs->lastFetchedDate());
    enableCheckBox->setChecked(filterSubs->isEnabled());
}

void SubscriptionEditor::on_fetchButton_clicked()
{
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QModelIndexList selected = tableView->selectionModel()->selectedRows();
    foreach (const QModelIndex &index, selected) {
        AdBlockSubscription *filterSubs = m_model->at(index.row());
        new SubscriptionDownload(m_accessPolicy, filterSubs, m_model, index);
    }
}

void SubscriptionEditor::enableCheckBoxChanged()
{
    QModelIndexList indexList = tableView->selectionModel()->selectedRows();
    foreach (const QModelIndex &index, indexList) {
        AdBlockSubscription *filterSubs = m_model->at(index.row());
        filterSubs->setEnabled(enableCheckBox->isChecked());
        emit m_model->emitDataChanged(index);
    }
}

