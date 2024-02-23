
#include <windows.h>

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

    enum {
        ID_BTNCONNECT = 100,
        ID_BTNCLOSE,
        ID_BTNSEND,
        ID_TXTPORT,
        ID_TXTHOST,
        ID_TEXTMSG
    };
public:

    MyFrame() : Frame("Test 5") {

        setResizable(FALSE);
        
        txtPort = new TextField(ID_TXTPORT, ES_NUMBER);
        txtHost = new TextField(ID_TXTHOST);
        btnConnect = new Button("Connect", ID_BTNCONNECT);
        btnClose = new Button("Close", ID_BTNCLOSE);
        txtMsg = new TextField(ID_TEXTMSG);
        btnSend = new Button("Send", ID_BTNSEND);
        txtRecvMsg = new TextArea();

        txtPort->setText("60280");
        txtHost->setText("localhost");

        Layout layout(this, 10, 10);
        layout.addLabel("Port", 0, 25);
        layout.add(txtPort, Size(50, 25));
        layout.addLabel("HostName", 0, 25);
        layout.add(txtHost, Size(100, 25), 30);
        layout.add(btnConnect, Size(80, 25));
        layout.add(btnClose, Size(80, 25));

        Layout layout2(this, 10, 50);
        layout2.addLabel("Msg", 0, 25);
        layout2.add(txtMsg, Size(100, 25));
        layout2.add(btnSend, Size(80, 25));

        addChild(txtRecvMsg, Bounds(10, 90, 200, 100));
        
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

    void onCommand(UINT id) {
        switch(id) {
            case ID_BTNCONNECT:
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
                break;
            
            case ID_BTNCLOSE:
                client.close();
                updateState(FALSE);
                break;

            case ID_BTNSEND:
                {
                    StrBuffer msg;
                    txtMsg->getText(msg);
                    client.send((LPBYTE)msg.getBuffer(), msg.getLength());
                }
                break;
        }
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
