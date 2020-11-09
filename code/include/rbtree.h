/*!
 * @file    rbtree.h
 *
 * @author  starshore <starshore@qq.com>
 * @date    2020-11-01
 *
 * @brief   红黑树
 */

#ifndef _SS_RBTREE_H_
#define _SS_RBTREE_H_

#include "platform.h"

/*! @brief   红黑树节点定义. */
struct rb_node {
    struct rb_node *right, *left;
    __ulong_ptr     rb_parent_color;
};
#define rb_entry(p, s, f) container_of(p, s, f)

/*! @brief   红黑树句柄. */
typedef struct rb_tree rb_tree;

/*!
 * @brief   红黑树构造
 *
 * @param [in,out]  cmpf        非空，比较回调函数，相等返回零，小于返回负数，大于返回正数.
 * @param [in,out]  userdata    比较回调函数用户数据，用于闭包.
 *
 * @returns 成功返回红黑树句柄，失败返回空.
 */
SS_API rb_tree *rbtree_create(int (*cmpf)(const struct rb_node *, const struct rb_node *),
                              void *userdata);

/*!
 * @brief   红黑树析构
 *
 * @param [in,out]  rb  红黑树句柄.
 */
SS_API void rbtree_destory(rb_tree *rb);

/*!
 * @brief   插入新节点
 *
 * @param [in,out]  rb  根节点.
 * @param [in,out]  z   待插入节点.
 */
SS_API void rbtree_insert(struct rb_tree *rb, struct rb_node *z);

#endif // _SS_RBTREE_H_