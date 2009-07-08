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

#include "networkaccesspolicy.h"

#include <qdesktopservices.h>
#include <qfile.h>
#include <qiodevice.h>
#include <qsettings.h>
#include <qtextstream.h>
#include <qurl.h>

#if defined(NETWORKACCESS_DEBUG)
#include <qdebug.h>
#endif

NetworkAccessPolicy::NetworkAccessPolicy(QObject *parent) :
    QObject(parent), m_acceptRules(true, this), m_blockRules(false, this)
{
    m_rules = new QList<UrlAccessRule*>();
    m_acceptRules.setRules(m_rules);
    m_blockRules.setRules(m_rules);
#if defined(NETWORKACCESS_DEBUG)
    m_elapsedTime = 0;
#endif
    load();
}

NetworkAccessPolicy::~NetworkAccessPolicy()
{
    delete m_rules;
}

bool NetworkAccessPolicy::allowedToConnect(const QNetworkRequest &request)
{
    if (!m_enabled) {
        // if AdBlock is not enabled, allow anything ..
        return true;
    }

    QUrl url = request.url();
    QString host = url.host();
    if (url.toString().startsWith(QLatin1String("data:"))) {
#if defined(NETWORKACCESS_DEBUG)
            qDebug() << "access to " << url.toString() << " is allowed anytime";
#endif
        return true;
    }

#if 0
#if defined(NETWORKACCESS_DEBUG)
    QTime time;
    time.start();
#endif
    for (int i = 0; i < m_rules.size(); ++i) {
        const UrlAccessRule *rule = m_rules.at(i);
        Decision decision = rule->decide(url);
        if (decision != Undecided) {
            const_cast<UrlAccessRule*>(rule)->incrementHitCount();
#if defined(NETWORKACCESS_DEBUG)
            int elapsedTime = time.elapsed();
            m_elapsedTime += elapsedTime;
            qDebug() << "access to " << url.toString() << " rule matched[" << i << "] : "
                    << rule->toString() << " result : " << (decision < 0 ? "allowed" : "block")
                    << " elapsed " << elapsedTime;
#endif
            return Allow == decision;
        }
    }
#if defined(NETWORKACCESS_DEBUG)
    int elapsedTime = time.elapsed();
    m_elapsedTime += elapsedTime;
    qDebug() << "elapsed " << time.elapsed() << " for " << url << " full time:" <<m_elapsedTime;
#endif
#endif

    QString urlString = url.toString();
    const UrlAccessRule *rule = m_acceptRules.get(urlString);
    if (rule) {
        const_cast<UrlAccessRule*>(rule)->incrementHitCount();
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "access to " << urlString << " matched"
                    << rule->toString() << " --> ALLOW ";
#endif
        return true;
    }
    rule = m_blockRules.get(urlString);
    if (rule) {
        const_cast<UrlAccessRule*>(rule)->incrementHitCount();
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "access to " << urlString << " matched"
                    << rule->toString() << " --> BLOCK ";
#endif
        return false;
    }
    return true;
}

bool NetworkAccessPolicy::importAdBlockRulesFromFile(QFile &importFrom,  QList<UrlAccessRule*> &rules)
{
    if (!importFrom.exists()) {
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << QLatin1String("Data file not found : ") << importFrom.fileName();
#endif
        return false;
    }
    return NetworkAccessPolicy::importAdBlockRules(importFrom, rules, importFrom.fileName());
}

bool NetworkAccessPolicy::importAdBlockRules(QIODevice &importFrom,  QList<UrlAccessRule*> &rules, const QString &name)
{
    Q_UNUSED(name)
    if (!importFrom.open(QIODevice::ReadOnly)) {
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << QLatin1String("Unable to open : ") << name;
#endif
        return false;
    }
    QTextStream txt(&importFrom);
    return NetworkAccessPolicy::importAdBlockRules(txt, rules);
}

bool NetworkAccessPolicy::importAdBlockRules(QTextStream &txt,  QList<UrlAccessRule*> &rules)
{
    QString header = txt.readLine(1024);
    if (!header.startsWith(QLatin1String("[Adblock"))) {
        return false;
    }
    QString line;
    rules.clear();
    do {
        line = txt.readLine(1024);
        if (!line.startsWith(QLatin1String("!")) && !line.contains(QLatin1Char('#'))) {
            bool exception = false;

            if (line.startsWith(QLatin1String("@@"))) {
                exception = true;
                line = line.right(line.size() - 2);
            }
            bool wildcard = true;
            if (line.startsWith(QLatin1Char('/'))) {
                wildcard = false;
                line = line.right(line.size() - 1);
                if (line.endsWith(QLatin1Char('/'))) {
                    line = line.left(line.size() - 1);
                }
            }
            int dollarSign = line.indexOf(QLatin1String("$"), 0);
            if (dollarSign >= 0) {
                // some filter contain 'directives' like '$script,image' or '$link,object'
                // seen : 'third-party,other,object_subrequest, script, image, link, object
#if defined(NETWORKACCESS_DEBUG)
                qDebug() << "line :" << line;
#endif
                line = line.left(dollarSign);
            }
            UrlAccessRule *rule = new UrlAccessRule(wildcard, line, exception, 0);
#if defined(NETWORKACCESS_DEBUG)
            qDebug() << rule->toString();
#endif
            rules.append(rule);
        }
    } while (!txt.atEnd());

    return true;
}

void NetworkAccessPolicy::exportSettings(QList<UrlAccessRule*> &rules, QFile &fileToExport)
{
    Q_UNUSED(rules)
    if (!fileToExport.open(QIODevice::WriteOnly)) {
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << QLatin1String("Unable to open : ") << fileToExport.fileName();
#endif
        return;
    }
    QTextStream txt(&fileToExport);
    txt << "[Adblock Plus 0.7.1]";
}


const QList<UrlAccessRule*> *NetworkAccessPolicy::accessRules() const
{
    return m_rules;
}

void NetworkAccessPolicy::setAccessRules(QList<UrlAccessRule*> &newRules)
{
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "setAccessRules: " << newRules.size();
#endif
    m_rules->clear();
    m_rules->append(newRules);
    m_acceptRules.rehash();
    m_blockRules.rehash();
    save();
}

void NetworkAccessPolicy::setAccessRules(FilterSubscription *subscription, QList<UrlAccessRule*> &newRules)
{
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "setAccessRules for : " << subscription->name() << " len : " << newRules.size();
#endif

    for (int i= m_rules->size() - 1; i >= 0; --i) {
        UrlAccessRule *rule = m_rules->at(i);
        if (rule->filterSubscription() == subscription)
            m_rules->removeAt(i);
    }

    for (int i = 0; i < newRules.size(); ++i)
        newRules.at(i)->setFilterSubscription(subscription);

    m_rules->append(newRules);
    m_acceptRules.rehash();
    m_blockRules.rehash();;
    save();
}

void NetworkAccessPolicy::subscriptionStateChanged()
{
    m_acceptRules.rehash();
    m_blockRules.rehash();;
    save();
}

void NetworkAccessPolicy::load()
{
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "NetworkAccessPolicy::load";
#endif
    QSettings settings;
    settings.beginGroup(QLatin1String("networkAccessPolicy"));
    m_enabled = settings.value(QLatin1String("enabled"),false).toBool();
    int filterSize = settings.beginReadArray(QLatin1String("subscriptions"));
    m_subscriptions.clear();

    for (int i = 0; i < filterSize; ++i) {
        settings.setArrayIndex(i);
        QString name = settings.value(QLatin1String("name")).toString();
        QString url = settings.value(QLatin1String("url")).toString();
        QDate lastFetch = settings.value(QLatin1String("lastFetch")).toDate();
        bool enabled = settings.value(QLatin1String("enabled"), false).toBool();
        m_subscriptions.append(new FilterSubscription(i, name, url, lastFetch, enabled));
    }
    settings.endArray();

    int size = settings.beginReadArray(QLatin1String("rules"));
    m_rules->clear();

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        //UrlAccessRule *rule = rules.at(i);
        QString pattern = settings.value(QLatin1String("pattern")).toString();

        bool excepRule = settings.value(QLatin1String("exceptionRule")).toBool();
        bool regexp = settings.value(QLatin1String("regexp")).toBool();
        int hitCount = settings.value(QLatin1String("hitCount"), 0).toInt();
        bool enabled = settings.value(QLatin1String("enabled"), true).toBool();
        UrlAccessRule *rule = new UrlAccessRule(!regexp, pattern, excepRule, hitCount, enabled);
        int subIndex = settings.value(QLatin1String("subIndex"), -1).toInt();
#if defined(NETWORKACCESS_DEBUG)
    qDebug()<< "subscription for " << pattern << " is " << subIndex;
#endif
        if (subIndex >= 0)
            rule->setFilterSubscription(m_subscriptions.at(subIndex));
        m_rules->append(rule);
    }
    settings.endArray();

    int version = settings.value(QLatin1String("ruleSetVersion"), 0).toInt();
    if (version < 1) {
        m_subscriptions.clear();
        // initialize
        m_subscriptions.append(new FilterSubscription(0, QLatin1String("EasyList (USA)"),
            QLatin1String("http://adblockplus.mozdev.org/easylist/easylist.txt"), QDate(), false));
        m_subscriptions.append(new FilterSubscription(1, QLatin1String("EasyList Germany"),
            QLatin1String("http://adblockplus.mozdev.org/easylist/ares+easylist.txt"), QDate(), false));
        m_subscriptions.append(new FilterSubscription(2, QLatin1String("Liste FR (France) + EasyList"),
            QLatin1String("http://adblockplus.mozdev.org/easylist/liste_fr+easylist.txt"), QDate(), false));
        m_subscriptions.append(new FilterSubscription(3, QLatin1String("Filter von Dr.Evil (Germany)"),
            QLatin1String("http://maltekraus.de/Firefox/adblock.txt"), QDate(), false));

    }
    settings.endGroup();

    m_acceptRules.rehash();
    m_blockRules.rehash();
}

void NetworkAccessPolicy::save()
{
#if defined(NETWORKACCESS_DEBUG)
    qDebug()<< "NetworkAccessPolicy::save";
#endif
    QSettings settings;
    settings.beginGroup(QLatin1String("networkAccessPolicy"));
    settings.setValue(QLatin1String("enabled"), m_enabled);

    settings.beginWriteArray(QLatin1String("rules"), m_rules->size());
    for (int i = 0; i < m_rules->size(); ++i) {
        settings.setArrayIndex(i);
        UrlAccessRule *rule = m_rules->at(i);
        settings.setValue(QLatin1String("pattern"), rule->pattern());
        settings.setValue(QLatin1String("exceptionRule"),rule->isExceptionRule());
        settings.setValue(QLatin1String("regexp"),rule->isRegexpRule());
        settings.setValue(QLatin1String("hitCount"), rule->hitCount());
        settings.setValue(QLatin1String("enabled"), rule->isEnabled());
        int index = -1;

        FilterSubscription *subscription = rule->filterSubscription();
        if (subscription)
            index = subscription->index();

        settings.setValue(QLatin1String("subIndex"), index);
    }
    settings.endArray();

    settings.beginWriteArray(QLatin1String("subscriptions"), m_subscriptions.size());
    for (int i = 0; i < m_subscriptions.size(); ++i) {
        settings.setArrayIndex(i);
        FilterSubscription *subscription = m_subscriptions.at(i);
        settings.setValue(QLatin1String("name"), subscription->name());
        settings.setValue(QLatin1String("url"), subscription->url());
        settings.setValue(QLatin1String("lastFetch"), subscription->lastFetchDate());
        settings.setValue(QLatin1String("enabled"), subscription->isEnabled());
    }
    settings.endArray();

    settings.setValue(QLatin1String("ruleSetVersion"), 1);

    settings.endGroup();
}
