#ifndef FILESYSTEM_BITMAP_H
#define FILESYSTEM_BITMAP_H

class Bitmap {
public:
    Bitmap() {
        for (int &status : DiskStatus) {
            status = 0;  // 初始化磁盘状态为0（空闲）
        }
    }
    void DrawBitmap();
private:
    int DiskStatus[100];
};

extern Bitmap BlockBitmap;
#endif //FILESYSTEM_BITMAP_H
