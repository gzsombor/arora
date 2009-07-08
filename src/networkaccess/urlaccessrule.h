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

#ifndef URLACCESSRULE_H
#define URLACCESSRULE_H

#include "filtersubscription.h"

#include <qobject.h>
#include <qregexp.h>
#include <qurl.h>

enum Decision {
    Undecided, Allow, Deny
};

class UrlAccessRule: public QObject
{
public:
    UrlAccessRule(bool wildcard, const QString &pattern, bool exception, int hitCount, bool enabled = true,
                  FilterSubscription *filterSubscription = 0, QObject *parent = 0);
    ~UrlAccessRule();
    Decision decide(const QUrl &url) const;

    QString toString() const;

    bool isExceptionRule() const;
    QString pattern() const;
    bool isRegexpRule() const;
    bool isEnabled() const;

    int hitCount() const;
    void setHitCount(int newCount);
    void incrementHitCount();

    FilterSubscription *filterSubscription() const;
    void setFilterSubscription(FilterSubscription *newSubs);
    void setEnabled(bool enabled);

    bool isEditable() const;

    const QString *hash() const;
    void setHash(const QString &hash);

    /**
      * return true, when it's enabled, and the subscription if it exists is enabled too.
      */
    bool isLiveRule() const;

    bool match(const QString &url) const;

private:
    QString m_pattern;
    QRegExp *m_regexp;
    bool m_exceptionRule;
    int m_hitCount;
    bool m_enabled;
    FilterSubscription *m_subscription;
    QString *m_hash;


};

#endif // URLACCESSRULE_H
