#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "item.h"
#include "search.h"

int main(int argc, char **argv)
{
    /* 引数処理: ユーザ入力が正しくない場合は使い方を標準エラーに表示して終了 */
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <file name> <max capacity (double)>\n", argv[0]);
        exit(1);
    }

    const double W = load_double(argv[2]);
    assert(W >= 0.0);

    printf("max capacity: W = %.f\n", W);

    Itemset *items = load_itemset(argv[1]);
    print_itemset(items);
    const size_t n = get_nitem(items);

    // ソルバーで解く
    Result total = solve(items, W);

    // 表示する
    printf("----\nbest solution:\n");
    printf("value: %4.1f\n", total.value);
    for (int i = 0; i < n; ++i)
    {
        printf("%d", total.list & (1 << (n - i - 1)) ? 1 : 0);
    }
    printf("\n");

    free_itemset(items);
    return 0;
}
