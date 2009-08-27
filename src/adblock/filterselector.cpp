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

#include "filterselector.h"

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

#define HASH_LEN 8

FilterSelector::FilterSelector(bool exceptionRules, QObject *parent)
    : QObject(parent)
    , m_exceptionRules(exceptionRules)
    , m_allRules(0)
{
}

const UrlAccessRule *FilterSelector::get(const QString &url) const
{
    for (int i = 0; i < url.length() - HASH_LEN; ++i) {
        QString substr = url.mid(i, HASH_LEN);
        UrlAccessRule *candidate = m_ruleHash.value(substr);
        if (candidate && candidate->match(url))
            return candidate;
    }

#if defined(NETWORKACCESS_DEBUG)
     qDebug() << "get rule for " << url;
#endif
    for (int i = 0; i < m_extraRules.length(); ++i) {
        UrlAccessRule *candidate = m_extraRules.at(i);
        if (candidate->match(url)) {
#if defined(NETWORKACCESS_DEBUG)
     qDebug() << "rule " << candidate->toString() << " MATCH";
#endif
            return candidate;
        } else {
#if defined(NETWORKACCESS_DEBUG)
     qDebug() << "rule " << candidate->toString() << " NOT MATCH";
#endif
        }
    }

    return 0;
}

void FilterSelector::setRules(QList<UrlAccessRule*> *rules)
{
    m_allRules = rules;
    rehash();
}

void FilterSelector::rehash()
{
    m_ruleHash.clear();
    m_extraRules.clear();
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "rehash of " << m_allRules->size() << " rules.";
#endif

    for (int i = 0; i < m_allRules->size(); ++i) {
        UrlAccessRule *rule = m_allRules->at(i);

        // filter out not enabled rules, or rules which are for blocking, when this filter selector is for accepting
        if (rule->isLiveRule() && rule->isExceptionRule() == m_exceptionRules) {
            bool foundHash = false;

            if (!rule->isRegexpRule()) {
                QString pattern = rule->pattern();

                for (int i = 0; (i < (pattern.length() - HASH_LEN)) && !foundHash; ++i) {
                    QString substr = pattern.mid(i, HASH_LEN);

                    if (!substr.contains(QLatin1Char('*'))
                        && !substr.contains(QLatin1Char('|'))
                        && !m_ruleHash.contains(substr)) {

                        m_ruleHash.insert(substr, rule);
#if defined(NETWORKACCESS_DEBUG)
                        qDebug() << "adding rule " << pattern << " with hash " << substr;
#endif
                        foundHash = true;
                    }
                }
            }

            if (!foundHash) {
#if defined(NETWORKACCESS_DEBUG)
                qDebug() << "adding as extra rule : " << rule->pattern();
#endif
                m_extraRules.append(rule);
            }
        }
    }

#if defined(NETWORKACCESS_DEBUG)
    qDebug() << " rehash finished, extra rules:" << m_extraRules.size()
             << " hashed : " << m_ruleHash.size() << "mode " << m_exceptionRules;
#endif
}
