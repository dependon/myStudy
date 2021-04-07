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
#include "avltree.h"

AVLTreeNode::AVLTreeNode()
{
    LeftChild = nullptr;
    RightChild = nullptr;
}

AVLTreeNode::~AVLTreeNode()
{
}

bool AVLTreeNode::isEmpty()
{
    if (key) {
        return false;
    } else {
        return true;
    }
}

bool AVLTreeNode::insertNode(int argKey, QString argValue)
{
    AVLTreeNode *tempNode = new AVLTreeNode;
    tempNode->key = argKey;
    tempNode->value = argValue;
    if (key > argKey) {
        insertNode(argKey, argValue);
        if (getBalance() > 1) {
            rotateRL(this);
        }
    }
    //todo
    return false;
}

int AVLTreeNode::getDepth()
{
    return getDepth(this);
}

int AVLTreeNode::getDepth(AVLTreeNode *argNode)
{
    if (!argNode) {
        return 0;
    }
    int lHeight = getDepth(argNode->LeftChild);
    int rHeight = getDepth(argNode->RightChild);
    int nMaxHeight = lHeight > rHeight ? lHeight : rHeight;
    nMaxHeight++;
    return  nMaxHeight;
}

int AVLTreeNode::getBalance()
{
    return getDepth(LeftChild) - getDepth(RightChild);
}

/** AVLTree Rotate Rules
 *
 *  Node's left child
 *
 *        (1)k2         (2)k1     *         (1)k2         (2)k1
 *        /             / \       *         /             / \
 *      (2)k1         (4)  (1)    *       (2)k1         (4)  (1)
 *      /                         *       / \                /
 *    (4)                         *     (4) (3)             (3)
 *
 *  choose parent and grandparent Node , get mid Node to set as root
 */

void AVLTreeNode::singleRotateRight(AVLTreeNode *argNode)
{

}

void AVLTreeNode::rotateRL(AVLTreeNode *argNode)
{

}



