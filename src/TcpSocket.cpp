#include "TcpSocket.h"

Socket::Socket(SOCKET sock)
{
    this->sock = sock;
}

BOOL Socket::send(LPBYTE buffer, UINT buffSize)
{
	return (::send(sock, (LPCSTR)buffer, buffSize, 0) != SOCKET_ERROR);

}

int Socket::recv(LPBYTE buffer, UINT buffSize)
{
	return ::recv(sock, (LPSTR)buffer, buffSize, 0);

}

void Socket::close() 
{
    closesocket(sock);
    sock = INVALID_SOCKET;
}

BOOL Socket::initClass()
{
	WSADATA WSAData;

	return (WSAStartup(MAKEWORD(2,0), &WSAData) == 0);

}

///////////////////////

BOOL TcpSocketServer::listen(Container* pCtrl, UINT port, UINT maxPendingConnection)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        return FALSE;

	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (PSOCKADDR) &sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
        close();
		return FALSE;
	}

	if (::listen(sock, maxPendingConnection) < 0)
	{
		close();
		return FALSE;
	}

    WSAAsyncSelect(sock, pCtrl->getHandle(), WSA_EVENT, FD_ACCEPT);

    return TRUE;
}


///////////////////////////:

BOOL TcpSocketClient::connect(Container* pCtrl, LPSTR strHostName, UINT portNumber)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        return FALSE;

	SOCKADDR_IN sockaddr;
	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(portNumber);

	ULONG ipAddr = inet_addr(strHostName);

	if (ipAddr == INADDR_NONE)
	{
		// try with hostname
		PHOSTENT phe;
		phe = gethostbyname(strHostName);

		if (phe == NULL)
		{
			close();
			return FALSE;
		}

		memcpy((char*)&(sockaddr.sin_addr), phe->h_addr, phe->h_length);
	}
	else
		sockaddr.sin_addr.s_addr = ipAddr;


	if (::connect(sock, (PSOCKADDR) &sockaddr, sizeof(sockaddr)) != 0)
	{
		close();
		return FALSE;
	}

    WSAAsyncSelect(sock, pCtrl->getHandle(), WSA_EVENT, FD_READ | FD_CLOSE);

	return TRUE;

}

