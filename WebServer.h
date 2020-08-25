#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <QWebSocketServer>
#include <QWebSocket>

class WebServer : public QObject
{
    Q_OBJECT
public:
    WebServer(quint16 port, QObject *parent = nullptr);
    ~WebServer();

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

#endif // WEBSERVER_H
