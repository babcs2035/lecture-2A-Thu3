#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "my_gravity.h" // この中に構造体定義、関数プロトタイプがある

int main(int argc, char **argv)
{
    const Condition cond =
        {
            .width = 75,
            .height = 40,
            .G = 1.0,
            .dt = 1.0};

    size_t objnum = 2;
    Object objects[objnum];

    // オブジェクトの定義
    // objects[0] = (Object){.m = 60.0, .x = -10.0, .y = -20.0, .vx = 0., .vy = 0.0};
    // objects[1] = (Object){.m = 120.0, .x = 20.0, .y = 10.0, .vx = 0., .vy = 0.0};
    objects[0] = (Object){.m = 100.0, .x = 0.0, .y = 0.0, .vx = 0., .vy = 0.0};
    objects[1] = (Object){.m = 1.0, .x = 20.0, .y = 0.0, .vx = 0., .vy = 3.0};

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
        printf(", objs[%d] = (%3.1f, %3.1f)", i, objs[i].x, objs[i].y);
    }
    printf("\n");
}

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond)
{
    for (int i = 0; i < numobj; ++i)
    {
        double ax = 0, ay = 0;
        for (int j = 0; j < numobj; ++j)
        {
            if (i == j)
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

void my_update_positions(Object objs[], const size_t numobj, const Condition cond)
{
    for (int i = 0; i < numobj; ++i)
    {
        objs[i].x += objs[i].vx * cond.dt;
        objs[i].y += objs[i].vy * cond.dt;
    }
}
