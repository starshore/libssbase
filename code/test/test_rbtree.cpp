/*!
 * @file    test_rbtree.c
 *
 * @author  starshore <starshore@qq.com>
 * @date    2020-11-01
 *
 * @brief   ºìºÚÊ÷½Ó¿Ú¼¶²âÊÔ
 */

#include <libssbase.hpp>

#include <array>
#include <gtest/gtest.h>
#include <memory>

struct integer_st {
    rb_node rb;
    int     val;

    integer_st() : val(0)
    {
    }
};

int cmp_callbak(const struct rb_node *lhs, const struct rb_node *rhs)
{
    int x = rb_entry(lhs, integer_st, rb)->val;
    int y = rb_entry(lhs, integer_st, rb)->val;

    return x > y ? 1 : (x == y ? 0 : -1);
}

TEST(rbtree, init)
{
    auto tree = rbtree_create(NULL, NULL);
    ASSERT_EQ(tree, nullptr);

    tree = rbtree_create(cmp_callbak, NULL);

    std::array<integer_st, 1000> arr;

    for (auto &&element : arr) {
        element.val = rand() % 10000;
    }

    for (auto &&element : arr) {
        rbtree_insert(tree, &element.rb);
    }

    rbtree_destory(tree);
}
