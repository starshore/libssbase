/*!
 * @file    rbtree.c
 *
 * @author  starshore <starshore@qq.com>
 * @date    2020-11-01
 *
 * @brief   �����
 */

#include "rbtree.h"

/*! @brief   ��������� */
struct rb_tree {

    /*! @brief   ���ڵ�. */
    struct rb_node *root;

    /*! @brief   Ҷ���ڱ��ڵ�. */
    struct rb_node *nil;

    /*!
     * @brief   �ص��������Ƚ����ڵ��С
     *
     * @param   x   �ڵ�x.
     * @param   y   �ڵ�y.
     *
     * @returns �Ƚ���������С�ڷ��ظ��������ڷ���0�����ڷ�������.
     */
    int (*rb_compare_callback)(const struct rb_node *x, const struct rb_node *y);

    /*! @brief   �Ƚϻص������û����� */
    void *rb_compare_userdata;

    /*!
     * @brief   �ص�����������ڵ���Դ
     *
     * @param [in,out]  x   �ڵ�x.
     */
    void (*rb_free_callback)(struct rb_node *x);
};

#define rb_parent(r) ((struct rb_node *)((r)->rb_parent_color & ~1))
#define rb_color(r)  ((r)->rb_parent_color & 1)

static inline void rb_set_color(struct rb_node *x)
{
    x->rb_parent_color |= 1;
}

static inline void rb_reset_color(struct rb_node *x)
{
    x->rb_parent_color &= ~1;
}

static inline void rb_set_parent(struct rb_node *x, struct rb_node *p)
{
    x->rb_parent_color = (x->rb_parent_color & 1) | (__ulong_ptr)p;
}

/*!
 * @brief   ����������.
 *
 * @param [in,out]  rb  ���ڵ�.
 * @param [in,out]  x   Ŀ��ڵ�.
 */
static void rbtree_rotate_left(struct rb_tree *rb, struct rb_node *x)
{
    struct rb_node *y;
    struct rb_node *p;

    y = x->right;
    p = rb_parent(x);

    x->right = y->left;

    if (y->left != rb->nil)
        rb_set_parent(y->left, x);

    rb_set_parent(y, p);

    if (p == rb->nil)
        rb->root = y;
    else if (x == p->left)
        p->left = y;
    else
        p->right = y;

    y->left = x;
    rb_set_parent(x, y);
}

/*!
 * @brief   ����������.
 *
 * @param [in,out]  rb  ���ڵ�.
 * @param [in,out]  x   Ŀ��ڵ�.
 */
static void rbtree_rotate_right(struct rb_tree *rb, struct rb_node *x)
{
    struct rb_node *y;
    struct rb_node *p;

    y = x->left;
    p = rb_parent(x);

    x->left = y->right;

    if (y->right != rb->nil)
        rb_set_parent(y->right, x);

    rb_set_parent(y, p);

    if (p == rb->nil)
        rb->root = y;
    else if (x == p->right)
        p->right = y;
    else
        p->left = y;

    y->right = x;
    rb_set_parent(x, y);
}

/*!
 * @brief   �����ָ�ƽ��
 *
 * @param [in,out]  rb  ���ڵ�.
 * @param [in,out]  z   ������ڵ�.
 */
static void rbtree_insert_fixup(struct rb_tree *rb, struct rb_node *z)
{
    struct rb_node *p;
    struct rb_node *gp;
    struct rb_node *y;

    while ((p = rb_parent(z)) && rb_color(p)) {

        gp = rb_parent(p);

        if (p == gp->left) {

            y = gp->right;

            if (rb_color(y)) {
                rb_reset_color(p);
                rb_reset_color(y);
                rb_set_color(gp);
                z = gp;
            }
            else if (z == p->right) {
                z = p;
                rbtree_rotate_left(rb, z);
            }
            else {
                rb_reset_color(p);
                rb_set_color(gp);
                rbtree_rotate_right(rb, gp);
            }
        }
        else {

            y = gp->left;

            if (rb_color(y)) {
                rb_reset_color(p);
                rb_reset_color(y);
                rb_set_color(gp);
                z = gp;
            }
            else if (z == p->left) {
                z = p;
                rbtree_rotate_right(rb, z);
            }
            else {
                rb_reset_color(p);
                rb_set_color(gp);
                rbtree_rotate_left(rb, gp);
            }
        }
    }

    rb_reset_color(rb->root);
}

/*!
 * @brief   �����½ڵ�
 *
 * @param [in,out]  rb  ���ڵ�.
 * @param [in,out]  z   ������ڵ�.
 */
SS_API void rbtree_insert(struct rb_tree *rb, struct rb_node *z)
{
    struct rb_node *x;
    struct rb_node *y;

    x = rb->root;
    y = rb->nil;

    while (x != rb->nil) {
        y = x;
        if (rb->rb_compare_callback(z, x) < 0)
            x = x->left;
        else
            x = x->right;
    }

    rb_set_parent(z, y);
    if (y == rb->nil)
        rb->root = z;
    else if (rb->rb_compare_callback(z, y) < 0)
        y->left = z;
    else
        y->right = z;

    z->left  = rb->nil;
    z->right = rb->nil;
    rb_set_color(z);

    rbtree_insert_fixup(rb, z);
}

struct rb_node *rbtree_increment(struct rb_tree *rb, struct rb_node *x)
{
    struct rb_node *y;

    if (x->right != rb->nil) {
        x = x->right;
        while (x->left != rb->nil)
            x = x->left;
    }
    else {
        y = rb_parent(x);
        while (x == y->left) {
            x = y;
            y = rb_parent(y);
        }

        if (x->right != y)
            x = y;
    }

    return x;
}

struct rb_node *rbtree_decrement(struct rb_tree *rb, struct rb_node *x)
{
    struct rb_node *y;

    if (rb_color(x) && rb_parent(rb_parent(x)) == x) {
        x = x->right;
    }
    else if (x->left != 0) {
        y = x->left;
        while (y->right != 0)
            y = y->right;
        x = y;
    }
    else {
        y = rb_parent(x);
        while (x == y->left) {
            x = y;
            y = rb_parent(y);
        }
        x = y;
    }

    return x;
}

/*!
 * @brief   ���������
 *
 * @param [in,out]  cmpf        �ǿգ��ȽϺ�������ȷ����㣬С�ڷ��ظ��������ڷ�������.
 * @param [in,out]  userdata    �Ƚϻص������û����ݣ����ڱհ�.
 *
 * @returns �ɹ����غ���������ʧ�ܷ��ؿ�.
 */
struct rb_tree *rbtree_create(int (*cmpf)(const struct rb_node *, const struct rb_node *),
                              void *userdata)
{
    struct rb_tree *rb;
    struct rb_node *nil;

    rb  = (struct rb_tree *)malloc(sizeof(struct rb_tree));
    nil = (struct rb_node *)malloc(sizeof(struct rb_node));

    if (!rb || !nil || !cmpf) {
        goto l_fail;
    }

    nil->left = nil->right = nil;
    rb_set_parent(nil, nil);
    rb_reset_color(nil);

    rb->root = rb->nil = nil;

    rb->rb_compare_callback = cmpf;
    rb->rb_compare_userdata = userdata;
    rb->rb_free_callback    = NULL;

    return rb;

l_fail:
    if (rb)
        free(rb);
    if (nil)
        free(nil);

    return NULL;
}

/*!
 * @brief   ���������
 *
 * @param [in,out]  rb  ��������.
 */
void rbtree_destory(struct rb_tree *rb)
{
    if (!rb)
        return;

    // TODO ���free�ص��������ڣ���������ڵ�
    free(rb->nil);
    free(rb);
}
