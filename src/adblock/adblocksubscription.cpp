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

#include "adblocksubscription.h"

AdBlockSubscription::AdBlockSubscription()
    : m_priority(0)
    , m_enabled(false)
{
}

AdBlockSubscription::AdBlockSubscription(int priority, QString &name, QString &url, QDate &lastFetch, bool enabled)
    : m_priority(priority)
    , m_name(name)
    , m_url(url)
    , m_lastFetchedDate(lastFetch)
    , m_enabled(enabled)
{

}


void AdBlockSubscription::setPriority(int priority)
{
    m_priority = priority;
}

int AdBlockSubscription::priority() const
{
    return m_priority;
}

void AdBlockSubscription::setName(const QString &name)
{
    m_name = name;
}

QString AdBlockSubscription::name() const
{
    return m_name;
}

void AdBlockSubscription::setUrl(const QUrl &url)
{
    m_url = url.toString();
}

QUrl AdBlockSubscription::url() const
{
    return QUrl(m_url);
}

void AdBlockSubscription::setLastFetchedDate(const QDate &date)
{
    m_lastFetchedDate = date;
}

QDate AdBlockSubscription::lastFetchedDate() const
{
    return m_lastFetchedDate;
}

void AdBlockSubscription::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool AdBlockSubscription::isEnabled() const
{
    return m_enabled;
}

QDataStream &operator>>(QDataStream &in, AdBlockSubscription &subscription)
{
    AdBlockSubscription::load(in, subscription);
    return in;
}

QDataStream &operator<<(QDataStream &out, const AdBlockSubscription &subscription)
{
    AdBlockSubscription::save(out, subscription);
    return out;
}

void AdBlockSubscription::load(QDataStream &in, AdBlockSubscription &subscription)
{
    in >> subscription.m_priority;
    in >> subscription.m_name;
    in >> subscription.m_url;
    in >> subscription.m_lastFetchedDate;
    in >> subscription.m_enabled;
}

void AdBlockSubscription::save(QDataStream &out, const AdBlockSubscription &subscription)
{
    out << subscription.priority();
    out << subscription.name();
    out << subscription.m_url;
    out << subscription.lastFetchedDate();
    out << subscription.isEnabled();
}

