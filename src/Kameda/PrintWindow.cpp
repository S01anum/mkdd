#include "Kameda/PrintWindow.h"

const u64 PrintWindow::mBckTag[13] = {
    'NGDcw1', 'GDEf_011', 'GDEf_021', 'GDEf_031',
    'GDEf_041', 'GDEf_051', 'GDEf_061', 'GDEf_071',
    'GDEf_081', 'GDEf_091', 'GDEf_101', 'GDEf_111',
    'GDEf_121'
};

const u64 PrintWindow::mBtkTag[4] = {
    'WGD_Li1', 'GDhiLi1', 'GDNoW1', 'GDYesW1',
};

const u64 PrintWindow::mBrkTag[9] = {
    'WGD1', 'WGD_Li1', 'GDhiLi1', 'GDNoB1',
    'GDYesB1', 'GDNoW1', 'GDYesW1', 'GDYesM1',
    'GDNoM1'
};

const u64 PrintWindow::mTextBoxTag[4] = {
    'TGDEr1', 'TGD1', 'GDYesM1', 'GDNoM1'
};

const u64 PrintWindow::mVisibleTag[3] = {
    'N_YN1', 'NGDNo1', 'NGDyes1'
};

PrintWindow::PrintWindow(JKRHeap *) {}

void PrintWindow::init() {}

void PrintWindow::draw() {}

void PrintWindow::calc() {}

J2DTextBox *PrintWindow::getTextBox() {}

J2DTextBox::TFontSize PrintWindow::getBaseFontSize() {}
