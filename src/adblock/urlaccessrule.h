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

#include <qobject.h>
#include <qregexp.h>
#include <qurl.h>

class AdBlockSubscription;
class UrlAccessRule: public QObject
{
public:
    enum Decision {
        Undecided, Allow, Deny
    };
    UrlAccessRule(bool wildcard, const QString &pattern, bool exception, int hitCount, bool enabled = true,
                  AdBlockSubscription *adBlockSubscription = 0, QObject *parent = 0);
    UrlAccessRule(QString &line, QObject *parent = 0);

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

    AdBlockSubscription *subscription() const;
    void setAdBlockSubscription(AdBlockSubscription *newSubs);
    void setEnabled(bool enabled);

    bool isEditable() const;

    const QString *hash() const;
    void setHash(const QString &hash);

    /**
      * return true, when it's enabled, and the subscription if it exists is enabled too.
      */
    bool isLiveRule() const;

    bool match(const QString &url) const;

    static UrlAccessRule *parse(QString &line, QObject *parent = 0);

private:
    bool m_enabled;
    bool m_exceptionRule;
    int m_hitCount;
    QString m_pattern;
    QRegExp *m_regexp;
    AdBlockSubscription *m_subscription;
    QString *m_hash;
};

#endif // URLACCESSRULE_H
