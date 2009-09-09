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

#include <qurl.h>
#include <qstringlist.h>

class QRegExp;
class AdBlockSubscription;
class AdBlockRule
{

public:
    AdBlockRule(const QString &filter = QString());

    bool isNull() const { return m_isNull; }
    QString toString() const;
    bool match(const QString &encodedUrl) const;

    void setExceptionRule(bool exception);
    bool isExceptionRule() const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    int hitCount() const;
    void setHitCount(int newCount);
    inline void incrementHitCount() { ++m_hitCount; }

    QString pattern() const;
    bool isRegexpRule() const;
    void setPattern(bool regexpRule, const QString &newPattern);

    AdBlockSubscription *subscription() const;
    void setAdBlockSubscription(AdBlockSubscription *newSubs);

    bool isEditable() const;
    bool isLiveRule() const;

    void load(QDataStream &in);
    void save(QDataStream &out) const;

protected:
    void parseFilter(const QString &filter);
    static QString convertPattern(const QString &wildcardPattern);
    QString regexpPattern() const { return m_regexp.pattern(); }

private:
    QString m_filter;
    bool m_isNull;
    QStringList m_options;

    bool m_enabled;
    bool m_exceptionRule;
    bool m_regexpRule;
    int m_hitCount;
    QString m_pattern;
    QRegExp m_regexp;
    AdBlockSubscription *m_subscription;
};

QDataStream &operator<<(QDataStream &, const AdBlockRule &rule);
QDataStream &operator>>(QDataStream &, AdBlockRule &rule);

#endif // ADBLOCKRULE_H

