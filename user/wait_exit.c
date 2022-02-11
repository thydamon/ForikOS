// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: user/wait_exit.c
// Create Time: Tue 01 Feb 2022 05:53:20 AM CST
// This is ... 

#include "user/wait_exit.h"
#include "global.h"
#include "lib/debug.h"
#include "thread/thread.h"
#include "lib/list.h"
#include "kernel/printk.h"
#include "mm/memory.h"
#include "lib/bitmap.h"
#include "fs/fs.h"

// �ͷ��û�������Դ: 
// 1 ҳ���ж�Ӧ������ҳ
// 2 �����ڴ��ռ����ҳ��
// 3 �رմ򿪵��ļ�
static void release_prog_resource(struct task_struct* release_thread) 
{
    uint32_t* pgdir_vaddr = release_thread->pgdir;
    uint16_t user_pde_nr = 768, pde_idx = 0;
    uint32_t pde = 0;
    uint32_t* v_pde_ptr = NULL;	    // v��ʾvar,�ͺ���pde_ptr����

    uint16_t user_pte_nr = 1024, pte_idx = 0;
    uint32_t pte = 0;
    uint32_t* v_pte_ptr = NULL;	    // �Ӹ�v��ʾvar,�ͺ���pte_ptr����

    uint32_t* first_pte_vaddr_in_pde = NULL;	// ������¼pde�е�0��pte�ĵ�ַ
    uint32_t pg_phy_addr = 0;

    // ����ҳ�����û��ռ��ҳ�� 
    while (pde_idx < user_pde_nr) 
    {
        v_pde_ptr = pgdir_vaddr + pde_idx;
        pde = *v_pde_ptr;
        if (pde & 0x00000001) 
        {   
            // ���ҳĿ¼��pλΪ1,��ʾ��ҳĿ¼���¿�����ҳ����
            first_pte_vaddr_in_pde = pte_ptr(pde_idx * 0x400000);	  // һ��ҳ���ʾ���ڴ�������4M,��0x400000
            pte_idx = 0;
            while (pte_idx < user_pte_nr) 
            {
                v_pte_ptr = first_pte_vaddr_in_pde + pte_idx;
                pte = *v_pte_ptr;
                if (pte & 0x00000001) 
                {
                    // ��pte�м�¼������ҳ��ֱ������Ӧ�ڴ�ص�λͼ����0 
                    pg_phy_addr = pte & 0xfffff000;
                    free_a_phy_page(pg_phy_addr);
                }
                pte_idx++;
            }
            // ��pde�м�¼������ҳ��ֱ������Ӧ�ڴ�ص�λͼ����0 
            pg_phy_addr = pde & 0xfffff000;
            free_a_phy_page(pg_phy_addr);
        }
        pde_idx++;
    }

    // �����û������ַ����ռ�������ڴ�
    uint32_t bitmap_pg_cnt = (release_thread->user_vaddr.vaddr_bitmap.btmp_bytes_len) / PG_SIZE;
    uint8_t* user_vaddr_pool_bitmap = release_thread->user_vaddr.vaddr_bitmap.bits;
    mfree_page(PF_KERNEL, user_vaddr_pool_bitmap, bitmap_pg_cnt);

    // �رս��̴򿪵��ļ� 
    uint8_t fd_idx = 3;
    while(fd_idx < MAX_FILES_OPEN_PER_PROC) 
    {
        if (release_thread->fd_table[fd_idx] != -1) 
        {
            sys_close(fd_idx);
        }
        fd_idx++;
    }
}

// list_traversal�Ļص�����,
// ����pelem��parent_pid�Ƿ���ppid,�ɹ�����true,ʧ���򷵻�false 
static bool find_child(struct list_elem* pelem, int32_t ppid) 
{
    // elem2entry�м�Ĳ���all_list_tagȡ����pelem��Ӧ�ı����� 
    struct task_struct* pthread = elem2entry(struct task_struct, all_list_tag, pelem);
    if (pthread->parent_pid == ppid) 
    {     // ���������parent_pidΪppid,����
        return true;   // list_traversalֻ���ڻص���������trueʱ�Ż�ֹͣ��������,�����ڴ˷���true
    }
    return false;     // ��list_traversal����������һ��Ԫ��
}

// list_traversal�Ļص�����,
// ����״̬ΪTASK_HANGING������ 
static bool find_hanging_child(struct list_elem* pelem, int32_t ppid) 
{
    struct task_struct* pthread = elem2entry(struct task_struct, all_list_tag, pelem);
    if (pthread->parent_pid == ppid && pthread->status == TASK_HANGING) 
    {
        return true;
    }
    return false; 
}

// list_traversal�Ļص�����,
// ��һ���ӽ��̹��̸�init 
static bool init_adopt_a_child(struct list_elem* pelem, int32_t pid) 
{
    struct task_struct* pthread = elem2entry(struct task_struct, all_list_tag, pelem);
    if (pthread->parent_pid == pid) 
    {     // ���ý��̵�parent_pidΪpid,����
        pthread->parent_pid = 1;
    }
    return false;		// ��list_traversal����������һ��Ԫ��
}

// �ȴ��ӽ��̵���exit,���ӽ��̵��˳�״̬���浽statusָ��ı���.
// �ɹ��򷵻��ӽ��̵�pid,ʧ���򷵻�-1 
pid_t sys_wait(int32_t* status) 
{
    struct task_struct* parent_thread = running_thread();

    while(1) 
    {
        // ���ȴ����Ѿ��ǹ���״̬������ 
        struct list_elem* child_elem = list_traversal(&thread_all_list, find_hanging_child, parent_thread->pid);
        // ���й�����ӽ��� 
        if (child_elem != NULL) 
        {
            struct task_struct* child_thread = elem2entry(struct task_struct, all_list_tag, child_elem);
            *status = child_thread->exit_status; 

            // thread_exit֮��,pcb�ᱻ����,�����ǰ��ȡpid 
            uint16_t child_pid = child_thread->pid;

            // 2 �Ӿ������к�ȫ��������ɾ�����̱���
            thread_exit(child_thread, false); // ����false,ʹthread_exit���ú�ص��˴�
            // ���̱����ǽ��̻��̵߳����������Դ, ���˸ý��̳�����ʧ�� 

            return child_pid;
        } 

        // �ж��Ƿ����ӽ��� 
        child_elem = list_traversal(&thread_all_list, find_child, parent_thread->pid);
        if (child_elem == NULL) 
        {	 // ��û���ӽ����������
            return -1;
        } 
        else 
        {
            /* ���ӽ��̻�δ������,����δ����exit,���Լ�����,ֱ���ӽ�����ִ��exitʱ���Լ����� */
            thread_block(TASK_WAITING); 
        }
    }
}

// �ӽ������������Լ�ʱ���� 
void sys_exit(int32_t status) 
{
    struct task_struct* child_thread = running_thread();
    child_thread->exit_status = status; 
    if (child_thread->parent_pid == -1) 
    {
        PANIC("sys_exit: child_thread->parent_pid is -1\n");
    }

    // ������child_thread�������ӽ��̶����̸�init 
    list_traversal(&thread_all_list, init_adopt_a_child, child_thread->pid);

    // ���ս���child_thread����Դ 
    release_prog_resource(child_thread); 

    // ������������ڵȴ��ӽ����˳�,�������̻��� 
    struct task_struct* parent_thread = pid2thread(child_thread->parent_pid);


    if (parent_thread->status == TASK_WAITING) 
    {
        thread_unblock(parent_thread);
    }

    // ���Լ�����,�ȴ������̻�ȡ��status,��������pcb 
    thread_block(TASK_HANGING);
}
