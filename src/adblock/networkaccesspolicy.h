/*
 * Copyright 2009 Zsombor Gegesy <gzsombor@gmail.com>
 * Copyright 2009 Benjamin Meyer <ben@meyerhome.net>
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

#ifndef NETWORKACCESSPOLICY_H
#define NETWORKACCESSPOLICY_H

#include <qobject.h>

#include "urlaccessrule.h"
#include "filterselector.h"

#include <qfile.h>
#include <qiodevice.h>
#include <qnetworkrequest.h>
#include <qtextstream.h>

// define NETWORKACCESS_DEBUG
class AdBlockSubscription;
class NetworkAccessPolicy : public QObject
{
    Q_OBJECT

public:
    NetworkAccessPolicy(QObject *parent = 0);
    ~NetworkAccessPolicy();

    bool allowedToConnect(const QNetworkRequest &request);

    static bool importAdBlockRules(QTextStream &txt,  QList<UrlAccessRule*> &rules);
    static bool importAdBlockRules(QIODevice &importFrom,  QList<UrlAccessRule*> &rules, const QString &name);
    static bool importAdBlockRulesFromFile(QFile &importFrom,  QList<UrlAccessRule*> &rules);
    static void exportSettings(QList<UrlAccessRule*> &rules, QFile &fileToExport);

    const QList<UrlAccessRule*> *accessRules() const;
    void setAccessRules(QList<UrlAccessRule*> &newRules);

    void setAccessRules(AdBlockSubscription *subscription, QList<UrlAccessRule*> &newRules);
    QList<AdBlockSubscription*> &subscriptions() { return m_subscriptions; }

    void setEnabled(bool flag) { m_enabled = flag; }

    bool isEnabled() { return m_enabled; }

    void load();
    void save();

public slots:
    void subscriptionStateChanged();

private:
    FilterSelector m_acceptRules;
    FilterSelector m_blockRules;
    QList<UrlAccessRule*> *m_rules;
    QList<AdBlockSubscription*> m_subscriptions;
    bool m_enabled;
#if defined(NETWORKACCESS_DEBUG)
    int m_elapsedTime;
#endif

};

#endif // NETWORKACCESSPOLICY_H
