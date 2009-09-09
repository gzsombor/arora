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

#include <qregexp.h>
#include <qdebug.h>

// #define ADBLOCKRULE_DEBUG

AdBlockRule::AdBlockRule(const QString &filter)
    : m_isNull(false)
    , m_enabled(true)
    , m_exceptionRule(false)
    , m_regexpRule(false)
    , m_hitCount(0)
    , m_subscription(0)
{
    parseFilter(filter);
}

void AdBlockRule::parseFilter(const QString &filter)
{
    m_filter = filter;
    if (filter.startsWith(QLatin1String("!"))
        || filter.contains(QLatin1Char('#'))
        || filter.trimmed().isEmpty()) {
        m_isNull = true;
        return;
    }

    QString parsedLine = filter;
    if (parsedLine.startsWith(QLatin1String("@@"))) {
        m_exceptionRule = true;
        parsedLine = parsedLine.right(parsedLine.size() - 2);
    }
    if (parsedLine.startsWith(QLatin1Char('/'))) {
        m_regexpRule = true;
        parsedLine = parsedLine.right(parsedLine.size() - 1);
        if (parsedLine.endsWith(QLatin1Char('/'))) {
            parsedLine = parsedLine.left(parsedLine.size() - 1);
        }
    }
    int dollarSign = parsedLine.indexOf(QLatin1String("$"), 0);
    if (dollarSign >= 0) {
        m_options = parsedLine.mid(dollarSign + 1).split(QLatin1Char(','));
        // some filter contain 'directives' like '$script,image' or '$link,object'
        // seen : 'third-party,other,object_subrequest, script, image, link, object
        parsedLine = parsedLine.left(dollarSign);
    }
    setPattern(m_regexpRule, parsedLine.trimmed());

    if (m_options.contains(QLatin1String("match-case"))) {
        m_regexp.setCaseSensitivity(Qt::CaseSensitive);
        m_options.removeOne(QLatin1String("match-case"));
    }
}

void AdBlockRule::setPattern(bool regexpRule, const QString &newPattern)
{
    m_regexpRule = regexpRule;
    m_regexp = QRegExp(m_regexpRule ? newPattern : convertPattern(newPattern),
                           Qt::CaseInsensitive, QRegExp::RegExp2);
}

QString AdBlockRule::pattern() const
{
    return m_pattern;
}

bool AdBlockRule::isRegexpRule() const
{
    return m_regexpRule;
}

QString AdBlockRule::convertPattern(const QString &wildcardPattern) {
    QString pattern = wildcardPattern;
    return pattern.replace(QRegExp(QLatin1String("\\*+")), QLatin1String("*"))   // remove multiple wildcards
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

bool AdBlockRule::match(const QString &encodedUrl) const
{
   if (m_isNull) {
#if defined(ADBLOCKRULE_DEBUG)
        qDebug() << "AdBlockRule::" << __FUNCTION__ << "isNull";
#endif
        return false;
    }

    if (!m_enabled) {
#if defined(ADBLOCKRULE_DEBUG)
        qDebug() << "AdBlockRule::" << __FUNCTION__ << "is not enabled";
#endif
        return false;
    }

    if (m_subscription && !m_subscription->isEnabled()) {
#if defined(ADBLOCKRULE_DEBUG)
        qDebug() << "AdBlockRule::" << __FUNCTION__ << "subscription is not enabled";
#endif
        return false;
    }

    bool matched = m_regexp.indexIn(encodedUrl) != -1;

    if (matched
        && !m_options.isEmpty()) {

        // we only support domain right now
        if (m_options.count() == 1) {
            foreach (const QString &option, m_options) {
                if (option.startsWith(QLatin1String("domain="))) {
                    QUrl url = QUrl::fromEncoded(encodedUrl.toUtf8());
                    QString host = url.host();
                    QStringList domainOptions = option.mid(7).split(QLatin1Char('|'));
                    foreach (QString domainOption, domainOptions) {
                        bool negate = domainOption.at(0) == QLatin1Char('~');
                        if (negate)
                            domainOption = domainOption.mid(1);
                        bool hostMatched = domainOption == host;
                        if (hostMatched && !negate)
                            return true;
                        if (!hostMatched && negate)
                            return true;
                    }
                }
            }
        }

#if defined(ADBLOCKRULE_DEBUG)
        qDebug() << "AdBlockRule::" << __FUNCTION__ << "options are currently not supported";
#endif
        return false;
    }

    return matched;
}

QString AdBlockRule::toString() const
{
    return m_filter;
}

void AdBlockRule::setExceptionRule(bool exception)
{
    m_exceptionRule = exception;
}

bool AdBlockRule::isExceptionRule() const
{
    return m_exceptionRule;
}

bool AdBlockRule::isEnabled() const
{
    return m_enabled;
}

void AdBlockRule::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

int AdBlockRule::hitCount() const
{
    return m_hitCount;
}

void AdBlockRule::setHitCount(int newCount)
{
    m_hitCount = newCount;
}

AdBlockSubscription *AdBlockRule::subscription() const
{
    return m_subscription;
}

void AdBlockRule::setAdBlockSubscription(AdBlockSubscription *newSubs)
{
    m_subscription = newSubs;
}

bool AdBlockRule::isEditable() const
{
    return !m_subscription;
}

bool AdBlockRule::isLiveRule() const
{
    return m_enabled && (!m_subscription || m_subscription->isEnabled());
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
    in >> m_filter;
    parseFilter(m_filter);
}

void AdBlockRule::save(QDataStream &out) const
{
    out << m_filter;
}

