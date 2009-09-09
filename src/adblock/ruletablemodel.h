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

#ifndef RULETABLEMODEL_H
#define RULETABLEMODEL_H

#include "networkaccesspolicy.h"

#include <qabstractitemmodel.h>
#include <qlist.h>

class RuleTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    RuleTableModel(NetworkAccessPolicy *datas, QObject *parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    QList<AdBlockRule*> currentRules() const;
    void setCurrentRules(const QList<AdBlockRule*> &newRules);

    void setData(int row, AdBlockRule *rule);
    void append(AdBlockRule *rule);

private:
    QList<AdBlockRule*> m_rules;

};

#endif // RULETABLEMODEL_H
