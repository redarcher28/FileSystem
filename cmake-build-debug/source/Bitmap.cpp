//
// Created by JXY on 2024/5/27.
//
#include"../head/Bitmap.h"
#include "../head/head.h"

void Bitmap::DrawBitmap() {
    for (int i = 0; i < 100; i++) {
        DiskStatus[i] = disk[i].Type;
    }
    for (int i = 0; i < 100; i++) {
        switch (DiskStatus[i]) {
            case 0:
                cout << "空";
                break;
            case 1:
                cout << "索";
                break;
            case 2:
                cout << "文";
                break;
            case 3:
                cout << "超";
                break;
        }
        // 每行10个元素，输出换行符
        if ((i + 1) % 10 == 0) {
            std::cout << std::endl;
        }
    }
}