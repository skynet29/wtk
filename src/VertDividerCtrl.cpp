#include "VertDividerCtrl.h"
#include "Panel.h"

#define SIZEPANELHEIGHT 5


class VertSizePanel : public Panel {
public:
    VertSizePanel();
protected:
    void onLButtonDown(Point pt);
    void onLButtonUp(Point pt);
    void onMouseMove(Point pt);
    void drawLine(Point pt);

    Graphic* pGraph;
    Point p1, p2;        

};

VertSizePanel::VertSizePanel() {
    pGraph = NULL;
    setBackColor(Color::getSysColor());
    setCursor(Cursor::K_SIZENS);
}

void VertSizePanel::drawLine(Point pt) {
    Size sz = getClientSize();
    p1 = Point(0, pt.y);
    clientToScreen(p1);
    p2 = Point(sz.width, pt.y);
    clientToScreen(p2);
    pGraph->drawLine(p1, p2);
}


void VertSizePanel::onLButtonDown(Point pt) {
    debugPrint("onLeftButtonDown");
    
    pGraph = new Graphic(GetWindowDC(GetDesktopWindow()));
    pGraph->setDrawMode(Graphic::K_NOT);
    drawLine(pt);

    ((VertDividerCtrl*)getParent())->clipCursor();
            
}
void VertSizePanel::onLButtonUp(Point pt) {

    if (pGraph != NULL) {
        pGraph->drawLine(p1, p2);
        ReleaseDC(GetDesktopWindow(), pGraph->getHandle());
        delete pGraph;
        pGraph = NULL;
        ClipCursor(NULL);
        ((VertDividerCtrl*)getParent())->updateHeight(p1);
    }
}

void VertSizePanel::onMouseMove(Point pt) {

    if (pGraph != NULL) {
        pGraph->drawLine(p1, p2);
        drawLine(pt);      
    }
}

VertDividerCtrl::VertDividerCtrl(Window* pTopCtrl, Window* pBottomCtrl, UINT bottomCtrlHeight)
{
    debugPrint("VertDividerCtrl\n");

    this->pTopCtrl = pTopCtrl;
    this->pBottomCtrl = pBottomCtrl;
    this->bottomCtrlHeight = bottomCtrlHeight;

    if (pTopCtrl != NULL)
        addChild(pTopCtrl);
    if (pBottomCtrl != NULL)
        addChild(pBottomCtrl);

    addChild(pSizePanel = new VertSizePanel());
}

void VertDividerCtrl::onSize(UINT width, UINT height)
{
	if (pBottomCtrl)
		pBottomCtrl->setBounds(Bounds(0, height - bottomCtrlHeight, width, bottomCtrlHeight));

	pSizePanel->setBounds(Bounds(0, height - bottomCtrlHeight - SIZEPANELHEIGHT, width, SIZEPANELHEIGHT));
	
	if (pTopCtrl)
		pTopCtrl->setBounds(Bounds(0, 0, width, height - bottomCtrlHeight - SIZEPANELHEIGHT));
}

void VertDividerCtrl::updateHeight(Point p1) 
{
    Size sz = getClientSize();
    screenToClient(p1);
    bottomCtrlHeight = sz.height - p1.y;
    onSize(sz.width, sz.height);
}

void VertDividerCtrl::clipCursor()
{
    Point leftTop = Point(0, 0);
    clientToScreen(leftTop);
    Size sz = getClientSize();
    Point rightBottom = Point(sz.width, sz.height);
    clientToScreen(rightBottom);

    Rect clipRect = Rect(leftTop, rightBottom);
    clipRect.bottom -= 50;
    clipRect.top += 50;
    ClipCursor(&clipRect);      
}