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

#include "ruletablemodel.h"

#include "adblockrule.h"

RuleTableModel::RuleTableModel(NetworkAccessPolicy *datas, QObject *parent)
    : QAbstractTableModel(parent)
    , m_rules(*datas->accessRules())
{
}

QVariant RuleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Action");
        case 1:
            return tr("Type");
        case 2:
            return tr("Active");
        case 3:
            return tr("Hits");
        case 4:
            return tr("Rule");
        }
    }
    return QVariant();
}

QVariant RuleTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_rules.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        int row = index.row();
        switch (index.column()) {
        case 0:
            return QVariant(m_rules.at(row)->isExceptionRule() ? tr("Allow") : tr("Block"));
        case 1:
            return QVariant(m_rules.at(row)->isRegexpRule() ? tr("Regexp") : tr("Simple"));
        case 2:
            return QVariant(m_rules.at(row)->isEnabled() ? tr("Enabled") : tr("Disabled"));
        case 3:
            return QVariant(m_rules.at(row)->hitCount());
        case 4:
            return QVariant(m_rules.at(row)->pattern());
        }
    }

    return QVariant();
}

int RuleTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

int RuleTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_rules.size();
}

bool RuleTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count);
    for (int i = 0; i < count; ++i)
        m_rules.removeAt(row);
    endRemoveRows();
    return true;
}

void RuleTableModel::setCurrentRules(const QList<AdBlockRule*> &newRules)
{
    beginRemoveRows(QModelIndex(), 0, m_rules.size());
    m_rules.clear();
    endRemoveRows();
    beginInsertRows(QModelIndex(), 0, newRules.size());
    m_rules.append(newRules);
    endInsertRows();
}

void RuleTableModel::append(AdBlockRule *rule)
{
    beginInsertRows(QModelIndex(), m_rules.size(), m_rules.size());
    m_rules.append(rule);
    endInsertRows();
}

void RuleTableModel::setData(int row, AdBlockRule *rule)
{
    m_rules.replace(row, rule);
    emit dataChanged(index(row, 0), index(row + 1, 3));
}

QList<AdBlockRule*> RuleTableModel::currentRules() const
{
    return m_rules;
}
