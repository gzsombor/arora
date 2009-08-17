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

#ifndef SUBSCRIPTIONDOWNLOAD_H
#define SUBSCRIPTIONDOWNLOAD_H

#include "adblocksubscription.h"
#include "networkaccesspolicy.h"
#include "subscriptiontablemodel.h"

#include <qobject.h>
#include <qnetworkreply.h>

class SubscriptionDownload : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SubscriptionDownload)

public:
    explicit SubscriptionDownload(NetworkAccessPolicy *policy, AdBlockSubscription *destination,
                                  SubscriptionTableModel *tableModel, const QModelIndex &index);
    ~SubscriptionDownload();

protected slots:
    void downloadFinished();

private:
    AdBlockSubscription *m_destination;
    NetworkAccessPolicy *m_policy;
    SubscriptionTableModel *m_tableModel;
    QModelIndex m_index;
    QNetworkReply *m_reply;
};

#endif // SUBSCRIPTIONDOWNLOAD_H
