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

#ifndef FILTERSELECTOR_H
#define FILTERSELECTOR_H

#include "urlaccessrule.h"

#include <qhash.h>
#include <qlist.h>
#include <qobject.h>

class FilterSelector : public QObject
{
    Q_OBJECT

public:
    FilterSelector(bool exceptionRules, QObject *parent = 0);

    const UrlAccessRule *get(QString url) const;

    void setRules(QList<UrlAccessRule*> *rules);

public slots:
    void rehash();

private:
    bool m_exceptionRules;
    QList<UrlAccessRule*> *m_allRules;
    QHash<QString,UrlAccessRule*> m_ruleHash;
    QList<UrlAccessRule*> m_extraRules;
};

#endif // FILTERSELECTOR_H
