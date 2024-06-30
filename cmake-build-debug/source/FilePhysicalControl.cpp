#ifndef FilePhysicalControl
#define FilePhysicalControl
#include "../head/FilePhysicalControl.h"

int CFilePhysicalControl::FileSplit(const CFile* targetFile) const {
    int FileLength = int(targetFile->content.size());
    return (FileLength + 3) / 4;
}

bool CFilePhysicalControl::Balloc(CFile* &targetFile) {
    int initialDiskPointer = disk_pointer;  // 保存初始磁盘指针位置，以便失败时回滚

    // 先从硬盘中找到一个空闲盘块用于存储索引文件
    bool inodeAllocated = false;
    for (int i = 0; i < 100; i++, IncrementDiskPointer()) {
        if (disk[disk_pointer].Type == 0) {  // 空闲块
            disk[disk_pointer].Type = 1;  // 设置为索引文件
            targetFile->inode = disk_pointer;
            disk[disk_pointer].File = *targetFile;
            inodeAllocated = true;
            break;
        }
    }

    if (!inodeAllocated) {
        return false;  // 没有找到空闲块存储索引文件
    }

    // 分配文件块
    vector<int> allocatedBlocks;  // 用于保存已分配的块，以便失败时回滚
    int FileBlockNumber = FileSplit(targetFile);
    for (int i = 0; i < FileBlockNumber; i++) {
        bool blockAllocated = false;
        for (int j = 0; j < 100; j++, IncrementDiskPointer()) {
            if (disk[disk_pointer].Type == 0) {  // 空闲块
                disk[disk_pointer].Type = 2;  // 设置为文件块
                targetFile->diskblock.push_back(disk_pointer);  // 保存分配的块位置
                allocatedBlocks.push_back(disk_pointer);
                blockAllocated = true;
                break;
            }
        }

        if (!blockAllocated) {
            // 分配失败，回滚之前的所有分配
            for (int allocatedBlock : allocatedBlocks) {
                disk[allocatedBlock].Type = 0;  // 将块标记为空闲
            }
            disk[targetFile->inode].Type = 0;  // 释放索引文件块
            targetFile->diskblock.clear();
            return false;  // 返回分配失败
        }
    }

    return true;  // 返回分配成功
}

bool CFilePhysicalControl::Bfree(CFile* &targetFile) {
    // 遍历目标文件的所有磁盘块，并释放它们
    for (int block : targetFile->diskblock) {
        if (disk[block].Type == 2) {  // 确认该块属于文件
            disk[block].Type = 0;  // 将磁盘块标记为空闲
        } else {
            return false;  // 如果发现任何问题，返回 false 表示释放失败
        }
    }
    targetFile->diskblock.clear();  // 清空文件的块列表
    return true;  // 返回 true 表示释放成功
}

bool CFilePhysicalControl::UpdateFileBlock(CFile* &targetFile) {
    int newFileBlockNumber = FileSplit(targetFile);
    int Interval = targetFile->diskblock.size() - newFileBlockNumber;

    if (Interval > 0) {
        // 如果 Interval 为正数，增加文件块数量
        for (int i = 0; i < Interval; i++) {
            bool blockAssigned = false;
            for (int j = 0; j < 100; j++, IncrementDiskPointer()) {
                if (!disk[disk_pointer].Type) {
                    disk[disk_pointer].Type = 2;
                    targetFile->diskblock.push_back(disk_pointer);  // 添加新的文件块
                    blockAssigned = true;
                    break;
                }
            }
            if (!blockAssigned) {
                // 如果找不到空闲块，回滚之前的操作
                for (int j = 0; j < i; j++) {
                    disk[targetFile->diskblock.back()].Type = 0;
                    targetFile->diskblock.pop_back();
                }
                return false;  // 更新失败
            }
        }
    } else if (Interval < 0) {
        // 如果 Interval 为负数，减少文件块数量
        Interval = -Interval;  // 取绝对值
        for (int i = 0; i < Interval; i++) {
            if (!targetFile->diskblock.empty()) {
                int blockToRemove = targetFile->diskblock.back();
                disk[blockToRemove].Type = 0;  // 将类型重置为0
                targetFile->diskblock.pop_back();  // 从文件的块列表中删除
            } else {
                return false;  // 没有更多块可以删除，更新失败
            }
        }
    }
    return true;  // 更新成功
}

void CFilePhysicalControl::IncrementDiskPointer() {
    disk_pointer = (disk_pointer + 1) % 100;
}


bool CFilePhysicalControl::Bfreefold(CFold *Fold){
    g_usedlogo[Fold->logo]=false;
    disk[Fold->inode].Type=0;//重新归为空闲块
    return 1;
}
#endif