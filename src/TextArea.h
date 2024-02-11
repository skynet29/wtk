#ifndef TextArea_H
#define TextArea_H

#include "Window.h"

class DllExport TextArea : public Control {
public:
    TextArea();

    void getSelText(StrBuffer& selText);
    UINT getLineCount();
    void getLineAt(UINT index, StrBuffer& text);
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
    BOOL loadFromFile(LPSTR fileName, StrBuffer& text);

};

#endif