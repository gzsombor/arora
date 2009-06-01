#include "demoextension.h"

#include "qaction.h"



DemoExtension::DemoExtension()
{
}

DemoExtension::~DemoExtension()
{

}

const QString DemoExtension::id() const
{
    return "demoextension";
}

const QString DemoExtension::name() const
{
    return "Simple Extension which add some menu items ...";
}

bool DemoExtension::init(PluginApi *api) {
    this->api = api;
    return true;
}

void DemoExtension::close() {
    foreach(WindowData* windowData, m_windowDatas) {
        delete windowData;
    }
}

void DemoExtension::newWindow(BrowserMainWindow *window, QMenu *extensionMenu)
{
    WindowData* wd = getData(window);
    wd->setup(extensionMenu);
}

void DemoExtension::closeWindow(BrowserMainWindow *window)
{
    WindowData *wd = this->m_windowDatas.take(window);
    if (wd) {
        delete wd;
    }
}

void DemoExtension::localize(BrowserMainWindow *window)
{
    WindowData *wd = getData(window);
    wd->localize();
}

WindowData* DemoExtension::getData(BrowserMainWindow* window)
{
    WindowData *wd = this->m_windowDatas.value(window);
    if (wd == NULL) {
        wd = new WindowData(this->api, window);
        this->m_windowDatas.insert(window, wd);
    }
    return wd;
}

WindowData::WindowData(PluginApi *api, BrowserMainWindow* window) : m_toolsCookiesAction(0), m_toolsCookieExceptionAction (0)
{
    this->api = api;
    this->extensionMenu = 0;
    this->window = window;
}

WindowData::~WindowData()
{
    if (extensionMenu) {
        this->extensionMenu->removeAction(this->m_toolsCookieExceptionAction);
        this->extensionMenu->removeAction(this->m_toolsCookiesAction);
        delete this->m_toolsCookiesAction;
        delete this->m_toolsCookieExceptionAction;
    }
}

void WindowData::setup(QMenu *extensionMenu)
{
    this->extensionMenu = extensionMenu;
    this->m_toolsCookiesAction = new QAction(extensionMenu);
    QObject::connect(this->m_toolsCookiesAction, SIGNAL(triggered()), this, SLOT(showCookies()));
    this->extensionMenu->addAction(this->m_toolsCookiesAction);

    this->m_toolsCookieExceptionAction = new QAction(extensionMenu);
    QObject::connect(this->m_toolsCookieExceptionAction, SIGNAL(triggered()), this, SLOT(showExceptions()));
    this->extensionMenu->addAction(this->m_toolsCookieExceptionAction);
}

void WindowData::localize()
{
    this->m_toolsCookiesAction->setText(tr("Cookies..."));
    this->m_toolsCookieExceptionAction->setText(tr("Cookie &Exceptions..."));
}

void WindowData::showCookies()
{
    this->api->showCookiesDialog(this->window);
}

void WindowData::showExceptions()
{
    this->api->showCookieExceptionsDialog(this->window);
}

Q_EXPORT_PLUGIN2(AroraExtension, DemoExtension)

