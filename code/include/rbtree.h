/*!
 * @file    rbtree.h
 *
 * @author  starshore <starshore@qq.com>
 * @date    2020-11-01
 *
 * @brief   �����
 */

#ifndef _SS_RBTREE_H_
#define _SS_RBTREE_H_

#include "platform.h"

/*! @brief   ������ڵ㶨��. */
struct rb_node {
    struct rb_node *right, *left;
    __ulong_ptr     rb_parent_color;
};
#define rb_entry(p, s, f) container_of(p, s, f)

/*! @brief   ��������. */
typedef struct rb_tree rb_tree;

/*!
 * @brief   ���������
 *
 * @param [in,out]  cmpf        �ǿգ��Ƚϻص���������ȷ����㣬С�ڷ��ظ��������ڷ�������.
 * @param [in,out]  userdata    �Ƚϻص������û����ݣ����ڱհ�.
 *
 * @returns �ɹ����غ���������ʧ�ܷ��ؿ�.
 */
SS_API rb_tree *rbtree_create(int (*cmpf)(const struct rb_node *, const struct rb_node *),
                              void *userdata);

/*!
 * @brief   ���������
 *
 * @param [in,out]  rb  ��������.
 */
SS_API void rbtree_destory(rb_tree *rb);

/*!
 * @brief   �����½ڵ�
 *
 * @param [in,out]  rb  ���ڵ�.
 * @param [in,out]  z   ������ڵ�.
 */
SS_API void rbtree_insert(struct rb_tree *rb, struct rb_node *z);

#endif // _SS_RBTREE_H_