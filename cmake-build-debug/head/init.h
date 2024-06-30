#ifndef INIT_H
#define INIT_H
#include <fstream>
#include "head.h"
#include "User.h"
//444444444
int init() {
    ifstream fin("disk.txt"); //打开存储目录
    if (!fin.is_open()) {
        cerr << "Error opening file 'disk.txt'" << endl;
        return -1;
    }

    //根目录处理
    disk[1].Fold.name = "ROOT";
    disk[1].Fold.logo = 0;
    disk[1].Fold.fatherlogo = -1; //根目录没有上级目录
    disk[1].Fold.fatherfold = nullptr;
    disk[1].Fold.inode = 1; //根目录存在一号块
    disk[1].Type = 3;

    int temp;
    fin >> temp;
    fin >> temp;

    for (int i = 2; i < MAXDISK; i++) {
        CDisk disktemp;
        CFold foldtemp;
        CFile filetemp;
        if (!(fin >> disktemp.Type)) {
            cerr << "Error reading disk type for block " << i << endl;
            continue;
        }
        //cout << "Read disk type: " << disktemp.Type << " for block " << i << endl;

        if (disktemp.Type == 0) {
            // 空闲块直接过
        } else if (disktemp.Type == 1) {
            // 是文件索引块
            if (!(fin >> filetemp.inode >> filetemp.name >> filetemp.content >> filetemp.isotherread >> filetemp.isotherwrite >> filetemp.fatherlogo >> filetemp.owner)) {
                cerr << "Error reading file information for block " << i << endl;
                continue;
            }
            int length;
            if (!(fin >> length)) {
                cerr << "Error reading length of diskblock for file in block " << i << endl;
                continue;
            }
            int num;
            while (length--) {
                if (!(fin >> num)) {
                    cerr << "Error reading diskblock numbers for file in block " << i << endl;
                    break;
                }
                filetemp.diskblock.push_back(num);
            }
            disktemp.File = filetemp;
        } else if (disktemp.Type == 2) {
            // 文件块，直接跳过
        } else if (disktemp.Type == 3) {
            // 文件夹类
            if (!(fin >> foldtemp.name >> foldtemp.logo >> foldtemp.fatherlogo >> foldtemp.inode)) {
                cerr << "Error reading folder information for block " << i << endl;
                continue;
            }
            g_usedlogo[foldtemp.logo] = true;
            disktemp.Fold = foldtemp;
        }
        disk[i] = disktemp;
    }

    for (int i = 1; i < MAXDISK; i++) {
        if (disk[i].Type == 3) {
            for (int j = 1; j < MAXDISK; j++) {
                switch (disk[j].Type) {
                    case 1:
                        // 是文件索引块
                        if (disk[j].File.fatherlogo == disk[i].Fold.logo) {
                            // 当前文件的上级目录是当前文件夹
                            disk[i].Fold.file.push_back(&disk[j].File);
                            disk[j].File.fatherfold = &disk[i].Fold;
                        }
                        break;
                    case 3:
                        // 是文件夹块
                        if (disk[j].Fold.fatherlogo == disk[i].Fold.logo) {
                            // 当前文件夹j的上级目录是当前文件夹i
                            disk[i].Fold.childfold.push_back(&disk[j].Fold);
                          //  cout << disk[i].Fold.name << " is " << disk[j].Fold.name << " father " << endl;
                            disk[j].Fold.fatherfold = &disk[i].Fold;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    userList.AddUser("111", "111", "111");
    return 0;
}

void store() {
    ofstream fout("disk.txt");
    if (!fout.is_open()) {
        cerr << "Error opening file 'disk.txt' for writing" << endl;
        return;
    }

    fout << -1 << endl << 3 << endl;
    for (int i = 2; i < MAXDISK; i++) {
        CDisk disktemp = disk[i];
        CFold foldtemp = disk[i].Fold;
        CFile filetemp = disk[i].File;
        fout << disktemp.Type;
        if (disktemp.Type == 0) {
            // 空闲块直接过
        } else if (disktemp.Type == 1) {
            // 是文件索引块
            fout << ' ' << filetemp.inode << ' ' << filetemp.name << ' ' << filetemp.content << ' ' << filetemp.isotherread << ' ' << filetemp.isotherwrite << ' ' << filetemp.fatherlogo << ' ' << filetemp.owner;
            int length = filetemp.diskblock.size();
            fout << ' ' << length;
            for (int j = 0; j < filetemp.diskblock.size(); j++) {
                fout << ' ' << filetemp.diskblock[j];
            }
        } else if (disktemp.Type == 2) {
            // 文件块，直接跳过
        } else if (disktemp.Type == 3) {
            // 文件夹类
            fout << ' ' << foldtemp.name << ' ' << foldtemp.logo << ' ' << foldtemp.fatherlogo << ' ' << foldtemp.inode;
            g_usedlogo[foldtemp.logo] = true;
        }
        fout << endl;
    }
}

#endif
