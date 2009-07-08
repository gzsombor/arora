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

#include "blockednetworkreply.h"

#include <qtimer.h>

BlockedNetworkReply::BlockedNetworkReply(QNetworkAccessManager::Operation op, const QNetworkRequest &request)
{
    setUrl(request.url());
    setRequest(request);
    setOperation(op);

    QTimer::singleShot(200, this, SLOT(notifyAboutFailure()));
}

void BlockedNetworkReply::abort()
{
}

qint64 BlockedNetworkReply::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return -1;
}

void BlockedNetworkReply::notifyAboutFailure()
{
    setError(QNetworkReply::UnknownContentError, tr("Blocked by filter"));
    emit error(QNetworkReply::UnknownContentError);
    emit finished();
}
