
#include <windows.h>

#include "Application.h"
#include "Frame.h"

#include "TreeCtrl.h"
#include "File.h"
#include "StrVector.h"
#include "Layout.h"
#include "TextField.h"
#include "Button.h"

class MyTree : public DynamicTreeCtrl
{
public:
    MyTree(UINT id) : DynamicTreeCtrl(id) {}

protected:
    void getFolder(TreeNode *pNode, StrVector &folders)
    {
        StrBuffer path(getenv("USERPROFILE"));
        path.append("\\");
        pNode->getNodePath(path);
        // printf("path=%s\n", path.getBuffer());
        File::findFolder(path, folders);
    }

    BOOL onItemHasChildren(TreeNode *pNode)
    {
        StrVector folders;
        getFolder(pNode, folders);
        return (folders.getCount() > 0);
    }
    void onItemExpanding(TreeNode *pNode)
    {
        printf("onItemExpanding\n");

        StrVector folders;
        getFolder(pNode, folders);

        for (UINT i = 0; i < folders.getCount(); i++)
        {
            pNode->addNode(folders[i]);
        }
    }
};

class MyFrame : public Frame
{
private:
public:
    enum
    {
        ID_TREE1 = 100
    };

    TreeCtrl *tree1;
    TextField *text1;
    Container *cont1;

    MyFrame() : Frame("Test 7")
    {
        cont1 = new Container();
        cont1->setBackColor(Color::getSysColor());

        Layout layout2(cont1, 0, 0);
        layout2.add(new Button("OK", IDOK), Size(60, 25));
        layout2.add(new Button("Cancel", IDCANCEL), Size(60, 25), 10);

        Layout layout(this, 10, 10);
        layout.add(tree1 = new MyTree(ID_TREE1), Size(500, 400));
        layout.endl();
        layout.addLabel("Folder:", 0, 25);
        layout.addEnd(text1 = new TextField(0), 25, 5);
        layout.endl();
        layout.addRight(cont1, cont1->getPackSize());

        setBackColor(Color::getSysColor());
    }

protected:
    void onCommand(UINT id)
    {
    }

    void onSelChange(UINT id)
    {
        if (id == ID_TREE1)
        {
            StrBuffer text(getenv("USERPROFILE"));
            text.append("\\");
            tree1->getSelNode()->getNodePath(text);
            text1->setText(text);
        }
    }

    void onCreate()
    {
        Frame::onCreate();

        StrVector folders;
        File::findFolder(getenv("USERPROFILE"), folders);
        for (UINT i = 0; i < folders.getCount(); i++)
        {
            tree1->addNode(folders[i]);
        }
        packSize(10, 10);
    }
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    // Application app;
    // MyFrame frame;

    printf("lpCmdLine=%s\n", lpCmdLine);

    WAVEOUTCAPS woc;
    unsigned long iNumDevs, i;

    /* Get the number of Digital Audio Out devices in this computer */
    iNumDevs = waveOutGetNumDevs();

    /* Go through all of those devices, displaying their names */
    for (i = 0; i < iNumDevs; i++)
    {
        /* Get info about the next device */
        if (!waveOutGetDevCaps(i, &woc, sizeof(WAVEOUTCAPS)))
        {
            /* Display its Device ID and name */
            printf("Device ID #%u: %s wChannels=%d, dwFormats=%04X, dwSupport=%04X\r\n", i, woc.szPname, woc.wChannels, woc.dwFormats, woc.dwSupport);
        }
    }

    DWORD nbBytesToPlay;
    HMMIO hMMIO = mmioOpen(lpCmdLine, 0, MMIO_READ);
    printf("hMMIO=%d\n", hMMIO);

    MMCKINFO infoParent;
    MMCKINFO infoSubchunk;
    infoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

    if (mmioDescend(hMMIO, (LPMMCKINFO)&infoParent, 0, MMIO_FINDRIFF))
    {
        printf("ERROR: This file doesn't contain a WAVE!\n");
        goto out;
    }
    infoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hMMIO, &infoSubchunk, &infoParent, MMIO_FINDCHUNK))
    {
        /* Oops! The required fmt chunk was not found! */
        printf("ERROR: Required fmt chunk was not found!\n");
        goto out;
    }

    WAVEFORMATEX WaveFormat;
    if (mmioRead(hMMIO, (HPSTR)&WaveFormat, infoSubchunk.cksize) != (LRESULT)infoSubchunk.cksize)
    {
        /* Oops! */
        printf("ERROR: reading the fmt chunk!\n");
        goto out;
    }

    printf("nChannels=%d, nSamplesPerSec=%d, nAvgBytesPerSec=%d, wBitsPerSample=%d\n",
           WaveFormat.nChannels,
           WaveFormat.nSamplesPerSec,
           WaveFormat.nAvgBytesPerSec,
           WaveFormat.wBitsPerSample);

    mmioAscend(hMMIO, &infoSubchunk, 0);
    infoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(hMMIO, &infoSubchunk, &infoParent, MMIO_FINDCHUNK))
    {
        /* Oops! */
        printf("ERROR: reading the data chunk!\n");
        goto out;
    }
    nbBytesToPlay = infoSubchunk.cksize;
    printf("nbBytesToPlay=%ld\n", nbBytesToPlay);
    printf("duration=%d\n", nbBytesToPlay / WaveFormat.nAvgBytesPerSec);

out:
    mmioClose(hMMIO, 0);

    // return app.run(frame);
    return 0;
}
