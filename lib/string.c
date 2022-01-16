// Copyright 2021 by Forik. All rights reserved.
// License(BSD/GPL/...)
// Author: Damon
// Mail: thydamon@gmail.com
// File Name: string.c
// Create Time: Tue 04 Jan 2022 09:19:49 AM CST
// This is ... 

#include "lib/string.h"
#include "global.h"
#include "lib/debug.h"

/* ��dst_��ʼ��size���ֽ���Ϊvalue */
void memset(void* dst_, uint8_t value, uint32_t size) {
   ASSERT(dst_ != NULL);
   uint8_t* dst = (uint8_t*)dst_;
   while (size-- > 0)
      *dst++ = value;
}

/* ��src_��ʼ��size���ֽڸ��Ƶ�dst_ */
void memcpy(void* dst_, const void* src_, uint32_t size) {
   ASSERT(dst_ != NULL && src_ != NULL);
   uint8_t* dst = dst_;
   const uint8_t* src = src_;
   while (size-- > 0)
      *dst++ = *src++;
}

/* �����Ƚ��Ե�ַa_�͵�ַb_��ͷ��size���ֽ�,������򷵻�0,��a_����b_����+1,���򷵻�-1 */
int memcmp(const void* a_, const void* b_, uint32_t size) {
   const char* a = a_;
   const char* b = b_;
   ASSERT(a != NULL || b != NULL);
   while (size-- > 0) {
      if(*a != *b) {
	 return *a > *b ? 1 : -1; 
      }
      a++;
      b++;
   }
   return 0;
}

/* ���ַ�����src_���Ƶ�dst_ */
char* strcpy(char* dst_, const char* src_) {
   ASSERT(dst_ != NULL && src_ != NULL);
   char* r = dst_;		       // ��������Ŀ���ַ�����ʼ��ַ
   while((*dst_++ = *src_++));
   return r;
}

/* �����ַ������� */
uint32_t strlen(const char* str) {
   ASSERT(str != NULL);
   const char* p = str;
   while(*p++);
   return (p - str - 1);
}

/* �Ƚ������ַ���,��a_�е��ַ�����b_�е��ַ�����1,���ʱ����0,���򷵻�-1. */
int8_t strcmp (const char* a, const char* b) {
   ASSERT(a != NULL && b != NULL);
   while (*a != 0 && *a == *b) {
      a++;
      b++;
   }
/* ���*aС��*b�ͷ���-1,���������*a���ڵ���*b��������ں���Ĳ������ʽ"*a > *b"��,
 * ��*a����*b,���ʽ�͵���1,����ͱ��ʽ������,Ҳ���ǲ���ֵΪ0,ǡǡ��ʾ*a����*b */
   return *a < *b ? -1 : *a > *b;
}

/* �����Ҳ����ַ���str���״γ����ַ�ch�ĵ�ַ(�����±�,�ǵ�ַ) */
char* strchr(const char* str, const uint8_t ch) {
   ASSERT(str != NULL);
   while (*str != 0) {
      if (*str == ch) {
	 return (char*)str;	    // ��Ҫǿ��ת���ɺͷ���ֵ����һ��,����������ᱨconst���Զ�ʧ,��ͬ.
      }
      str++;
   }
   return NULL;
}

/* �Ӻ���ǰ�����ַ���str���״γ����ַ�ch�ĵ�ַ(�����±�,�ǵ�ַ) */
char* strrchr(const char* str, const uint8_t ch) {
   ASSERT(str != NULL);
   const char* last_char = NULL;
   /* ��ͷ��β����һ��,������ch�ַ�,last_char���Ǹ��ַ����һ�γ����ڴ��еĵ�ַ(�����±�,�ǵ�ַ)*/
   while (*str != 0) {
      if (*str == ch) {
	 last_char = str;
      }
      str++;
   }
   return (char*)last_char;
}

/* ���ַ���src_ƴ�ӵ�dst_��,����ƴ�ӵĴ���ַ */
char* strcat(char* dst_, const char* src_) {
   ASSERT(dst_ != NULL && src_ != NULL);
   char* str = dst_;
   while (*str++);
   --str;      // �𿴴��ˣ�--str�Ƕ�����һ�䣬������while��ѭ����
   while((*str++ = *src_++));	 // ��*str����ֵΪ0ʱ,��ʱ���ʽ������,����������ַ�����β��0.
   return dst_;
}

/* ���ַ���str�в���ָ���ַ�ch���ֵĴ��� */
uint32_t strchrs(const char* str, uint8_t ch) {
   ASSERT(str != NULL);
   uint32_t ch_cnt = 0;
   const char* p = str;
   while(*p != 0) {
      if (*p == ch) {
	 ch_cnt++;
      }
      p++;
   }
   return ch_cnt;
}
