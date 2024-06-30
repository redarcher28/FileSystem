//
// Created by JXY on 2024/5/27.
//
#ifndef HEAD
#define HEAD
#include <algorithm>
#include"../head/head.h"
#include"../head/FilePhysicalControl.h"

void CFold::showAll(const string& prefix, bool isLast) {
    cout << prefix;
    cout << (isLast ? "+--" : "|--") << name << "/" << endl;

    string newPrefix = prefix + (isLast ? "    " : "|   ");

    for (size_t i = 0; i < childfold.size(); ++i) {
        childfold[i]->showAll(newPrefix, i == childfold.size() - 1 && file.empty());
    }

    for (size_t i = 0; i < file.size(); ++i) {
        cout << newPrefix;
        cout << (i == file.size() - 1 ? "+--" : "|--") << file[i]->name << endl;
    }
}


void CFold::ialloc(CFile* TargetFile) {
    try {
        // 从当前文件夹的文件列表中创建文件
        g_nowfold->file.push_back(TargetFile);
        TargetFile->fatherfold=g_nowfold;
        TargetFile->fatherlogo=g_nowfold->logo;
        // 输出确认信息
        cout << "File " << TargetFile->name << " has been created." << endl;
    } catch (const std::exception& e) {
        // 捕获异常并处理
        cout << "An error occurred while updating the file: " << e.what() << endl;
    }
    system("pause");
}

void CFold::ifree(CFile* &TargetFile) {
    string fileName = TargetFile->name;
    string originalContent = TargetFile->content;
    try {
        // 先释放磁盘块
        bool freeSuccess = Control.Bfree(TargetFile);
        if (!freeSuccess) {
            cout << "Failed to free file blocks for " << fileName << "." << endl;
            return;
        }

        // 从当前文件夹的文件列表中删除文件
        auto& files = g_nowfold->file;
        auto it = std::find_if(files.begin(), files.end(), [TargetFile](CFile* file) {
            return g_nowfold->compareCFile(file, TargetFile);
        });

        if (it != files.end()) {
            // 先保存需要删除的文件指针
            CFile* fileToDelete = *it;

            // 从文件列表中删除
            files.erase(it);
            // 之后再删除文件对象
            // delete fileToDelete;
            // 输出确认信息
            cout << "File " << fileName << " has been deleted." << endl;
        } else {
            cout << "File not found in the current folder." << endl;
        }
    } catch (const std::exception& e) {
        // 捕获异常并处理
        cout << "An error occurred while deleting the file: " << e.what() << endl;
        // 恢复原内容
        TargetFile->content = originalContent;
        cout << "File content has been restored to the original state." << endl;
    }
}



bool CFold::compareCFile(CFile* lhs, CFile* rhs) {
    return lhs->inode == rhs->inode;
}

void delfold(CFold* fold) {
    try {
        for (int i = 0; i < fold->childfold.size(); i++)
            delfold(fold->childfold[i]);
        for (int i = 0; i < fold->file.size(); i++)
            fold->ifree(fold->file[i]);
        Control.Bfreefold(fold);
        delete fold;
    }catch (const std::exception &e) {
        // 捕获异常并处理
        cout << "An error occurred while deleting the fold: " << e.what() << endl;
    }
}
#endif