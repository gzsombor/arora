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

#include "subscriptiondownload.h"

#include "networkaccessmanager.h"
#include "browserapplication.h"

#include <qnetworkrequest.h>
#include <qnetworkreply.h>
#include <qmessagebox.h>

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

SubscriptionDownload::SubscriptionDownload(NetworkAccessPolicy *policy, AdBlockSubscription *destination,
                                           SubscriptionTableModel *tableModel, const QModelIndex &index)
    : m_index(index)
{
    m_policy = policy;
    m_destination = destination;
    m_tableModel = tableModel;
    m_reply = BrowserApplication::networkAccessManager()->get(QNetworkRequest(QUrl(destination->url())));
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "download started for " << m_destination->url();
#endif
    connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

SubscriptionDownload::~SubscriptionDownload()
{
    delete m_reply;
}

void SubscriptionDownload::downloadFinished()
{
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "download finished for " << m_destination->url();
#endif
    QByteArray response = m_reply->readAll();
    QTextStream txt(response);
    QList<UrlAccessRule*> rules;

    if (NetworkAccessPolicy::importAdBlockRules(txt, rules)) {
        m_policy->setAccessRules(m_destination, rules);
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("AdBlock Rule Subscriptions"));
        msgBox.setText(tr("Rules loaded"));
        msgBox.setInformativeText(tr("%1 rule loaded from %2").arg(QString::number(rules.size()),m_destination->url().toString()));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "rules imported";
#endif
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("AdBlock Rule Subscriptions"));
        msgBox.setText(tr("Rule downloading failed!"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

    QDate now = QDate::currentDate();
    m_destination->setLastFetchedDate(now);
    m_tableModel->emitDataChanged(m_index);
    deleteLater();
}
