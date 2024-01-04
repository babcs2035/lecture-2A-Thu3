#include <stdio.h>
#include <stdlib.h>
#include "city.h"
#include "map.h"
#include "solve.h"

Answer search(const City *city, int n, int pos, int *visited)
{
    Answer res;
    res.distance = 1123456789;
    res.route = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i)
    {
        res.route[i] = -1;
    }

    if (!visited[1] && visited[2])
    {
        return res;
    }

    int flag = 0;
    for (int i = 0; i < n; ++i)
    {
        if (!visited[i])
        {
            visited[i] = 1;
            const Answer r = search(city, n, i, visited);
            visited[i] = 0;
            const int d = distance(get_city(city, pos), get_city(city, i));
            if (r.distance + d < res.distance)
            {
                res.distance = r.distance + d;
                res.route[0] = pos;
                for (int j = 0; r.route[j] != -1; ++j)
                {
                    res.route[j + 1] = r.route[j];
                }
            }
            flag = 1;
        }
    }
    if (!flag)
    {
        res.route[0] = pos;
        res.route[1] = 0;
        res.distance = distance(get_city(city, pos), get_city(city, 0));
    }
    return res;
}

double solve(const City *city, int n, int *route, int *visited)
{
    // 以下はとりあえずダミー。ここに探索プログラムを実装する
    // 現状は町の番号順のルートを回っているだけ
    // 実際は再帰的に探索して、組み合わせが膨大になる。
    route[0] = 0; // 循環した結果を避けるため、常に0番目からスタート
    visited[0] = 1;

    const Answer ans = search(city, n, 0, visited);

    for (int i = 0; i < n; ++i)
    {
        route[i] = ans.route[i];
    }
    return ans.distance;
}
