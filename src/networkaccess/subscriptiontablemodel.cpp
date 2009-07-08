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

#include "subscriptiontablemodel.h"

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

SubscriptionTableModel::SubscriptionTableModel(QList<FilterSubscription*> &subscription)
    : m_subscriptions(subscription)
{
}

QVariant SubscriptionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Last Fetch");
            case 2:
                return tr("Enabled");
            }
        } else {
            return QVariant();
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant SubscriptionTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_subscriptions.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        int row = index.row();
        switch (index.column()) {
        case 0:
            return QVariant(m_subscriptions.at(row)->name());
        case 1:
            return QVariant(m_subscriptions.at(row)->lastFetchDate());
        case 2:
            return QVariant(m_subscriptions.at(row)->isEnabled() ? tr("Enabled") : tr("Disabled"));
        }
    }

    return QVariant();
}

int SubscriptionTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

int SubscriptionTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_subscriptions.size();
}

FilterSubscription *SubscriptionTableModel::at(int index) const
{
    return m_subscriptions.at(index);
}

void SubscriptionTableModel::emitDataChanged(const QModelIndex &row)
{
    emit dataChanged(row.sibling(row.row(), 0), row.sibling(row.row(), 3));
}
