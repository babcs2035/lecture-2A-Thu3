#include <stdio.h>
#include <stdlib.h>
#include "city.h"
#include "map.h"
#include "solve.h"

double get_total_distance(const City *city, int n, const int *route)
{
    double res = 0.;
    for (int i = 0; i < n; ++i)
    {
        res += distance(get_city(city, route[i]), get_city(city, route[(i + 1) % n]));
    }
    return res;
}

void set_rand_permutation(int n, int *route)
{
    for (int i = 0; i < n; ++i)
    {
        route[i] = i;
    }
    for (int i = 1; i < n; ++i)
    {
        int j = rand() % n;
        if (j == 0)
        {
            continue;
        }
        int tmp = route[i];
        route[i] = route[j];
        route[j] = tmp;
    }
}

void swap(int *route, int i, int j)
{
    int tmp = route[i];
    route[i] = route[j];
    route[j] = tmp;
}

Answer search(const City *city, int n)
{
    Answer ans;
    ans.distance = 1123456789.;
    ans.route = (int *)malloc(sizeof(int) * n);

    const size_t ans_num = 70;
    int ans_route[ans_num][n];
    for (int i = 0; i < ans_num; ++i)
    {
        printf("[%d]\n", i);
        set_rand_permutation(n, ans_route[i]);
        double res = 1123456789.;
        if (i == 69)
        {
            for (int j = 0; j < n; ++j)
            {
                printf("%d -> ", ans_route[i][j]);
            }
            printf("0\n");
        }
        while (1)
        {
            int route_temp[n];
            for (int j = 0; j < n; ++j)
            {
                route_temp[j] = ans_route[i][j];
            }

            int flag = 0;
            for (int p = 1; p < n - 1; ++p)
            {
                for (int q = p + 1; q < n; ++q)
                {
                    swap(route_temp, p, q);

                    double d = get_total_distance(city, n, route_temp);
                    if (d < res)
                    {
                        res = d;
                        for (int j = 0; j < n; ++j)
                        {
                            ans_route[i][j] = route_temp[j];
                        }
                        flag = 1;
                    }

                    swap(route_temp, p, q);
                }
            }
            printf("%lf\n", res);
            if (!flag)
            {
                break;
            }
        }
        printf("\n");
        if (i == 69)
        {
            for (int j = 0; j < n; ++j)
            {
                printf("%d -> ", ans_route[i][j]);
            }
            printf("0\n");
        }
    }

    for (int i = 0; i < ans_num; ++i)
    {
        double d = get_total_distance(city, n, ans_route[i]);
        if (d < ans.distance)
        {
            ans.distance = d;
            for (int j = 0; j < n; ++j)
            {
                ans.route[j] = ans_route[i][j];
            }
        }
    }

    return ans;
}

double solve(const City *city, int n, int *route)
{
    route[0] = 0;

    const Answer ans = search(city, n);

    for (int i = 0; i < n; ++i)
    {
        route[i] = ans.route[i];
    }
    return ans.distance;
}
