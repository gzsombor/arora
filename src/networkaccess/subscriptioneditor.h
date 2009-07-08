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

#ifndef SUBSCRIPTIONEDITOR_H
#define SUBSCRIPTIONEDITOR_H

#include <qdialog.h>
#include "ui_subscriptioneditor.h"

#include "networkaccesspolicy.h"
#include "subscriptiontablemodel.h"

#include <qdialog.h>
#include <qtableview.h>

class SubscriptionEditor : public QDialog, public Ui_SubscriptionEditor
{
    Q_OBJECT

public:
    SubscriptionEditor(NetworkAccessPolicy *accessPolicy, QWidget *parent = 0);

private slots:
    void enableCheckBoxChanged();
    void on_fetchButton_clicked();
    void selectionChanged(const QModelIndex &current, const QModelIndex &prev);

private:
    NetworkAccessPolicy *m_accessPolicy;
    SubscriptionTableModel *m_model;
};

#endif // SUBSCRIPTIONEDITOR_H
