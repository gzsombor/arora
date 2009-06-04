#ifndef ARORAEDITLISTVIEW_H
#define ARORAEDITLISTVIEW_H

#include "qwidget.h"

#include "editlistview.h"
#include "pluginapi.h"


class PLUGINAPI_EXPORT AroraEditListView : public EditListView
{
    Q_OBJECT
public:
    PLUGINAPI_EXPORT AroraEditListView(QWidget *parent = 0);

};

#endif // ARORAEDITLISTVIEW_H
