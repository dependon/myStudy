/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author: Deng jinhui<dengjinhui@uniontech.com>
*
* Maintainer: Deng jinhui <dengjinhui@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <QString>
#include <QPointer>

#define MAXSIZE 512

struct test {
    char a: 4;
    char b: 4;
    unsigned short c: 8;
    unsigned long d;
};

class AVLTreeNode
{
public:
    AVLTreeNode();
    ~AVLTreeNode();
    bool isEmpty();
    bool insertNode(int argKey, QString argValue);
    bool deleteNode(int argKey);
    int getDepth();
    int getDepth(AVLTreeNode *argNode);
    int getBalance();


    void singleRotateRight(AVLTreeNode *argNode);
    void singleRotateLeft(AVLTreeNode *argNode);
    void rotateRL(AVLTreeNode *argNode);
    void rotateLR(AVLTreeNode *argNode);

    unsigned int getCount() const
    {
        return m_i;
    }
public:
    int key;
    QString value;
    AVLTreeNode *LeftChild;
    AVLTreeNode *RightChild;
private:
    unsigned int m_i;
    int m_balanceFactor;
};
