#ifndef FilePhysicalControl_H
#define FilePhysicalControl_H
#include "head.h"
#include <iostream>
//#include"ClusteredAllocation.h"
class CFilePhysicalControl {
public:
    int a;
    //CFilePhysicalControl();
    bool Balloc(CFile* &TargetFile);  // 磁盘块分配函数
    // 盘块栈分配空间
    bool Bfree(CFile* &TargetFile);   // 磁盘块释放函数
    bool ClusteredBree(CFile *&targetFile);  //释放空间给空闲盘块栈
    bool UpdateFileBlock(CFile* &TargetFile);   // 添加硬盘块函数（修改文件时会用到）
    bool Bfreefold(CFold *Fold);

private:
    int FileSplit(const CFile* targetFile) const;  // 文件硬盘分配计算函数
    static void IncrementDiskPointer();
};

extern CFilePhysicalControl Control;
#endif