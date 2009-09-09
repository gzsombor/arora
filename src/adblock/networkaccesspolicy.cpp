/*
 * Copyright 2009 Zsombor Gegesy <gzsombor@gmail.com>
 * Copyright 2009 Benjamin Meyer <ben@meyerhome.net>
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

#include "adblocksubscription.h"
#include "browserapplication.h"

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
    QObject(parent), m_acceptRules(true, this), m_blockRules(false, this), m_autoSaver(this)
{
    m_rules = new QList<AdBlockRule*>();
    m_acceptRules.setRules(m_rules);
    m_blockRules.setRules(m_rules);
#if defined(NETWORKACCESS_DEBUG)
    m_elapsedTime = 0;
#endif
    load();
}

NetworkAccessPolicy::~NetworkAccessPolicy()
{
    m_autoSaver.saveIfNeccessary();
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

    QString urlString = url.toString();
    const AdBlockRule *rule = m_acceptRules.get(urlString);
    if (rule) {
        const_cast<AdBlockRule*>(rule)->incrementHitCount();
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "access to " << urlString << " matched"
                    << rule->toString() << " --> ALLOW ";
#endif
        m_autoSaver.changeOccurred();
        return true;
    }
    rule = m_blockRules.get(urlString);
    if (rule) {
        const_cast<AdBlockRule*>(rule)->incrementHitCount();
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "access to " << urlString << " matched"
                    << rule->toString() << " --> BLOCK,hash:" << rule->hash();
#endif
        m_autoSaver.changeOccurred();
        return false;
    }
    return true;
}

bool NetworkAccessPolicy::importAdBlockRulesFromFile(QFile &importFrom,  QList<AdBlockRule*> &rules)
{
    if (!importFrom.exists()) {
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << QLatin1String("Data file not found : ") << importFrom.fileName();
#endif
        return false;
    }
    return NetworkAccessPolicy::importAdBlockRules(importFrom, rules, importFrom.fileName());
}

bool NetworkAccessPolicy::importAdBlockRules(QIODevice &importFrom,  QList<AdBlockRule*> &rules, const QString &name)
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

bool NetworkAccessPolicy::importAdBlockRules(QTextStream &txt,  QList<AdBlockRule*> &rules)
{
    QString header = txt.readLine(1024);
    if (!header.startsWith(QLatin1String("[Adblock"))) {
        return false;
    }
    QString line;
    rules.clear();
    do {
        line = txt.readLine();
        AdBlockRule *rule = AdBlockRule::parse(line);
        if (rule) {
#if defined(NETWORKACCESS_DEBUG)
            qDebug() << rule->toString();
#endif
            rules.append(rule);
        }
    } while (!txt.atEnd());

    return true;
}

void NetworkAccessPolicy::exportSettings(QList<AdBlockRule*> &rules, QFile &fileToExport)
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


const QList<AdBlockRule*> *NetworkAccessPolicy::accessRules() const
{
    return m_rules;
}

void NetworkAccessPolicy::setAccessRules(QList<AdBlockRule*> &newRules)
{
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "NetworkAccessPolicy::" << __FUNCTION__ << newRules.size();
#endif
    m_rules->clear();
    m_rules->append(newRules);
    m_acceptRules.rehash();
    m_blockRules.rehash();
    save();
}

void NetworkAccessPolicy::setAccessRules(AdBlockSubscription *subscription, QList<AdBlockRule*> &newRules)
{
#if defined(NETWORKACCESS_DEBUG)
        qDebug() << "setAccessRules for : " << subscription->name() << " len : " << newRules.size();
#endif

    for (int i= m_rules->size() - 1; i >= 0; --i) {
        AdBlockRule *rule = m_rules->at(i);
        if (rule->subscription() == subscription)
            m_rules->removeAt(i);
    }

    for (int i = 0; i < newRules.size(); ++i)
        newRules.at(i)->setAdBlockSubscription(subscription);

    m_rules->append(newRules);
    m_acceptRules.rehash();
    m_blockRules.rehash();
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
    qDebug() << "NetworkAccessPolicy" << __FUNCTION__;
#endif
    QSettings settings;
    settings.beginGroup(QLatin1String("networkAccessPolicy"));
    m_enabled = settings.value(QLatin1String("enabled"), false).toBool();
    m_subscriptions.clear();

    if (!settings.contains(QLatin1String("subscriptions"))) {
        // initialize

        AdBlockSubscription *d1 = new AdBlockSubscription;
        d1->setName(QLatin1String("EasyList (USA)"));
        d1->setUrl(QString(QLatin1String("http://adblockplus.mozdev.org/easylist/easylist.txt")));
        d1->setPriority(0);

        AdBlockSubscription *d2 = new AdBlockSubscription;
        d2->setName(QLatin1String("EasyList Germany"));
        d2->setUrl(QString(QLatin1String("http://adblockplus.mozdev.org/easylist/ares+easylist.txt")));
        d2->setPriority(1);

        AdBlockSubscription *d3 = new AdBlockSubscription;
        d3->setName(QLatin1String("Liste FR (France) + EasyList"));
        d3->setUrl(QString(QLatin1String("http://adblockplus.mozdev.org/easylist/liste_fr+easylist.txt")));
        d3->setPriority(2);

        AdBlockSubscription *d4 = new AdBlockSubscription;
        d4->setName(QLatin1String("Filter von Dr.Evil (Germany)"));
        d4->setPriority(3);
        d4->setUrl(QString(QLatin1String("http://maltekraus.de/Firefox/adblock.txt")));
        m_subscriptions.append(d1);
        m_subscriptions.append(d2);
        m_subscriptions.append(d3);
        m_subscriptions.append(d4);
    } else {
        int filterSize = settings.beginReadArray(QLatin1String("subscriptions"));
        m_subscriptions.clear();

        for (int i = 0; i < filterSize; ++i) {
            settings.setArrayIndex(i);
            QString name = settings.value(QLatin1String("name")).toString();
            QString url = settings.value(QLatin1String("url")).toString();
            QDate lastFetch = settings.value(QLatin1String("lastFetch")).toDate();
            bool enabled = settings.value(QLatin1String("enabled"), false).toBool();
            m_subscriptions.append(new AdBlockSubscription(i, name, url, lastFetch, enabled));
        }
        settings.endArray();
    }

    m_rules->clear();

    QFile adblockRuleFile(BrowserApplication::dataFilePath(QLatin1String("adblock-rules")));
    if (adblockRuleFile.exists()) {
        if (adblockRuleFile.open(QIODevice::ReadOnly)) {
            QDataStream in(&adblockRuleFile);
            QString header;
            in >> header;
            if (QLatin1String("arora-adblock-rules") == header) {
                int version;
                in >> version;
                if (version == 1) {
                    int length;
                    in >> length;
                    for (int i = 0; i < length; ++i) {
                        AdBlockRule *rule = new AdBlockRule();
                        rule->load(in);
                        int priority;
                        in >> priority;
                        if (priority>=0 && priority < m_subscriptions.size()) {
                            rule->setAdBlockSubscription(m_subscriptions.at(priority));
                        }
                        m_rules->append(rule);
                    }
                }
            }
        }
    }



//    int size = settings.beginReadArray(QLatin1String("rules"));
//    m_rules->clear();
//
//    for (int i = 0; i < size; ++i) {
//        settings.setArrayIndex(i);
//        //AdBlockRule *rule = rules.at(i);
//        QString pattern = settings.value(QLatin1String("pattern")).toString();
//
//        bool excepRule = settings.value(QLatin1String("exceptionRule")).toBool();
//        bool regexp = settings.value(QLatin1String("regexp")).toBool();
//        int hitCount = settings.value(QLatin1String("hitCount"), 0).toInt();
//        bool enabled = settings.value(QLatin1String("enabled"), true).toBool();
//        AdBlockRule *rule = new AdBlockRule(regexp, pattern, excepRule, hitCount, enabled);
//        int subIndex = settings.value(QLatin1String("subIndex"), -1).toInt();
//#if defined(NETWORKACCESS_DEBUG)
//    qDebug()<< "subscription for " << pattern << " is " << subIndex;
//#endif
//        if (subIndex >= 0)
//            rule->setAdBlockSubscription(m_subscriptions.at(subIndex));
//        m_rules->append(rule);
//    }
//    settings.endArray();
    settings.endGroup();

    m_acceptRules.rehash();
    m_blockRules.rehash();
}

void NetworkAccessPolicy::save()
{
#if defined(NETWORKACCESS_DEBUG)
    qDebug() << "NetworkAccessPolicy" << __FUNCTION__;
#endif
    QSettings settings;
    settings.beginGroup(QLatin1String("networkAccessPolicy"));
    settings.setValue(QLatin1String("enabled"), m_enabled);

/*    settings.beginWriteArray(QLatin1String("rules"), m_rules->size());
    for (int i = 0; i < m_rules->size(); ++i) {
        settings.setArrayIndex(i);
        AdBlockRule *rule = m_rules->at(i);
        settings.setValue(QLatin1String("pattern"), rule->pattern());
        settings.setValue(QLatin1String("exceptionRule"),rule->isExceptionRule());
        settings.setValue(QLatin1String("regexp"),rule->isRegexpRule());
        settings.setValue(QLatin1String("hitCount"), rule->hitCount());
        settings.setValue(QLatin1String("enabled"), rule->isEnabled());
        int index = -1;

        AdBlockSubscription *subscription = rule->subscription();
        if (subscription)
            index = subscription->priority();

        settings.setValue(QLatin1String("subIndex"), index);
    }
    settings.endArray();
    */

    QList<AdBlockSubscription> subscriptions;
    foreach (const AdBlockSubscription *subscription, m_subscriptions)
        subscriptions.append(AdBlockSubscription(*subscription));
    QVariant v = qVariantFromValue(subscriptions);
    settings.setValue(QLatin1String("subscriptions"), v);
    settings.endGroup();

    QFile adblockRuleFile(BrowserApplication::dataFilePath(QLatin1String("adblock-rules")));
    if (adblockRuleFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&adblockRuleFile);
        out << QString(QLatin1String("arora-adblock-rules"));
        out << 1; // version
        out << m_rules->size();
        for (int i = 0; i < m_rules->size(); ++i) {
            AdBlockRule *rule = m_rules->at(i);
            rule->save(out);
            int priority = rule->subscription() ? rule->subscription()->priority() : -1;
            out << priority;
        }
        adblockRuleFile.close();
    }
}
