

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "TcpSocket.h"
#include "TextField.h"
#include "Button.h"
#include "Layout.h"
#include "TextArea.h"

class MyFrame : public Frame {
private:
    TcpSocketServer server;
    Vector<SOCKET> clients;
    TextField* txtPort;
    Button* btnListen;
    TextArea* txtInfo;
public:

    MyFrame() : Frame("Test 6") {
        Layout layout(this, 10, 10);
        layout.addLabel("Port", 30, 25);
        layout.add(txtPort = new TextField(ES_NUMBER), Size(50, 25), 5);
        layout.add(btnListen = new Button("Listen"), Size(50, 25), 10);
        btnListen->setOnClick(CBK(MyFrame, btnListen_onClick));
        layout.endl();
        layout.add(txtInfo = new TextArea(), Size(300, 300));

        txtPort->setText("60280");

    }

    void broadcast(LPSTR msg) {
        for(UINT i = 0; i < clients.getCount(); i++) {
            Socket(clients[i]).send((LPBYTE)msg, strlen(msg));
        }
    }

protected:
    void btnListen_onClick(void *from) 
    {
        StrBuffer port;
        txtPort->getText(port);
        server.listen(this, port.toInt());
        startTimer(0, 5000);
    }

    void onCreate() {
        Frame::onCreate();

        packSize(10, 10);

        //server.listen(this, 60280);

        
    }

    void onIncomingConnection(SOCKET srvSock, SOCKET sock) {
        debugPrint("onIncomingConnection");
        StrBuffer text;
        text.format("onIncomingConnection #%d\r\n", sock);
        txtInfo->append(text.getBuffer());
        clients.add(sock);
    }

    void onDataReceived(SOCKET sock) {
        debugPrint("onDataReceived");
        BYTE buffer[1024];
        int byteRead = Socket(sock).recv(buffer, sizeof(buffer));
        debugPrint("byteRead=%d\n", byteRead);
        buffer[byteRead] = 0;
        debugPrint("Msg=%s\n", buffer);

        StrBuffer text;
        text.format("onDataReceived #%d: %s\r\n", sock, buffer);
        txtInfo->append(text.getBuffer());


    }

    void onConnectionClosed(SOCKET sock) {
        debugPrint("connection closed!");
        StrBuffer text;
        text.format("onConnectionClosed #%d\r\n", sock);
        txtInfo->append(text.getBuffer());
        clients.remove(sock);
    }

    void onTimer(UINT id) {
        static UINT nbMsg = 0;
         debugPrint("onTimer %d", id);
         StrBuffer text;
         text.format("Hello #%d", nbMsg++);
         broadcast(text.getBuffer());
    }

};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{ 
    Application app;
    MyFrame frame;

    return app.run(frame);
}
