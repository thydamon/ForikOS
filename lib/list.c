// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: list.c
// Create Time: Sun 02 Jan 2022 04:12:58 PM CST
// This is ... 

#include "lib/list.h"
#include "kernel/interrupt.h"

// 初始化双向链表
void list_init(struct list* list)
{
    list->head.prev = NULL;
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    list->tail.next = NULL;
}

// 把链表元素elem插入在元素before之前
void list_insert_before(struct list_elem* before, struct list_elem* elem)
{
    enum intr_status old_status = intr_disable();

    // 将before前驱元素的后继元素更新为elem, 暂时使before脱离链表
    before->prev->next = elem;
    // 更新elem自己的前驱结点为before的前驱
    elem->prev = before->prev;
    elem->next = before;

    // 更新before的前驱结点为elem
    before->prev = elem;

    intr_set_status(old_status);
}

// 添加元素到列表队首,类似栈push操作
void list_push(struct list* plist, struct list_elem* elem)
{
    list_insert_before(plist->head.next, elem);
}

// 追加元素到链表队尾,类似队列的先进先出操作
void list_append(struct list* plist, struct list_elem* elem)
{
    list_insert_before(&plist->tail, elem);
}

// 移除元素
void list_remove(struct list_elem* pelem)
{
    enum intr_status old_status = intr_disable();

    pelem->prev->next = pelem->next;
    pelem->next->prev = pelem->prev;

    intr_set_status(old_status);
}

// 弹出第一个元素
struct list_elem* list_pop(struct list* plist)
{
    struct list_elem* elem = plist->head.next;
    list_remove(elem);
    return elem;
}

// 从链表中查找元素obj_elem,成功时返回true,失败时返回false
bool elem_find(struct list* plist, struct list_elem* obj_elem)
{
    struct list_elem* elem = plist->head.next;

    while (elem != &plist->tail)
    {
        if (elem == obj_elem)
        {
            return true;
        }

        elem = elem->next;
    }

    return false;
}

// 列表plist中的每个元素elem和arg传给回调函数func
struct list_elem* list_traversal(struct list* plist, function func, int arg)
{
    struct list_elem* elem = plist->head.next;

    if (list_empty(plist))
    {
        return NULL;
    }

    while (elem != &plist->tail)
    {
        if (func(elem, arg))
        {
            return elem;
        }

        elem  = elem->next;
    }

    return NULL;
}

uint32_t list_len(struct list* plist)
{
    struct list_elem* elem = plist->head.next;

    uint32_t length = 0;
    while (elem != &plist->tail)
    {
        length++;
        elem = elem->next;
    }

    return length;
}

bool list_empty(struct list* plist)
{
    return (plist->head.next == &plist->tail ? true : false);
}
