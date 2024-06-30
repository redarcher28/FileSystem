#ifndef HEAD_H
#define HEAD_H

#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include "define.h"

using namespace std;

class CFile;
class CFold;
class CDisk;

class CFile {
public:
    CFile() = default;
    CFile(string mname, string mcontent, string mowner, bool mread, bool mwrite, int mfatherlogo)
            : name(move(mname)), content(move(mcontent)), owner(move(mowner)), isotherread(mread), isotherwrite(mwrite), fatherlogo(mfatherlogo) {}

    CFold* fatherfold;  // 所属目录
    int fatherlogo;
    int inode;  // 索引文件自己的位置
    vector<int> diskblock; // 记录的所有文件块的位置
    string name;  // 文件名
    string owner;  // 文件的所有者
    string content;  // 文件内容
    bool isotherread;  // 文件是否可读
    bool isotherwrite;  // 文件是否可写
//光标移一下
    bool operator==(const CFile& other) const {
        return inode == other.inode;
    }
};

class CFold {
public:
    CFold() = default;
    CFold* fatherfold;
    int fatherlogo;
    int inode;
    vector<CFold*> childfold;
    vector<CFile*> file;
    string name;
    int logo;

    void ialloc(CFile* TargetFile);
    void ifree(CFile* &TargetFile);
    void showAll(const string& prefix = "", bool isLast = true);
    bool compareCFile(CFile* lhs, CFile* rhs);
};
void delfold(CFold* fold);


class Csuperblock {
public:
    int a;
};


class CDisk {
public:
    int Type;  // 0空闲块 1索引块 2文件块 3文件夹块 -1超级块
    CFile File;
    CFold Fold;
    Csuperblock* superblock;
};

extern CDisk disk[MAXDISK];
extern int disk_pointer;
//extern CFold ROOT;
extern CFold* g_nowfold;
extern bool g_usedlogo[MAXDISK];
#endif