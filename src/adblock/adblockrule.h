/*
 * Copyright (c) 2009, Zsombor Gegesy <gzsombor@gmail.com>
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
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

#ifndef ADBLOCKRULE_H
#define ADBLOCKRULE_H

#include <qobject.h>
#include <qregexp.h>
#include <qurl.h>

class AdBlockSubscription;
class AdBlockRule : public QObject
{

public:
    enum Decision {
        Undecided, Allow, Deny
    };

    AdBlockRule(bool regexpRule, const QString &pattern,
                bool exception, int hitCount, bool enabled = true,
                AdBlockSubscription *adBlockSubscription = 0, QObject *parent = 0);
    AdBlockRule(QString &line, QObject *parent = 0);
    AdBlockRule(QObject *parent = 0);
//    AdBlockRule(const AdBlockRule &orig);

    ~AdBlockRule();
    Decision decide(const QUrl &url) const;

    QString toString() const;

    bool isExceptionRule() const;
    QString pattern() const;
    QString regexpPattern() const;
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

    static AdBlockRule *parse(QString &line, QObject *parent = 0);
    static QString convertPattern(QString wildcardPattern);

    void load(QDataStream &in);
    void save(QDataStream &out) const;

private:
    bool m_enabled;
    bool m_exceptionRule;
    bool m_regexpRule;
    int m_hitCount;
    QString m_pattern;
    QRegExp *m_regexp;
    AdBlockSubscription *m_subscription;
    QString *m_hash;

    void setPattern(bool regexpRule, QString newPattern);
};

QDataStream &operator<<(QDataStream &, const AdBlockRule &rule);
QDataStream &operator>>(QDataStream &, AdBlockRule &rule);

#endif // ADBLOCKRULE_H

