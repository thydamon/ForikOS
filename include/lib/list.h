// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: list.h
// Create Time: Sun 02 Jan 2022 03:59:23 PM CST
// This is ... 

#ifndef LIST_H_
#define LIST_H_

#include "global.h"

#define offset(struct_type,member) (int)(&((struct_type*)0)->member)
#define elem2entry(struct_type, struct_member_name, elem_ptr) \
    (struct_type*)((int)elem_ptr - offset(struct_type, struct_member_name))

// 链表节点
struct list_elem
{
    struct list_elem* prev;
    struct list_elem* next;
};

// 队列结构
struct list
{
    // 队首固定不变,第一个元素是head.next
    struct list_elem head;
    // 队尾
    struct list_elem tail;
};

// 函数指针,用于回调函数
typedef bool (function)(struct list_elem*, int arg);

void list_init(struct list*);
void list_insert_before(struct list_elem* before, struct list_elem* elem);
void list_push(struct list* plist, struct list_elem* elem);
void list_iterate(struct list* plist);
void list_append(struct list* plist, struct list_elem* elem);
void list_remove(struct list_elem* pelem);
struct list_elem* list_pop(struct list* plist);
bool list_empty(struct list* plist);
uint32_t list_len(struct list* plist);
struct list_elem* list_traversal(struct list* plist, function func, int arg);
bool elem_find(struct list* plist, struct list_elem* obj_elem);

#endif
