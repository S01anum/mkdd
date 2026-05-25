#ifndef PRINTWINDOW_H
#define PRINTWINDOW_H

#include <JSystem/J2D/J2DPrint.h>
#include <JSystem/J2D/J2DPane.h>
#include <JSystem/JKernel/JKRHeap.h>

class PrintWindow
{
public:
    PrintWindow(JKRHeap *heap);
    void init();
    void draw();
    void calc();

    J2DTextBox *getTextBox();
    J2DTextBox::TFontSize getBaseFontSize();

    // TODO: many inlines

    static const u64 mBckTag[13];    // 0x80375e70
    static const u64 mBtkTag[4];     // 0x80375ed8
    static const u64 mBrkTag[9];     // 0x80375ef8
    static const u64 mTextBoxTag[4]; // 0x80375f40
    static const u64 mVisibleTag[3]; // 0x80375f60

//private:
    u8 _0[0x2e8];
    u32 _2e8;
    u32 _2ec;
    u32 _2f0;
    f32 _2f4;
    u32 _2f8;
    u8 _2fc[0x308 - 0x2fc];
}; // Size: 0x308

#endif
