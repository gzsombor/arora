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

#ifndef ADBLOCKSUBSCRIPTION_H
#define ADBLOCKSUBSCRIPTION_H

#include <qdatetime.h>
#include <qurl.h>
#include <qmetatype.h>

class AdBlockSubscription
{

public:
    AdBlockSubscription();
    AdBlockSubscription(int priority, QString &name, QString &url, QDate &lastFetch, bool enabled);

    int priority() const;
    void setPriority(int priority);

    void setName(const QString &name);
    QString name() const;

    void setUrl(const QUrl &url);
    QUrl url() const;

    void setLastFetchedDate(const QDate &date);
    QDate lastFetchedDate() const;

    void setEnabled(bool enabled);
    bool isEnabled() const;

    static void load(QDataStream &in, AdBlockSubscription &subscription);
    static void save(QDataStream &out, const AdBlockSubscription &subscription);

    // static void parseUrl(const QUrl &url);
    // "abp:subscribe?location=http%3A%2F%2Feasylist.adblockplus.org%2Feasylist.txt&title=EasyList"

private:
    int m_priority;
    QString m_name;
    QString m_url;
    QDate m_lastFetchedDate;
    bool m_enabled;
    // QList<AdBlockFilter> rules
};

QDataStream &operator<<(QDataStream &, const AdBlockSubscription &subscription);
QDataStream &operator>>(QDataStream &, AdBlockSubscription &subscription);

Q_DECLARE_METATYPE(AdBlockSubscription)
Q_DECLARE_METATYPE(QList<AdBlockSubscription>)

#endif // FILTERSUBSCRIPTION_H
