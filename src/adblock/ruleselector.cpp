/**
 * Copyright (c) 2009, Zsombor Gegesy <gzsombor@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Benjamin Meyer nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "ruleselector.h"

#define HASH_LEN 8

// #define RULESELECTOR_DEBUG

#if defined(RULESELECTOR_DEBUG)
#include <qdebug.h>
#endif

RuleSelector::RuleSelector(QObject *parent)
        : QObject(parent)
{
}


void RuleSelector::rehash(QList<const AdBlockRule *> &rules, bool exceptionRules)
{
    m_ruleHash.clear();
    m_extraRules.clear();
#if defined(RULESELECTOR_DEBUG)
    qDebug() << __FUNCTION__ << " rehash of " << rules.size() << " rules.";
#endif

    for (int i = 0; i < rules.size(); ++i) {
        const AdBlockRule *rule = rules[i];

        // filter out not enabled rules, or rules which are for blocking, when this filter selector is for accepting
        if (!rule->isCSSRule() && rule->isEnabled() && rule->isException() == exceptionRules) {
            bool foundHash = false;

            if (!rule->isRegexp()) {
                QString pattern = rule->pattern();

                for (int i = 0; (i < (pattern.length() - HASH_LEN)) && !foundHash; ++i) {
                    QString substr = pattern.mid(i, HASH_LEN);

                    if (!substr.contains(QLatin1Char('*'))
                        && !substr.contains(QLatin1Char('|'))
                        && !substr.contains(QLatin1Char('^'))
                        && !m_ruleHash.contains(substr)) {

                        m_ruleHash.insert(substr, rule);
#if defined(RULESELECTOR_DEBUG)
                        qDebug() << "adding rule " << pattern << " with hash " << substr;
#endif
                        foundHash = true;
                    }
                }
            }

            if (!foundHash) {
#if defined(RULESELECTOR_DEBUG)
                qDebug() << "adding as extra rule : " << rule->pattern();
#endif
                m_extraRules.append(rule);
            }
        }
    }

#if defined(RULESELECTOR_DEBUG)
    qDebug() << " rehash finished, extra rules:" << m_extraRules.size()
             << " hashed : " << m_ruleHash.size() << "mode " << exceptionRules;
#endif
}


 const AdBlockRule *RuleSelector::get(const QString &url) const
 {
    for (int i = 0; i < url.length() - HASH_LEN; ++i) {
        QString substr = url.mid(i, HASH_LEN);
        const AdBlockRule *candidate = m_ruleHash.value(substr);
        if (candidate && candidate->networkMatch(url))
            return candidate;
    }

#if defined(RULESELECTOR_DEBUG)
     qDebug() << "get rule for " << url;
#endif
    for (int i = 0; i < m_extraRules.length(); ++i) {
        const AdBlockRule *candidate = m_extraRules.at(i);
        if (candidate->networkMatch(url)) {
#if defined(RULESELECTOR_DEBUG)
     qDebug() << "rule " << candidate->filter() << " MATCH";
#endif
            return candidate;
        } else {
#if defined(RULESELECTOR_DEBUG)
     qDebug() << "rule " << candidate->filter() << " NOT MATCH";
#endif
        }
    }

    return 0;
 }
