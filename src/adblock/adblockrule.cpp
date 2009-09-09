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

#include "adblockrule.h"

#include "adblocksubscription.h"

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

AdBlockRule::AdBlockRule(bool regexpRule, const QString &pattern,
                         bool exception, int hitCount, bool enabled,
                         AdBlockSubscription *adBlockSubscription, QObject *parent)
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

AdBlockRule::AdBlockRule(QString &line, QObject *parent)
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

AdBlockRule::AdBlockRule(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
    , m_regexpRule(false)
    , m_hitCount(0)
    , m_regexp(0)
    , m_subscription(0)
    , m_hash(0)
{
}

QString AdBlockRule::convertPattern(QString wildcardPattern) {
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

AdBlockRule::~AdBlockRule()
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

void AdBlockRule::setPattern(bool regexpRule, QString newPattern)
{
    m_regexpRule = regexpRule;
    m_pattern = newPattern;
    m_regexp = new QRegExp(m_regexpRule ? newPattern : convertPattern(newPattern),
                           Qt::CaseInsensitive, QRegExp::RegExp2);
}

AdBlockRule::Decision AdBlockRule::decide(const QUrl &url) const
{
    if (!m_enabled)
        return Undecided;

    if (m_subscription != 0 && !m_subscription->isEnabled())
        return Undecided;

    QString str = url.toString();
    return match(str) ? (m_exceptionRule ? Allow : Deny) : Undecided;
}

bool AdBlockRule::match(const QString &str) const
{
    return m_regexp->indexIn(str) != -1;
}

QString AdBlockRule::toString() const
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

bool AdBlockRule::isExceptionRule() const
{
    return m_exceptionRule;
}

QString AdBlockRule::pattern() const
{
    return m_pattern;
}

QString AdBlockRule::regexpPattern() const
{
    return m_regexp->pattern();
}

bool AdBlockRule::isRegexpRule() const
{
    return m_regexpRule;
}

const QString *AdBlockRule::hash() const
{
    return m_hash;
}

void AdBlockRule::setHash(const QString &hash)
{
    m_hash = new QString(hash);
}

bool AdBlockRule::isEnabled() const
{
    return m_enabled;
}

int AdBlockRule::hitCount() const
{
    return m_hitCount;
}

void AdBlockRule::setHitCount(int newCount)
{
    m_hitCount = newCount;
}

void AdBlockRule::incrementHitCount()
{
    ++m_hitCount;
}

AdBlockSubscription *AdBlockRule::subscription() const
{
    return m_subscription;
}

void AdBlockRule::setAdBlockSubscription(AdBlockSubscription *newSubs)
{
    m_subscription = newSubs;
}

void AdBlockRule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool AdBlockRule::isEditable() const
{
    return m_subscription == 0;
}

bool AdBlockRule::isLiveRule() const
{
    return m_enabled && (m_subscription == 0 || m_subscription->isEnabled());
}

AdBlockRule *AdBlockRule::parse(QString &line, QObject *parent)
{
    if (!line.startsWith(QLatin1String("!")) && !line.contains(QLatin1Char('#'))) {
        if (line.trimmed().length()>0) {
            return new AdBlockRule(line, parent);
        }
    }
    return 0;
}

QDataStream &operator>>(QDataStream &in, AdBlockRule &rule)
{
    rule.load(in);
    return in;
}

QDataStream &operator<<(QDataStream &out, const AdBlockRule &rule)
{
    rule.save(out);
    return out;
}

void AdBlockRule::load(QDataStream &in)
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

void AdBlockRule::save(QDataStream &out) const
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

