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

#include "filtersubscription.h"

FilterSubscription::FilterSubscription(int index, const QString &name, const QString &source, const QDate &lastFetch, bool enabled)
    : m_index(index)
    , m_name(name)
    , m_sourceUrl(source)
    , m_lastFetchDate(lastFetch)
    , m_enabled(enabled)
{
}

QString FilterSubscription::name() const
{
    return m_name;
}

QString FilterSubscription::url() const
{
    return m_sourceUrl;
}

QDate FilterSubscription::lastFetchDate() const
{
    return m_lastFetchDate;
}

bool FilterSubscription::isEnabled() const
{
    return m_enabled;
}

int FilterSubscription::index() const
{
    return m_index;
}

void FilterSubscription::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void FilterSubscription::setLastFetchDate(const QDate &date)
{
    m_lastFetchDate = date;
}
