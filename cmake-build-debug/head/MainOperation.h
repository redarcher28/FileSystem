//
// Created by JXY on 2024/5/26.
//
#ifndef MAINOPERATOR_H
#define MAINOPERATOR_H
#include<iostream>
#include<vector>
#include"User.h"
#include"head.h"
#include"FilePhysicalControl.h"
#include"Bitmap.h"
#include"MainOperation.h"
using namespace std;
class MainOperation{
    User CurrentUser;
    CFile* TargetFile{};
public:
   void Login();
   void Logout();
   void Menu();
   void ShowALL();
   static void CreateFile();
   void ReadFile();
   void WriteFile();
   void DeleteFile();
   static void BitMap();
   void CreateFold();  //瀚轩写的
   void DeleteFold();   //瀚轩写的
   bool isExitRequested() const;
private:
   bool SelectFile(CFile* &file, const string& operation);     //仅为内部函数

   bool exitRequested = false;
   static void OutputAllFolds();
   static void OutputAllFiles();
};
extern MainOperation mainOperation;

#endif