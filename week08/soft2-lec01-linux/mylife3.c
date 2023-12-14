#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int get_rand()
{
    return (int)(100 * (double)rand() / RAND_MAX);
}

void my_init_cells(const int height, const int width, int cell[height][width], FILE *fp)
{
    if (fp == NULL)
    {
        srand((unsigned int)time(NULL));
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; ++y)
            {
                if (get_rand() <= 10)
                {
                    cell[y][x] = 1;
                }
                else
                {
                    cell[y][x] = 0;
                }
            }
        }
    }
    else
    {
        const int buf_size = 100;
        char buf[buf_size];
        int is_rle = 0, ox = 0, oy = 0, sx = 0, sy = 0;
        while (fgets(buf, buf_size, fp) != NULL)
        {
            if (buf[0] == '#')
            {
                continue;
            }
            else if (buf[0] == 'x')
            {
                int x, y;
                sscanf(buf, "x = %d, y = %d", &x, &y);
                ox = width / 2 - x / 2;
                oy = height / 2 - y / 2;
                sx = ox;
                sy = oy;
                is_rle = 1;
            }
            else if (is_rle)
            {
                int seek = 0;
                while (seek < buf_size && buf[seek] != '\n' && buf[seek] != '\0')
                {
                    if ('0' <= buf[seek] && buf[seek] <= '9')
                    {
                        int num = 0;
                        while ('0' <= buf[seek] && buf[seek] <= '9')
                        {
                            num = num * 10 + buf[seek] - '0';
                            ++seek;
                        }
                        for (int c = 0; c < num; c++)
                        {
                            if (buf[seek] == 'b')
                            {
                                cell[sy][sx] = 0;
                            }
                            else if (buf[seek] == 'o')
                            {
                                cell[sy][sx] = 1;
                            }
                            ++sx;
                        }
                    }
                    else if (buf[seek] == '$')
                    {
                        ++sy;
                        sx = ox;
                    }
                    else if (buf[seek] == 'b')
                    {
                        cell[sy][sx] = 0;
                        ++sx;
                    }
                    else if (buf[seek] == 'o')
                    {
                        cell[sy][sx] = 1;
                        ++sx;
                    }
                    seek++;
                }
            }
            else
            {
                int x, y;
                sscanf(buf, "%d%d", &x, &y);
                cell[y][x] = 1;
            }
        }
    }
}

/*
 グリッドの描画: 世代情報とグリッドの配列等を受け取り、ファイルポインタに該当する出力にグリッドを描画する
 */
void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width])
{
    int alive_cnt = 0;
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            alive_cnt += cell[y][x];
        }
    }
    fprintf(fp, "generation = %d, surviving rate = %lf\n", gen, (double)alive_cnt / (height * width));
    fprintf(fp, "+");
    for (int i = 0; i < width; ++i)
    {
        fprintf(fp, "-");
    }
    fprintf(fp, "+\n");
    for (int y = 0; y < height; ++y)
    {
        fprintf(fp, "|");
        for (int x = 0; x < width; ++x)
        {
            if (cell[y][x] == 1)
            {
                fprintf(fp, "\e[31m#\e[0m");
            }
            else
            {
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "|\n");
    }
    fprintf(fp, "+");
    for (int i = 0; i < width; ++i)
    {
        fprintf(fp, "-");
    }
    fprintf(fp, "+\n");
}

/*
 着目するセルの周辺の生きたセルをカウントする関数
 */
int my_count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width])
{
    const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int cnt = 0;
    for (int k = 0; k < 8; ++k)
    {
        const int x = w + dx[k];
        const int y = h + dy[k];
        if (0 <= x && x < width && 0 <= y && y < height && cell[y][x] == 1)
        {
            ++cnt;
        }
    }
    return cnt;
}

/*
 ライフゲームのルールに基づいて2次元配列の状態を更新する
 */
void my_update_cells(const int height, const int width, int cell[height][width])
{
    int temp[height][width];
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            const int cnt = my_count_adjacent_cells(y, x, height, width, cell);
            if (cell[y][x])
            {
                if (cnt == 2 || cnt == 3)
                {
                    temp[y][x] = 1;
                }
                else
                {
                    temp[y][x] = 0;
                }
            }
            else
            {
                if (cnt == 3)
                {
                    temp[y][x] = 1;
                }
                else
                {
                    temp[y][x] = 0;
                }
            }
        }
    }
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            cell[y][x] = temp[y][x];
        }
    }
}

void output_field(FILE *fp, const int height, const int width, int cell[height][width])
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            if (cell[y][x])
            {
                fprintf(fp, "%d %d\n", x, y);
            }
        }
    }
}

int main(int argc, char **argv)
{
    FILE *fp = stdout;
    const int height = 40;
    const int width = 70;

    int cell[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            cell[y][x] = 0;
        }
    }

    if (argc > 2)
    {
        fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        FILE *lgfile;
        if ((lgfile = fopen(argv[1], "r")) != NULL)
        {
            my_init_cells(height, width, cell, lgfile);
        }
        else
        {
            fprintf(stderr, "cannot open file %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        fclose(lgfile);
    }
    else
    {
        my_init_cells(height, width, cell, NULL); // デフォルトの初期値を使う
    }

    my_print_cells(fp, 0, height, width, cell); // 表示する
    sleep(1);                                   // 1秒休止
    fprintf(fp, "\e[%dA", height + 3);          // height+3 の分、カーソルを上に戻す(壁2、表示部1)

    /* 世代を進める*/
    for (int gen = 1;; gen++)
    {
        my_update_cells(height, width, cell);         // セルを更新
        my_print_cells(fp, gen, height, width, cell); // 表示する
        sleep(1);                                     // 1秒休止する
        fprintf(fp, "\e[%dA", height + 3);            // height+3 の分、カーソルを上に戻す(壁2、表示部1)

        if (gen % 100 == 0 && gen < 10000)
        {
            char filename[100];
            sprintf(filename, "gen%04d.lif", gen);
            FILE *fp;
            if ((fp = fopen(filename, "w")) != NULL)
            {
                fprintf(fp, "#Life 1.06\n");
                output_field(fp, height, width, cell);
            }
            fclose(fp);
        }
    }

    return EXIT_SUCCESS;
}
