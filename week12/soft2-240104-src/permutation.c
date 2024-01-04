#include <stdio.h>
#include <stdlib.h>

void permutation(int *pattern, int *used, size_t number);

int main(int argc, char **argv)
{
    size_t number = 6;

    int *pattern = (int *)calloc(number, sizeof(int));
    int *used = (int *)calloc(number, sizeof(int));

    for (int i = 0; i < number; i++)
    {
        // まだパターンが確定してないことのフラグを立てておく
        pattern[i] = -1;
        used[i] = 0;
    }

    permutation(pattern, used, number);

    free(used);
    free(pattern);

    return 0;
}

const char dict[] = {'I', 'T', 'C', 'L', 'M', 'S'};

void permutation(int *pattern, int *used, size_t number)
{
    int start = -1;

    // patternをfor文で確認し，未確定の最初のインデックスをstartに入れる

    /* ここにコードをかく */
    for (int i = 0; i < number; ++i)
    {
        if (pattern[i] == -1)
        {
            start = i;
            break;
        }
    }

    // パターンが確定した場合（再帰の終端）
    if (start == -1)
    {
        for (int i = 0; i < number; i++)
        {
            printf("%c", dict[pattern[i] - 1]);
            if (i == 2)
            {
                printf("-");
            }
        }
        printf("\n");
        return;
    }

    // パターンが確定してない場合の処理をを以下にかく
    // used と pattern を更新しながらpermutationを呼び出す

    /* ここにコードをかく */
    for (int i = 0; i < number; ++i)
    {
        if (!used[i])
        {
            pattern[start] = i + 1;
            used[i] = 1;
            permutation(pattern, used, number);
            pattern[start] = -1;
            used[i] = 0;
        }
    }

    // 終了
    return;
}
