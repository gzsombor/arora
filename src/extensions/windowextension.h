#ifndef WINDOWEXTENSION_H
#define WINDOWEXTENSION_H

#include "aroraextension.h"
#include "pluginapi.h"

class WindowExtension
{

public:

    /**
      * this method called after a new application window is created.
      */
    virtual void newWindow(BrowserMainWindow *window, QMenu *extensionMenu) = 0;

    /**
      * this method called before a new application window is closed.
      */
    virtual void closeWindow(BrowserMainWindow *window) = 0;

    /**
      * this method called when language is changed, or during the initialization phase
      * and strings should be (re)translated.
      */
    virtual void localize(BrowserMainWindow *window) = 0;

};

Q_DECLARE_INTERFACE(WindowExtension, "org.arora-browser.WindowExtension/1.0")


#endif // WINDOWEXTENSION_H
