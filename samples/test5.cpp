

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "TextField.h"
#include "Button.h"
#include "Layout.h"
#include "TcpSocket.h"
#include "TextArea.h"

class MyFrame : public Frame {
private:
    TextField* txtPort;
    TextField* txtHost;
    Button* btnConnect;
    Button* btnClose;
    TcpSocketClient client;
    TextField* txtMsg;
    Button* btnSend;
    TextArea* txtRecvMsg;


public:

    MyFrame() : Frame("Test 5") {

        setResizable(FALSE);
        
        txtPort = new TextField(ES_NUMBER);
        txtHost = new TextField();
        btnConnect = new Button("Connect");
        btnConnect->setOnClick(CBK(MyFrame, btnConnect_onClick));
        btnClose = new Button("Close");
        btnClose->setOnClick(CBK(MyFrame, btnClose_onClick));

        txtMsg = new TextField();
        btnSend = new Button("Send");
        btnSend->setOnClick(CBK(MyFrame, btnSend_onClick));

        txtRecvMsg = new TextArea();

        txtPort->setText("60280");
        txtHost->setText("localhost");

        Layout layout(this, 10, 10);
        layout.addLabel("Port", 0, 25);
        layout.add(txtPort, Size(50, 25), 5);
        layout.addLabel("HostName", 0, 25, 10);
        layout.add(txtHost, Size(100, 25), 5);
        layout.add(btnConnect, Size(80, 25), 10);
        layout.add(btnClose, Size(80, 25), 10);
        layout.endl();
        layout.addLabel("Msg", 0, 25);
        layout.add(txtMsg, Size(100, 25), 5);
        layout.add(btnSend, Size(80, 25), 10);
        layout.endl();
        layout.addEnd(txtRecvMsg, 100);
        
        updateState(FALSE);
    }

    void updateState(BOOL isConnected)  {
        btnConnect->setEnabled(!isConnected);
        btnClose->setEnabled(isConnected);
        btnSend->setEnabled(isConnected);
    }
protected:
    void onCreate() {
        Frame::onCreate();

        packSize(10, 10);
    }

    void onDataReceived(SOCKET sock) {
        BYTE buffer[1024];
        int byteRead = client.recv(buffer, sizeof(buffer));
        debugPrint("byteRead=%d", byteRead);
        buffer[byteRead] = 0;
        debugPrint("Msg=%s", buffer);
        txtRecvMsg->append((char*)buffer);
        txtRecvMsg->append("\r\n");
        txtRecvMsg->scrollToEnd();

    }

    void onConnectionClosed(SOCKET sock) {
        debugPrint("connection closed!");
        updateState(FALSE);
    }

    void btnConnect_onClick(void* from) 
    {
        StrBuffer host;
        txtHost->getText(host);
        StrBuffer port;
        txtPort->getText(port);
        if (!client.connect(this, host.getBuffer(), port.toInt())) {
            showMsg("Connection fail!");
        }
        else {
            updateState(TRUE);
        }
    }

    void btnClose_onClick(void* from) 
    {
        client.close();
        updateState(FALSE);
    }
    
    void btnSend_onClick(void* from) 
    {
        StrBuffer msg;
        txtMsg->getText(msg);
        client.send((LPBYTE)msg.getBuffer(), msg.getLength());
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
