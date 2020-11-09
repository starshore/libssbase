/*!
 * @file    rbtree.c
 *
 * @author  starshore <starshore@qq.com>
 * @date    2020-11-01
 *
 * @brief   红黑树
 */

#include "rbtree.h"

/*! @brief   红黑树定义 */
struct rb_tree {

    /*! @brief   根节点. */
    struct rb_node *root;

    /*! @brief   叶子哨兵节点. */
    struct rb_node *nil;

    /*!
     * @brief   回调函数，比较两节点大小
     *
     * @param   x   节点x.
     * @param   y   节点y.
     *
     * @returns 比较运算结果，小于返回负数，等于返回0，大于返回正数.
     */
    int (*rb_compare_callback)(const struct rb_node *x, const struct rb_node *y);

    /*! @brief   比较回调函数用户数据 */
    void *rb_compare_userdata;

    /*!
     * @brief   回调函数，清理节点资源
     *
     * @param [in,out]  x   节点x.
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
 * @brief   二叉树左旋.
 *
 * @param [in,out]  rb  根节点.
 * @param [in,out]  x   目标节点.
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
 * @brief   二叉树右旋.
 *
 * @param [in,out]  rb  根节点.
 * @param [in,out]  x   目标节点.
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
 * @brief   插入后恢复平衡
 *
 * @param [in,out]  rb  根节点.
 * @param [in,out]  z   待插入节点.
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
 * @brief   插入新节点
 *
 * @param [in,out]  rb  根节点.
 * @param [in,out]  z   待插入节点.
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
 * @brief   红黑树构造
 *
 * @param [in,out]  cmpf        非空，比较函数，相等返回零，小于返回负数，大于返回正数.
 * @param [in,out]  userdata    比较回调函数用户数据，用于闭包.
 *
 * @returns 成功返回红黑树句柄，失败返回空.
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
 * @brief   红黑树析构
 *
 * @param [in,out]  rb  红黑树句柄.
 */
void rbtree_destory(struct rb_tree *rb)
{
    if (!rb)
        return;

    // TODO 如果free回调函数存在，遍历清理节点
    free(rb->nil);
    free(rb);
}
