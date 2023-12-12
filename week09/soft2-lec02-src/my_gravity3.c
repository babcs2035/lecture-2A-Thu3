#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "my_gravity3.h" // この中に構造体定義、関数プロトタイプがある

int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));

    const Condition cond =
        {
            .width = 75,
            .height = 40,
            .G = 1.0,
            .dt = 1.0};

    if (argc != 3)
    {
        return EXIT_FAILURE;
    }
    size_t objnum = atoi(argv[1]);
    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL)
    {
        printf("file open error\n");
        return EXIT_FAILURE;
    }

    Object objects[objnum];
    const size_t buff_size = 1000;
    char buff[buff_size];
    int index = 0;
    while (fgets(buff, buff_size, fp) != NULL)
    {
        if (buff[0] == '#')
        {
            continue;
        }
        sscanf(buff, "%lf%lf%lf%lf%lf", &objects[index].m, &objects[index].x, &objects[index].y, &objects[index].vx, &objects[index].vy);
        objects[index].flag = 0;
        ++index;
    }
    for (int i = 0; i < objnum - index + 1; ++i)
    {
        objects[index + i].m = 0;
        objects[index + i].x = (double)(rand() - RAND_MAX / 2) / RAND_MAX * cond.height / 2;
        objects[index + i].y = (double)(rand() - RAND_MAX / 2) / RAND_MAX * cond.height / 2;
        objects[index + i].vx = (double)(rand() - RAND_MAX / 2) / RAND_MAX;
        objects[index + i].vy = (double)(rand() - RAND_MAX / 2) / RAND_MAX;
        objects[index + i].flag = 0;
    }
    for (int i = 0; i < objnum; ++i)
    {
        printf("%lf %lf %lf %lf %lf\n", objects[i].m, objects[i].x, objects[i].y, objects[i].vx, objects[i].vy);
    }

    // シミュレーション. ループは整数で回しつつ、実数時間も更新する
    const double stop_time = 400;
    double t = 0;
    for (size_t i = 0; t <= stop_time; i++)
    {
        t = i * cond.dt;
        my_update_velocities(objects, objnum, cond);
        my_update_positions(objects, objnum, cond);

        // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
        my_plot_objects(objects, objnum, t, cond);

        usleep(200 * 1000);                // 200 x 1000us = 200 ms ずつ停止
        printf("\e[%dA", cond.height + 2); // 表示位置を巻き戻す。壁がないのでheight+2行（境界とパラメータ表示分）
    }
    return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的には physics.h 内の、こちらが用意したプロトタイプをコメントアウト
void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond)
{
    int ys[numobj], xs[numobj];
    for (int i = 0; i < numobj; ++i)
    {
        const int x = objs[i].x + cond.width / 2;
        const int y = objs[i].y + cond.height / 2;
        xs[i] = x;
        ys[i] = y;
    }
    for (int h = 0; h < cond.height; ++h)
    {
        for (int w = 0; w < cond.width; ++w)
        {
            int flag = 0;
            for (int i = 0; i < numobj; ++i)
            {
                if (objs[i].flag)
                {
                    continue;
                }
                if (ys[i] == h && xs[i] == w)
                {
                    printf("%c", 'a' + i);
                    flag = 1;
                }
            }
            if (!flag)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("-----\n");
    printf("t = %3.1f", t);
    for (int i = 0; i < numobj; ++i)
    {
        if (objs[i].flag)
        {
            printf(", objs[%d] = (merged)", i);
        }
        else
        {
            printf(", objs[%d] = (%3.1f, %3.1f)", i, objs[i].x, objs[i].y);
        }
    }
    printf("\n");
}

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond)
{
    for (int i = 0; i < numobj; ++i)
    {
        if (objs[i].flag)
        {
            continue;
        }

        double ax = 0, ay = 0;
        for (int j = 0; j < numobj; ++j)
        {
            if (i == j || objs[j].flag)
            {
                continue;
            }
            const double dx = objs[j].x - objs[i].x;
            const double dy = objs[j].y - objs[i].y;
            const double dr_abs = sqrt(dx * dx + dy * dy);
            ax += objs[j].m * dx / (dr_abs * dr_abs * dr_abs);
            ay += objs[j].m * dy / (dr_abs * dr_abs * dr_abs);
        }
        ax *= cond.G;
        ay *= cond.G;

        const double temp_x = objs[i].vx;
        const double temp_y = objs[i].vy;
        objs[i].vx = objs[i].prev_vx + ax * cond.dt;
        objs[i].vy = objs[i].prev_vy + ay * cond.dt;
        objs[i].prev_vx = temp_x;
        objs[i].prev_vy = temp_y;
    }
}

const double merge_breakpoint = 5.;
void my_update_positions(Object objs[], const size_t numobj, const Condition cond)
{
    for (int i = 0; i < numobj; ++i)
    {
        if (objs[i].flag)
        {
            continue;
        }

        objs[i].x += objs[i].vx * cond.dt;
        objs[i].y += objs[i].vy * cond.dt;

        for (int j = 0; j < i; ++j)
        {
            const double dx = objs[j].x - objs[i].x;
            const double dy = objs[j].y - objs[i].y;
            const double dr_abs = sqrt(dx * dx + dy * dy);
            if (dr_abs < merge_breakpoint && objs[j].m > 0)
            {
                const double p1_x = objs[i].m * objs[i].vx;
                const double p1_y = objs[i].m * objs[i].vy;
                const double p2_x = objs[j].m * objs[j].vx;
                const double p2_y = objs[j].m * objs[j].vy;
                objs[j].vx = (p1_x + p2_x) / objs[j].m;
                objs[j].vy = (p1_y + p2_y) / objs[j].m;
                objs[j].m += objs[i].m;
                objs[i].m = 0;
                objs[i].x = objs[j].x;
                objs[i].vx = 0;
                objs[i].prev_vx = 0;
                objs[i].y = objs[j].y;
                objs[i].vy = 0;
                objs[i].prev_vy = 0;
                objs[i].flag = 1;
            }
        }
    }
}
