#ifndef DEMOEXTENSION_H
#define DEMOEXTENSION_H

#include "aroraapi.h"

#include "qhash.h"

class WindowData;

class DemoExtension : public QObject, AroraExtension, WindowExtension
{
    Q_OBJECT
    Q_INTERFACES(AroraExtension)
    Q_INTERFACES(WindowExtension)

public:
    DemoExtension();


    ~DemoExtension();

    /**
      * return a unique id for this plugin, this is used by the manager to
      * identifie the extension
      */
    const QString id() const;

    /**
      * Human readable/translated string, this will show up on the plugin
      * manager dialog.
      */
    const QString name() const;

    /**
      * this method called before the extension is activated, should return true
      * if succeeded.
      */
    bool activate(PluginApi *api);

    /**
      * this method called before the extension is deactivated
      */
    void deactivate();

    /**
      * this method called after a new application window is created.
      */
    void newWindow(AroraWindow *window);

    /**
      * this method called before a new application window is closed.
      */
    void closeWindow(AroraWindow *window);

    /**
      * this method called when language is changed, or during the initialization phase
      * and strings should be (re)translated.
      */
    void localize(AroraWindow *window);

    void configure() {}


private:
    PluginApi *m_api;
    QHash<AroraWindow*,WindowData*> m_windowDatas;
    WindowData *getData(AroraWindow *window);
};


class WindowData : public QObject
{
    Q_OBJECT

public:
    WindowData(PluginApi *api, AroraWindow *window);
    ~WindowData();

    void setup(QMenu *extensionMenu);
    void localize();

private slots:

    void showCookies();
    void showExceptions();

private:
    QAction *m_toolsCookiesAction;
    QAction *m_toolsCookieExceptionAction;
    QMenu *m_extensionMenu;
    AroraWindow *m_window;
    PluginApi *m_api;
};


#endif // DEMOEXTENSION_H
