#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "Container.h"

class DllExport Socket {
public:
    Socket(SOCKET sock = INVALID_SOCKET);

    BOOL send(LPBYTE buffer, UINT buffSize);
    int recv(LPBYTE buffer, UINT buffSize);
    void close();

    static BOOL initClass();

protected:
    SOCKET sock;

};

class DllExport TcpSocketClient : public Socket {
public:
    BOOL connect(Container* pCtrl, LPSTR strHostName, UINT portNumber);

private:
    Container* pCtrl;
};

class DllExport TcpSocketServer : public Socket {
public:
    BOOL listen(Container* pCtrl, UINT port, UINT maxPendingConnection = 5);
};


#endif