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

#include "autosaver.h"
#include "filterselector.h"

#include <qfile.h>
#include <qiodevice.h>
#include <qnetworkrequest.h>
#include <qtextstream.h>

#define NETWORKACCESS_DEBUG

class AdBlockSubscription;
class NetworkAccessPolicy : public QObject
{
    Q_OBJECT

public:
    NetworkAccessPolicy(QObject *parent = 0);
    ~NetworkAccessPolicy();

    bool allowedToConnect(const QNetworkRequest &request);

    static bool importAdBlockRules(QTextStream &txt,  QList<AdBlockRule*> &rules);
    static bool importAdBlockRules(QIODevice &importFrom,  QList<AdBlockRule*> &rules, const QString &name);
    static bool importAdBlockRulesFromFile(QFile &importFrom,  QList<AdBlockRule*> &rules);
    static void exportSettings(QList<AdBlockRule*> &rules, QFile &fileToExport);

    const QList<AdBlockRule*> *accessRules() const;
    void setAccessRules(QList<AdBlockRule*> &newRules);

    void setAccessRules(AdBlockSubscription *subscription, QList<AdBlockRule*> &newRules);
    QList<AdBlockSubscription*> &subscriptions() { return m_subscriptions; }

    void setEnabled(bool flag) { m_enabled = flag; }

    bool isEnabled() { return m_enabled; }

    void load();

public slots:
    void subscriptionStateChanged();
    void save();

private:
    FilterSelector m_acceptRules;
    FilterSelector m_blockRules;
    QList<AdBlockRule*> *m_rules;
    QList<AdBlockSubscription*> m_subscriptions;
    bool m_enabled;
    AutoSaver m_autoSaver;
#if defined(NETWORKACCESS_DEBUG)
    int m_elapsedTime;
#endif

};

#endif // NETWORKACCESSPOLICY_H

