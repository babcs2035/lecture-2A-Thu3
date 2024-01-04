#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "search.h"
#include "item.h"

// ソルバーは search を index = 0 で呼び出すだけ
Result solve(const Itemset *list, double capacity)
{
    // 品物を入れたかどうかを記録するフラグ配列 => !!最大の組み合わせが返ってくる訳ではない!!
    Result max_value = search(0, list, capacity, 0, 0.0, 0.0);
    return max_value;
}

// 再帰的な探索関数
Result search(int index, const Itemset *list, double capacity, int flags, double sum_v, double sum_w)
{
    int max_index = get_nitem(list);
    assert(index >= 0 && sum_v >= 0 && sum_w >= 0);
    // 必ず再帰の停止条件を明記する (最初が望ましい)
    if (index == max_index)
    {
        const char *format_ok = ", total_value = %5.1f, total_weight = %5.1f\n";
        const char *format_ng = ", total_value = %5.1f, total_weight = %5.1f NG\n";
        Result res;
        res.list = flags;
        for (int i = 0; i < max_index; i++)
        {
            printf("%d", flags & (1 << (max_index - i - 1)) ? 1 : 0);
        }
        printf((sum_w < capacity) ? format_ok : format_ng, sum_v, sum_w);
        res.value = sum_w < capacity ? sum_v : 0;
        return res;
    }

    // 以下は再帰の更新式: 現在のindex の品物を使う or 使わないで分岐し、index をインクリメントして再帰的にsearch() を実行する

    const Result r0 = search(index + 1, list, capacity, flags, sum_v, sum_w);

    Item *item = get_item(list, index);
    Result r1;
    r1.value = 0;
    if (sum_w + get_itemweight(item) < capacity)
    {
        r1 = search(index + 1, list, capacity, flags | (1 << (max_index - index - 1)), sum_v + get_itemvalue(item), sum_w + get_itemweight(item));
    }

    // 使った場合の結果と使わなかった場合の結果を比較して返す
    return (r0.value > r1.value) ? r0 : r1;
}
