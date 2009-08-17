/*
 * Copyright 2009 Benjamin C. Meyer <ben@meyerhome.net>
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

#include <QtTest/QtTest>

#include <qbuffer.h>

#include <urlaccessrule.h>
#include <networkaccesspolicy.h>

class tst_AdBlockFilter : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void match_data();
    void match();
};

// Subclass that exposes the protected functions.
class SubAdBlockFilter
{
public:
    SubAdBlockFilter(const QString &filter);
    bool match(const QUrl &url);
    UrlAccessRule *rule;
};

SubAdBlockFilter::SubAdBlockFilter(const QString &filter)
    : rule(0)
{
    QList<UrlAccessRule*> rules;
    QString dummy = "dummy";
    QBuffer buffer(&filter.toUtf8());
    bool ok = NetworkAccessPolicy::importAdBlockRules(buffer,  rules, dummy);
    rule = rules.value(0);
}

bool SubAdBlockFilter::match(const QUrl &url)
{
    if (!rule)
        return false;
    return rule->match(url.toString());
}

// This will be called before the first test function is executed.
// It is only called once.
void tst_AdBlockFilter::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_AdBlockFilter::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_AdBlockFilter::init()
{
}

// This will be called after every test function.
void tst_AdBlockFilter::cleanup()
{
}

void tst_AdBlockFilter::match_data()
{
    QTest::addColumn<QString>("filter");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("match");
    QTest::newRow("null") << QString() << QUrl() << false;

    //Examples taken from http://adblockplus.org/en/filters

    // Basic filter rules
    QTest::newRow("b0") << QString("http://example.com/ads/banner123.gif")
                        << QUrl("http://example.com/ads/banner123.gif")
                        << true;
    QTest::newRow("b1") << QString("http://example.com/ads/banner*.gif")
                        << QUrl("http://example.com/ads/banner123.gif")
                        << true;
    QTest::newRow("b2") << QString("http://example.com/ads/*")
                        << QUrl("http://example.com/ads/banner123.gif")
                        << true;
    QTest::newRow("b3") << QString("http://example.com/*")
                        << QUrl("http://example.com/ads/banner123.gif")
                        << true;

    // defining exception rules
    QTest::newRow("e0") << QString("@@advice")
                        << QUrl("http://example.com/advice.html")
                        << true;
    QTest::newRow("e1") << QString("@@|http://example.com")
                        << QUrl("http://example.com/advice.html")
                        << true;
    QTest::newRow("e2") << QString("@@http://example.com")
                        << QUrl("http://example.com/advice.html")
                        << true;

    // matching at beginning/end of an address
    QTest::newRow("m0") << QString("ad")
                        << QUrl("http://example.com/advice.html")
                        << true;
    QTest::newRow("m1") << QString("*ad*")
                        << QUrl("http://example.com/advice.html")
                        << true;
    QTest::newRow("m2") << QString("swf")
                        << QUrl("http://example.com/swf/index.html")
                        << true;
    QTest::newRow("m3") << QString("swf|")
                    << QUrl("http://example.com/annoyingflash.swf")
                    << true;
    QTest::newRow("m4") << QString("swf|")
                    << QUrl("http://example.com/swf/index.html")
                    << false;

    QTest::newRow("m5") << QString("http://baddomain.example/")
                    << QUrl("http://baddomain.example/banner.gif")
                    << true;
    QTest::newRow("m6") << QString("http://baddomain.example/")
                    << QUrl("http://gooddomain.example/analyze?http://baddomain.example.")
                    << false;

    QTest::newRow("m7") << QString("||example.com/banner.gif")
                    << QUrl("http://example.com/banner.gif")
                    << true;
    QTest::newRow("m8") << QString("||example.com/banner.gif")
                    << QUrl("https://example.com/banner.gif")
                    << true;
    QTest::newRow("m9") << QString("||example.com/banner.gif")
                    << QUrl("http://www.example.com/banner.gif")
                    << true;

    QTest::newRow("m10") << QString("||example.com/banner.gif")
                    << QUrl("http://badexample.com/banner.gif")
                    << false;
    QTest::newRow("m10") << QString("||example.com/banner.gif")
                    << QUrl("http://gooddomain.example/analyze?http://example.com/banner.gif")
                    << false;

    // Marking separate characters
    QTest::newRow("s0") << QString("http://example.com^")
                         << QUrl("http://example.com/")
                         << true;
    QTest::newRow("s1") << QString("http://example.com^")
                         << QUrl("http://example.com:8000/")
                         << true;
    QTest::newRow("s2") << QString("http://example.com^")
                         << QUrl("http://example.com.ar/")
                         << false;
    QTest::newRow("s3") << QString("^example.com^")
                         << QUrl("http://example.com:8000/foo.bar?a=12&b=%D1%82%D0%B5%D1%81%D1%82")
                         << true;
    QTest::newRow("s4") << QString("^%D1%82%D0%B5%D1%81%D1%82^")
                         << QUrl("http://example.com:8000/foo.bar?a=12&b=%D1%82%D0%B5%D1%81%D1%82")
                         << true;
    QTest::newRow("s5") << QString("^foo.bar^")
                         << QUrl("http://example.com:8000/foo.bar?a=12&b=%D1%82%D0%B5%D1%81%D1%82")
                         << true;

    // Comments
    QTest::newRow("c0") << QString("!foo.bar")
                        << QUrl("!foo.bar")
                        << false;
    QTest::newRow("c1") << QString("!foo.bar")
                        << QUrl("foo.bar")
                        << false;

    // Specifying filter options
    // type
    QTest::newRow("o0") << QString("*/ads/*$script,image,background,stylesheet,object,xbl,ping,xmlhttprequest,object-subrequest,object-subrequest,dtd,subdocument,document,other")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    // Inverse type
    QTest::newRow("o1") << QString("*/ads/*$~script, ~image, ~background, ~stylesheet, ~object, ~xbl, ~ping, ~xmlhttprequest, ~object-subrequest, ~dtd, ~subdocument, ~document, ~other")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    // Restriction to third-party/first-party requests
    QTest::newRow("o2") << QString("*/ads/*$third-party")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    QTest::newRow("o3") << QString("*/ads/*$first-party")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    // Domain restrictions
    QTest::newRow("o4") << QString("*/ads/*$domain=example.com|example.net")
                        << QUrl("http://example.com/ads/foo.jpg")
                        << true;
    QTest::newRow("o5") << QString("*/ads/*$domain=example.com")
                        << QUrl("http://foo.com/ads/foo.jpg")
                        << false;

    QTest::newRow("o6") << QString("*/ads/*$domain=~example.com")
                        << QUrl("http://foo.com/ads/foo.jpg")
                        << true;
    QTest::newRow("o7") << QString("*/ads/*$domain=~example.com")
                        << QUrl("http://example.com/ads/foo.jpg")
                        << false;
    QTest::newRow("o8") << QString("*/ads/*$domain=example.com|~foo.example.com")
                        << QUrl("http://example.com/ads/foo.jpg")
                        << true;
    QTest::newRow("o9") << QString("*/ads/*$domain=example.com|~foo.example.com")
                        << QUrl("http://foo.example.com/ads/foo.jpg")
                        << false;
    QTest::newRow("o10") << QString("*/ads/*$domain=example.com|~foo.example.com")
                        << QUrl("http://bar.example.com/ads/foo.jpg")
                        << true;
    // match-case
    QTest::newRow("o11") << QString("*/BannerAd.gif$match-case")
                         << QUrl("http://example.com/BannerAd.gif")
                         << true;
    QTest::newRow("o12") << QString("*/BannerAd.gif$match-case")
                         << QUrl("http://example.com/bannerad.gif")
                         << false;
    // collapse
    // TODO test collapse somehow
    QTest::newRow("o11") << QString("*/BannerAd.gif$collapse")
                         << QUrl("http://example.com/bannerad.gif")
                         << true;
    QTest::newRow("o12") << QString("*/BannerAd.gif$~collapse")
                         << QUrl("http://example.com/bannerad.gif")
                         << true;

    // Regular expressions
    QTest::newRow("r0") << QString("/banner\\d+/")
                         << QUrl("banner123")
                         << true;
    QTest::newRow("r1") << QString("/banner\\d+/")
                         << QUrl("banner321")
                         << true;
    QTest::newRow("r2") << QString("/banner\\d+/")
                         << QUrl("banners")
                         << false;

    // Element hiding
    // TODO
    QTest::newRow("e0") << QString("##div.textad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("##div#sponsorad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("##*#sponsorad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("##textad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("example.com##*.sponsor")
                        << QUrl("example.com")
                        << false;
    QTest::newRow("e1") << QString("example.com,example.net##*.sponsor")
                        << QUrl("example.com")
                        << false;
    // TODO more
    // Attribute selectors
    // Advanced selectors
}

// public bool match(const QUrl &url) const
void tst_AdBlockFilter::match()
{
    QFETCH(QString, filter);
    QFETCH(QUrl, url);
    QFETCH(bool, match);

    SubAdBlockFilter adBlockFilter(filter);

    QCOMPARE(adBlockFilter.match(url), match);
}

QTEST_MAIN(tst_AdBlockFilter)
#include "tst_adblockfilter.moc"

