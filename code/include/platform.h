/*!
 *
 * @file    platform.h
 *
 * @author  starshore <starshore@qq.com>
 * @date    2020-11-01
 *
 * @brief   平台无关定义
 */

#ifndef _SS_PLATFORM_H_
#define _SS_PLATFORM_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if (__LP64__ || _WIN64)
    #define __ulong_ptr uint64_t
#else
    #define __ulong_ptr uint32_t
#endif

#ifndef offsetof
    #define offsetof(s, m) ((__ulong_ptr) & (((s *)0)->m))
#endif

#ifndef container_of
    #define container_of(p, s, f) ((s *)(((__ulong_ptr *)(p)) - offsetof(s, f)))
#endif

// 动态库导出
#ifdef SS_API_EXPORT
    #define SS_API __declspec(dllexport)
#else
    #define SS_API __declspec(dllimport)
#endif


#endif // _SS_PLATFORM_H_