#include "HorzDividerCtrl.h"
#include "Panel.h"
#include "Cursor.h"

#define SIZEPANELWIDTH 5


class HorzSizePanel : public Panel {
public:
    HorzSizePanel();
protected:
    void onLButtonDown(Point pt);
    void onLButtonUp(Point pt);
    void onMouseMove(Point pt);
    void drawLine(Point pt);

    Graphic* pGraph;
    Point p1, p2;        

};

HorzSizePanel::HorzSizePanel() {
    pGraph = NULL;
    setBackColor(Color::getSysColor());
    setCursor(Cursor::K_SIZEWE);
}
void HorzSizePanel::onLButtonDown(Point pt) {
    //debugPrint("onLeftButtonDown");
    
    pGraph = new Graphic(GetWindowDC(GetDesktopWindow()));
    pGraph->setDrawMode(DrawMode::NOT);
    drawLine(pt);

    ((HorzDividerCtrl*)getParent())->clipCursor();
            
}

void HorzSizePanel::drawLine(Point pt) {
    Size sz = getClientSize();
    p1 = Point(pt.x, 0);
    clientToScreen(p1);
    p2 = Point(pt.x, sz.height);
    clientToScreen(p2);
    pGraph->drawLine(p1, p2);
}

void HorzSizePanel::onLButtonUp(Point pt) {
    //debugPrint("onLeftButtonUp");

    if (pGraph != NULL) {
        pGraph->drawLine(p1, p2);
        ReleaseDC(GetDesktopWindow(), pGraph->getHandle());
        delete pGraph;
        pGraph = NULL;
        ClipCursor(NULL);
        ((HorzDividerCtrl*)getParent())->updateWidth(p1);
    }
}

void HorzSizePanel::onMouseMove(Point pt) {

    if (pGraph != NULL) {
        //debugPrint("onMouseMove (%d, %d)\n", pt.x, pt.y);

        pGraph->drawLine(p1, p2);

        drawLine(pt);         
    }
}

HorzDividerCtrl::HorzDividerCtrl(Window* pRightCtrl, Window* pLeftCtrl, UINT leftCtrlWidth)
{
    this->pRightCtrl = pRightCtrl;
    this->pLeftCtrl = pLeftCtrl;
    this->leftCtrlWidth = leftCtrlWidth;

    if (pRightCtrl != NULL)
        addChild(pRightCtrl);
    if (pLeftCtrl != NULL)
        addChild(pLeftCtrl);

    addChild(pSizePanel = new HorzSizePanel());
}

void HorzDividerCtrl::onSize(UINT width, UINT height)
{
	if (pLeftCtrl != NULL)
		pLeftCtrl->setBounds(Bounds(0, 0, leftCtrlWidth, height));

	pSizePanel->setBounds(Bounds(leftCtrlWidth, 0, SIZEPANELWIDTH, height));
	
	if (pRightCtrl != NULL)
		pRightCtrl->setBounds(Bounds(leftCtrlWidth + SIZEPANELWIDTH, 0, width - leftCtrlWidth - SIZEPANELWIDTH, height));
}

void HorzDividerCtrl::updateWidth(Point p1) 
{
    screenToClient(p1);
    leftCtrlWidth = p1.x;
    Size sz = getClientSize();
    onSize(sz.width, sz.height);
}

void HorzDividerCtrl::clipCursor()
{
    Point leftTop = Point(0, 0);
    clientToScreen(leftTop);
    Size sz = getClientSize();
    Point rightBottom = Point(sz.width, sz.height);
    clientToScreen(rightBottom);

    Rect clipRect = Rect(leftTop, rightBottom);
    clipRect.right -= 50;
    clipRect.left += 50;
    ClipCursor(&clipRect);      
}