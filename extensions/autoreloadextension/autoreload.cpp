#include "autoreload.h"

#include "qaction.h"



AutoReloadExtension::AutoReloadExtension()
{
}

AutoReloadExtension::~AutoReloadExtension()
{

}

const QString AutoReloadExtension::id() const
{
    return "autoreload";
}

const QString AutoReloadExtension::name() const
{
    return "Simple Extension which provides user configurable auto-reloads";
}

bool AutoReloadExtension::activate(PluginApi *api) {
    this->m_api = api;
    return true;
}

void AutoReloadExtension::deactivate() {
    /*foreach (WindowData *windowData, m_windowDatas) {
        delete windowData;
    }
    m_windowDatas.clear();*/
}

void AutoReloadExtension::newWindow(AroraWindow *window)
{
    QAction *reloadAction = window->action(AroraWindow::ReloadStop);

    QMenu *myMenu = new QMenu(reloadAction->parentWidget());
    QAction *enabledItem = new QAction(myMenu);
    enabledItem->setCheckable(true);

    myMenu->addAction(enabledItem);
    reloadAction->setMenu(myMenu);

    window->set(QLatin1String("autoreload_menu"), myMenu);
    window->set(QLatin1String("autoreload_enabledAction"), enabledItem);

}

void AutoReloadExtension::closeWindow(AroraWindow *window)
{
    QObject *act = window->remove(QLatin1String("autoreload_enabledAction"));
    QObject *menu = window->remove(QLatin1String("autoreload_menu"));

    QAction *reloadAction = window->action(AroraWindow::ReloadStop);
    reloadAction->setMenu(0);

    if (act) {
        delete act;
    }

    if (menu) {
        delete menu;
    }
}

void AutoReloadExtension::localize(AroraWindow *window)
{
    QObject *act = window->get(QLatin1String("autoreload_enabledAction"));
    if (act) {
        QAction *action = qobject_cast<QAction*>(act);
        action->setText(tr("Enable..."));
        action->setToolTip(tr("Enable auto reload"));
    }
}


Q_EXPORT_PLUGIN2(AroraExtension, AutoReloadExtension)

