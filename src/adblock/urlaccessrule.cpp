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

#include "urlaccessrule.h"

#include "adblocksubscription.h"

UrlAccessRule::UrlAccessRule(bool wildcard, const QString &pattern, bool exception, int hitCount, bool enabled,
                             AdBlockSubscription *AdBlockSubscription, QObject *parent)
    : QObject(parent),m_pattern(pattern),m_enabled(enabled),m_hash(0)
{
    m_subscription = AdBlockSubscription;
    m_exceptionRule = exception;

    if (wildcard) {
        QString cPattern = pattern;

        if (cPattern.startsWith(QLatin1Char('|')))
            cPattern.remove(0, 1);
        else
            cPattern.prepend(QLatin1Char('*'));

        if (cPattern.endsWith(QLatin1Char('|')))
            cPattern.remove(cPattern.length()-1, 1);
        else
            cPattern.append(QLatin1Char('*'));

        m_regexp = new QRegExp(cPattern, Qt::CaseInsensitive, QRegExp::Wildcard);
    } else {
        m_regexp = new QRegExp(pattern, Qt::CaseInsensitive, QRegExp::RegExp2);
    }

    m_hitCount = hitCount;
}

UrlAccessRule::~UrlAccessRule()
{
    if (m_hash) {
        delete m_hash;
        m_hash = 0;
    }
    delete m_regexp;
}

UrlAccessRule::Decision UrlAccessRule::decide(const QUrl &url) const
{
    if (!m_enabled)
        return Undecided;

    if (m_subscription != 0 && !m_subscription->isEnabled())
        return Undecided;

    QString str = url.toString();
    return match(str) ? (m_exceptionRule ? Allow : Deny) : Undecided;
}

bool UrlAccessRule::match(const QString &str) const
{
    return m_regexp->exactMatch(str);
}

QString UrlAccessRule::toString() const
{
    return QString(QLatin1String("Rule:"))
            + m_regexp->pattern()
            + QLatin1Char(',')
            + (m_exceptionRule ? QLatin1String(" exception") : QLatin1String(" filtering rule"));
}

bool UrlAccessRule::isExceptionRule() const
{
    return m_exceptionRule;
}

QString UrlAccessRule::pattern() const
{
    return m_pattern;
}

bool UrlAccessRule::isRegexpRule() const
{
    return m_regexp->patternSyntax() == QRegExp::RegExp2;
}

const QString *UrlAccessRule::hash() const
{
    return m_hash;
}

void UrlAccessRule::setHash(const QString &hash)
{
    m_hash = new QString(hash);
}

bool UrlAccessRule::isEnabled() const
{
    return m_enabled;
}

int UrlAccessRule::hitCount() const
{
    return m_hitCount;
}

void UrlAccessRule::setHitCount(int newCount)
{
    m_hitCount = newCount;
}

void UrlAccessRule::incrementHitCount()
{
    ++m_hitCount;
}

AdBlockSubscription *UrlAccessRule::subscription() const
{
    return m_subscription;
}

void UrlAccessRule::setAdBlockSubscription(AdBlockSubscription *newSubs)
{
    m_subscription = newSubs;
}

void UrlAccessRule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool UrlAccessRule::isEditable() const
{
    return m_subscription == 0;
}

bool UrlAccessRule::isLiveRule() const
{
    return m_enabled && (m_subscription == 0 || m_subscription->isEnabled());
}
