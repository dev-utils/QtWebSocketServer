#include "WebServer.h"

static QString getIdentifier(QWebSocket *peer)
{
    return QString("%1:%2").arg(peer->peerAddress().toString(),
                                       QString::number(peer->peerPort()));
}

WebServer::WebServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QString("Chat Server"),
                                            QWebSocketServer::NonSecureMode,
                                            this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        QTextStream(stdout) << "Chat Server listening on port " << port << '\n';
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WebServer::onNewConnection);
    }
}

WebServer::~WebServer()
{
    m_pWebSocketServer->close();
}

void WebServer::onNewConnection()
{
    auto pSocket = m_pWebSocketServer->nextPendingConnection();
    QTextStream(stdout) << getIdentifier(pSocket) << " connected!\n";
    pSocket->setParent(this);

    connect(pSocket, &QWebSocket::textMessageReceived,
            this, &WebServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected,
            this, &WebServer::socketDisconnected);

    m_clients << pSocket;
}

void WebServer::processMessage(const QString &message)
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    pSender->sendTextMessage("server got "+ message);
    qDebug()<<"recevier msg:"<<message;
//    for (QWebSocket *pClient : qAsConst(m_clients)) {
//        if (pClient != pSender) { //don't echo message back to sender
//            pClient->sendTextMessage(message);
//        }
//    }
}

void WebServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QTextStream(stdout) << getIdentifier(pClient) << " disconnected!\n";
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
