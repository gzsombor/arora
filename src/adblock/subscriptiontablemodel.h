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

#ifndef SUBSCRIPTIONTABLEMODEL_H
#define SUBSCRIPTIONTABLEMODEL_H

#include "adblocksubscription.h"

#include <qabstractitemmodel.h>

class SubscriptionTableModel : public QAbstractTableModel
{
public:
    SubscriptionTableModel(QList<AdBlockSubscription*> &subscription);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    AdBlockSubscription *at(int index) const;

    void emitDataChanged(const QModelIndex &row);

private:
    QList<AdBlockSubscription*> &m_subscriptions;
};

#endif // SUBSCRIPTIONTABLEMODEL_H
