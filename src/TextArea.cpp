#include "TextArea.h"

#include <stdio.h>

TextArea::TextArea()
{
	attr.style = WS_CHILD | WS_VISIBLE | ES_MULTILINE |
		ES_AUTOVSCROLL | WS_VSCROLL | ES_NOHIDESEL;
	attr.styleEx = WS_EX_CLIENTEDGE;
	attr.className = "EDIT";
}

void TextArea::getSelText(StrBuffer& selText)
{
	DWORD start, stop;

	sendMsg(EM_GETSEL, (WPARAM) &start, (LPARAM) &stop);

	if (start != stop)
	{
        StrBuffer text;
		getText(text);
		LPSTR temp = text.getBuffer();
		temp[stop] = 0;
        temp = &temp[start];
        selText.append(temp);
	}
}

UINT TextArea::getLineCount()
{
	return sendMsg(EM_GETLINECOUNT);
}

void TextArea::getLineAt(UINT index, StrBuffer& text)
{
	static WORD buffer[64];
	buffer[0] = sizeof(buffer);

	UINT length = sendMsg(EM_GETLINE, index, (LPARAM) buffer);
	if (length > 0)
	{
		LPSTR strBuff = text.allocate(length + 1);
		memcpy(strBuff, buffer, length);
		strBuff[length] = 0;		
	}
}


void TextArea::setTabSize(UINT tabSize)
{
    tabSize *= 4; 
	sendMsg(EM_SETTABSTOPS, 1, (LPARAM) &tabSize);
}

void TextArea::setSelection(int startIdx, int stopIdx)
{
	sendMsg(EM_SETSEL, startIdx, stopIdx);
}

void TextArea::replaceSelection(LPSTR newText)
{
	sendMsg(EM_REPLACESEL, TRUE, (LPARAM) newText);
}

BOOL TextArea::canPaste()
{
	return IsClipboardFormatAvailable(CF_TEXT);
}

int TextArea::getIndexFromLine(int lineNumber)
{
	return sendMsg(EM_LINEINDEX, lineNumber);
}

void TextArea::selectLine(int lineNumber)
{
	int idx = getIndexFromLine(lineNumber);
    StrBuffer text;
	getLineAt(lineNumber, text);
	setSelection(idx, idx + text.getLength());

}

void TextArea::scrollToEnd()
{
    sendMsg(EM_LINESCROLL , 0, getLineCount());

}

void TextArea::copy()
{
	sendMsg(WM_COPY);
}

void TextArea::paste()
{
	sendMsg(WM_PASTE);
}

void TextArea::cut()
{
	sendMsg(WM_CUT);
}

void TextArea::append(LPSTR strText)
{
    StrBuffer text;
    getText(text);
    text.append(strText);
	setText(text.getBuffer());
}

BOOL TextArea::loadFromFile(LPSTR fileName, StrBuffer& text)
{
    FILE* fd;
    fd = fopen(fileName, "rb");
    if (fd == NULL) return FALSE;
    
	fseek(fd, 0, SEEK_END);
	long length = ftell(fd);
	fseek(fd, 0, SEEK_SET);

    LPSTR buffer = text.allocate(length + 1);
	int byteRead = fread(buffer, length, 1, fd);
	if (byteRead == 1) {
        buffer[length] = 0;
    }
    fclose(fd);

    return (byteRead == 1);		
}