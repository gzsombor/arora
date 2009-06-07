#ifndef NETWORKEXTENSION_H
#define NETWORKEXTENSION_H

#include <qnetworkreply.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <qiodevice.h>

class NetworkExtension
{
public:
    /**
      * should return a reply object, if it's handled, else it can modify the request for the other handlers.
      */
    virtual QNetworkReply *handleRequest(QNetworkAccessManager::Operation op,
                                         const QNetworkRequest &request,
                                         QIODevice *outgoingData = 0) = 0;

};

Q_DECLARE_INTERFACE(NetworkExtension, "org.arora-browser.NetworkExtension/1.0")


#endif // NETWORKEXTENSION_H
