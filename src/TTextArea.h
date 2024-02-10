#ifndef TTEXTAREA_H
#define TTEXTAREA_H

#include "TWindow.h"

class DllExport TTextArea : public TControl {
public:
    TTextArea();

    void getSelText(TStrBuffer& selText);
    UINT getLineCount();
    void getLineAt(UINT index, TStrBuffer& text);
    void setTabSize(UINT tabSize);
    void setSelection(int startIdx, int stopIdx);
    void replaceSelection(LPSTR newText);
    BOOL canPaste();
    int getIndexFromLine(int lineNumber);
    void selectLine(int lineNumber);
    void scrollToEnd();
    void copy();
    void paste();
    void cut();
    void append(LPSTR strText);
    BOOL loadFromFile(LPSTR fileName, TStrBuffer& text);

};

#endif