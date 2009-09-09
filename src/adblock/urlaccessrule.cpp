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

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

UrlAccessRule::UrlAccessRule(bool regexpRule, const QString &pattern, bool exception, int hitCount, bool enabled, AdBlockSubscription *adBlockSubscription, QObject *parent)
    : QObject(parent)
    , m_enabled(enabled)
    , m_hitCount(hitCount)
    , m_pattern(pattern)
    , m_hash(0)
{
    m_subscription = adBlockSubscription;
    m_exceptionRule = exception;
    m_regexpRule = regexpRule;

    setPattern(regexpRule, pattern);

#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "url access rule " << m_pattern << " regexpRule:" << m_regexpRule << " regexp :" << m_regexp->pattern();
#endif
}

UrlAccessRule::UrlAccessRule(QString &line, QObject *parent)
    : QObject(parent),m_enabled(true),m_hitCount(0),m_subscription(0),m_hash(0)
{
    m_exceptionRule = false;
    if (line.startsWith(QLatin1String("@@"))) {
        m_exceptionRule = true;
        line = line.right(line.size() - 2);
    }
    m_regexpRule = false;
    if (line.startsWith(QLatin1Char('/'))) {
        m_regexpRule = true;
        line = line.right(line.size() - 1);
        if (line.endsWith(QLatin1Char('/'))) {
            line = line.left(line.size() - 1);
        }
    }
    int dollarSign = line.indexOf(QLatin1String("$"), 0);
    if (dollarSign >= 0) {
        // some filter contain 'directives' like '$script,image' or '$link,object'
        // seen : 'third-party,other,object_subrequest, script, image, link, object
        line = line.left(dollarSign);
    }
    setPattern(m_regexpRule, line.trimmed());
}

UrlAccessRule::UrlAccessRule(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
    , m_hitCount(0)
    , m_regexp(0)
    , m_regexpRule(false)
    , m_subscription(0)
    , m_hash(0)
{
}

QString UrlAccessRule::convertPattern(QString wildcardPattern) {
    return wildcardPattern.replace(QRegExp(QLatin1String("\\*+")), QLatin1String("*"))   // remove multiple wildcards
        .replace(QRegExp(QLatin1String("\\^\\|$")), QLatin1String("^"))        // remove anchors following separator placeholder
        .replace(QRegExp(QLatin1String("^(\\*)")), QLatin1String(""))          // remove leading wildcards
        .replace(QRegExp(QLatin1String("(\\*)$")), QLatin1String(""))          // remove trailing wildcards
        .replace(QRegExp(QLatin1String("(\\W)")), QLatin1String("\\\\1"))      // escape special symbols
        .replace(QRegExp(QLatin1String("^\\\\\\|\\\\\\|")),
                 QLatin1String("^[\\w\\-]+:\\/+(?!\\/)(?:[^\\/]+\\.)?"))       // process extended anchor at expression start
        .replace(QRegExp(QLatin1String("\\\\\\^")),
                 QLatin1String("(?:[^\\w\\d\\-.%]|$)"))                        // process separator placeholders
        .replace(QRegExp(QLatin1String("^\\\\\\|")), QLatin1String("^"))       // process anchor at expression start
        .replace(QRegExp(QLatin1String("\\\\\\|$")), QLatin1String("$"))       // process anchor at expression end
        .replace(QRegExp(QLatin1String("\\\\\\*")), QLatin1String(".*"))       // replace wildcards by .*
        ;
}

UrlAccessRule::~UrlAccessRule()
{
    if (m_hash) {
        delete m_hash;
        m_hash = 0;
    }
    if (m_regexp) {
        delete m_regexp;
        m_regexp = 0;
    }
}


void UrlAccessRule::setPattern(bool regexpRule, QString newPattern)
{
    m_regexpRule = regexpRule;
    m_pattern = newPattern;
    m_regexp = new QRegExp(m_regexpRule ? newPattern : convertPattern(newPattern),
                           Qt::CaseInsensitive, QRegExp::RegExp2);
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
    return m_regexp->indexIn(str) != -1;
}

QString UrlAccessRule::toString() const
{
    return QString(QLatin1String("Rule:"))
            + m_regexp->pattern()
            + QLatin1Char(',')
            + (m_exceptionRule ? QLatin1String(" exception") : QLatin1String(" filtering rule"))
            + QLatin1String(", internal pattern: ")
            + m_pattern
            + QLatin1String(", ")
            + (m_regexpRule ? QLatin1String("regexp mode") : QLatin1String("wildcard mode"));

}

bool UrlAccessRule::isExceptionRule() const
{
    return m_exceptionRule;
}

QString UrlAccessRule::pattern() const
{
    return m_pattern;
}

QString UrlAccessRule::regexpPattern() const
{
    return m_regexp->pattern();
}

bool UrlAccessRule::isRegexpRule() const
{
    return m_regexpRule;
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

UrlAccessRule *UrlAccessRule::parse(QString &line, QObject *parent)
{
    if (!line.startsWith(QLatin1String("!")) && !line.contains(QLatin1Char('#'))) {
        if (line.trimmed().length()>0) {
            return new UrlAccessRule(line, parent);
        }
    }
    return 0;
}

QDataStream &operator>>(QDataStream &in, UrlAccessRule &rule)
{
    rule.load(in);
    return in;
}

QDataStream &operator<<(QDataStream &out, const UrlAccessRule &rule)
{
    rule.save(out);
    return out;
}

void UrlAccessRule::load(QDataStream &in)
{
    QString pattern;
    bool regexpRule;
    in >> m_enabled;
    in >> pattern;
    in >> m_exceptionRule;
    in >> regexpRule;
    in >> m_hitCount;
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "load " << m_enabled << pattern << m_exceptionRule << regexpRule << m_hitCount;
#endif
    setPattern(regexpRule, pattern);
}

void UrlAccessRule::save(QDataStream &out) const
{
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "save " << m_enabled << m_pattern << m_exceptionRule << m_regexpRule << m_hitCount;
#endif
    out << m_enabled;
    out << m_pattern;
    out << m_exceptionRule;
    out << m_regexpRule;
    out << m_hitCount;
}

