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

#ifndef FILTERSUBSCRIPTION_H
#define FILTERSUBSCRIPTION_H

#include <qdatetime.h>
#include <qobject.h>
#include <qstring.h>

class FilterSubscription
{

public:
    FilterSubscription(int index, const QString &name, const QString &source, const QDate &lastFetch, bool enabled);

    QString name() const;
    QString url() const;
    QDate lastFetchDate() const;
    bool isEnabled() const;
    int index() const;

    void setEnabled(bool enabled);
    void setLastFetchDate(const QDate &date);

private:
    int m_index;
    QString m_name;
    QString m_sourceUrl;
    QDate m_lastFetchDate;
    bool m_enabled;
};

#endif // FILTERSUBSCRIPTION_H
