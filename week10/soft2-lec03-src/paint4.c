// コマンドラインペイントプログラム

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h> // for error catch
#include <math.h>

// Structure for canvas
typedef struct
{
    int width;
    int height;
    int **canvas;
    int pen;
} Canvas;

// 最大履歴と現在位置の情報は持たない
typedef struct command Command;
struct command
{
    char *str;
    size_t bufsize;
    Command *next;
};

// コマンドリストの先頭へのポインタをメンバに持つ構造体としてHistoryを考える。
// 履歴がない時点ではbegin = NULL となる。
typedef struct
{
    Command *begin;
    size_t bufsize;
} History;

// functions for Canvas type
Canvas *init_canvas(int width, int height, int pen);
void reset_canvas(Canvas *c);
void print_canvas(Canvas *c);
void free_canvas(Canvas *c);

// display functions
void rewind_screen(unsigned int line);
void clear_command(void);
void clear_screen(void);

// enum for interpret_command results
// interpret_command の結果をより詳細に分割
typedef enum res
{
    EXIT,
    LINE,
    RECT,
    CIRCLE,
    UNDO,
    SAVE,
    OUTPUT,
    LOAD,
    CP,
    CHCOLOR,
    CLEAR,
    UNKNOWN,
    ERRNONINT,
    ERRLACKARGS,
    ERRFILEERROR,
    ERRINVALIDCOLOR,
} Result;
// Result 型に応じて出力するメッセージを返す
char *strresult(Result res);

int max(const int a, const int b);
void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1);
Result interpret_command(const char *command, History *his, Canvas *c);
void save_history(const char *filename, History *his);

size_t get_hsize(History *his)
{
    size_t hsize = 0;
    Command *cmd = his->begin;
    if (cmd == NULL)
        return 0;
    while (cmd->next != NULL)
    {
        cmd = cmd->next;
        ++hsize;
    }
    return hsize + 1;
}

void push_command(History *his, size_t bufsize, const char *buf)
{
    Command *cmd = (Command *)malloc(sizeof(Command));
    cmd->bufsize = bufsize;
    cmd->next = NULL;
    cmd->str = (char *)malloc(sizeof(char *));

    if (his->begin == NULL)
    {
        his->begin = cmd;
    }
    else
    {
        Command *seek = his->begin;
        while (seek->next != NULL)
        {
            seek = seek->next;
        }
        seek->next = cmd;
    }

    strcpy(cmd->str, buf);
}

int main(int argc, char **argv)
{
    // for history recording
    const int bufsize = 1000;
    History *his = (History *)malloc(sizeof(History));
    his->begin = NULL;
    his->bufsize = bufsize;

    int width;
    int height;
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <width> <height>\n", argv[0]);
        return EXIT_FAILURE;
    }
    else
    {
        char *e;
        long w = strtol(argv[1], &e, 10);
        if (*e != '\0')
        {
            fprintf(stderr, "%s: irregular character found %s\n", argv[1], e);
            return EXIT_FAILURE;
        }
        long h = strtol(argv[2], &e, 10);
        if (*e != '\0')
        {
            fprintf(stderr, "%s: irregular character found %s\n", argv[2], e);
            return EXIT_FAILURE;
        }
        width = (int)w;
        height = (int)h;
    }
    int pen = 8;

    char buf[his->bufsize];

    Canvas *c = init_canvas(width, height, pen);

    printf("\n"); // required especially for windows env

    while (1)
    {
        size_t bufsize = his->bufsize;
        print_canvas(c);

        printf("%zu > ", get_hsize(his));
        if (fgets(buf, bufsize, stdin) == NULL)
            break;

        const Result r = interpret_command(buf, his, c);

        if (r == EXIT)
            break;

        // 返ってきた結果に応じてコマンド結果を表示
        clear_command();
        printf("%s\n", strresult(r));
        // LINEの場合はHistory構造体に入れる
        if (r == LINE || r == RECT || r == CIRCLE || r == CHCOLOR || r == CP)
        {
            push_command(his, bufsize, buf);
        }
        rewind_screen(2);          // command results
        clear_command();           // command itself
        rewind_screen(height + 2); // rewind the screen to command input
    }

    clear_screen();
    free_canvas(c);

    return 0;
}

Canvas *init_canvas(int width, int height, int pen)
{
    Canvas *new = (Canvas *)malloc(sizeof(Canvas));
    new->width = width;
    new->height = height;
    new->canvas = (int **)malloc(width * sizeof(int *));

    int *tmp = (int *)malloc(width * height * sizeof(int));
    memset(tmp, -1, width * height * sizeof(int));
    for (int i = 0; i < width; i++)
    {
        new->canvas[i] = tmp + i *height;
    }

    new->pen = pen;
    return new;
}

void reset_canvas(Canvas *c)
{
    const int width = c->width;
    const int height = c->height;
    memset(c->canvas[0], -1, width * height * sizeof(int));
}

void print_canvas(Canvas *c)
{
    const int height = c->height;
    const int width = c->width;
    int **canvas = c->canvas;

    // 上の壁
    printf("+");
    for (int x = 0; x < width; x++)
        printf("--");
    printf("+\n");

    // 外壁と内側
    for (int y = 0; y < height; y++)
    {
        printf("|");
        for (int x = 0; x < width; x++)
        {
            const int c = canvas[x][y];
            if (c == -1)
            {
                printf("\x1b[7m");
            }
            else
            {
                printf("\x1b[%dm", c + 100);
            }
            printf("  ");
            printf("\x1b[0m");
        }
        printf("|\n");
    }

    // 下の壁
    printf("+");
    for (int x = 0; x < width; x++)
        printf("--");
    printf("+\n");
    fflush(stdout);
}

void free_canvas(Canvas *c)
{
    free(c->canvas[0]); //  for 2-D array free
    free(c->canvas);
    free(c);
}

void rewind_screen(unsigned int line)
{
    printf("\e[%dA", line);
}

void clear_command(void)
{
    printf("\e[2K");
}

void clear_screen(void)
{
    printf("\e[2J");
}

int max(const int a, const int b)
{
    return (a > b) ? a : b;
}
void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1)
{
    const int width = c->width;
    const int height = c->height;
    int pen = c->pen;

    const int n = max(abs(x1 - x0), abs(y1 - y0));
    if ((x0 >= 0) && (x0 < width) && (y0 >= 0) && (y0 < height))
        c->canvas[x0][y0] = pen;
    for (int i = 1; i <= n; i++)
    {
        const int x = x0 + i * (x1 - x0) / n;
        const int y = y0 + i * (y1 - y0) / n;
        if ((x >= 0) && (x < width) && (y >= 0) && (y < height))
            c->canvas[x][y] = pen;
    }
}

void save_history(const char *filename, History *his)
{
    const char *default_history_file = "history.txt";
    if (filename == NULL)
        filename = default_history_file;

    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)
    {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return;
    }

    Command *cmd = his->begin;
    while (cmd != NULL)
    {
        fprintf(fp, "%s", cmd->str);
        cmd = cmd->next;
    }

    fclose(fp);
}

int rgbs[9][3] = {
    {102, 102, 102},
    {252, 0, 0},
    {0, 229, 0},
    {227, 237, 0},
    {48, 0, 255},
    {255, 0, 235},
    {0, 236, 234},
    {230, 230, 230},
    {0, 0, 0}};
void output_bmp(Canvas *c)
{
    const int width = c->width;
    const int height = c->height;
    int **canvas = c->canvas;

    FILE *fp;
    if ((fp = fopen("output.bmp", "wb")) == NULL)
    {
        fprintf(stderr, "error: cannot open output.bmp.\n");
        return;
    }

    // ヘッダー情報
    unsigned char header[54] = {
        'B', 'M',    // ファイルタイプ
        0, 0, 0, 0,  // ファイルサイズ
        0, 0,        // 予約領域1
        0, 0,        // 予約領域2
        54, 0, 0, 0, // ファイル先頭から画像データまでのオフセット
        40, 0, 0, 0, // 情報ヘッダサイズ
        0, 0, 0, 0,  // 画像の幅
        0, 0, 0, 0,  // 画像の高さ
        1, 0,        // 色プレーン数
        24, 0,       // 1画素あたりのビット数
        0, 0, 0, 0,  // 圧縮方式
        0, 0, 0, 0,  // 画像データサイズ
        0, 0, 0, 0,  // 水平解像度
        0, 0, 0, 0,  // 垂直解像度
        0, 0, 0, 0,  // 色数
        0, 0, 0, 0,  // 重要な色数
    };

    // ファイルサイズ
    const int file_size = 54 + 3 * width * height;
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    // 画像の幅
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    // 画像の高さ
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    fwrite(header, sizeof(unsigned char), 54, fp);

    // 画像データ
    unsigned char color[3];
    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = 0; x < width * 3; x += 3)
        {
            if (canvas[x / 3][y] == -1)
            {
                color[2] = 255;
                color[1] = 255;
                color[0] = 255;
            }
            else
            {
                color[2] = (unsigned char)rgbs[canvas[x / 3][y]][0];
                color[1] = (unsigned char)rgbs[canvas[x / 3][y]][1];
                color[0] = (unsigned char)rgbs[canvas[x / 3][y]][2];
            }
            fwrite(color, sizeof(unsigned char), 3, fp);
        }
        if (width * 3 % 4 != 0)
        {
            unsigned char tmp[4] = {0};
            fwrite(tmp, sizeof(unsigned char), 4 - width * 3 % 4, fp);
        }
    }

    fclose(fp);
}

Result interpret_command(const char *command, History *his, Canvas *c)
{
    char buf[his->bufsize];
    strcpy(buf, command);
    buf[strlen(buf) - 1] = 0; // remove the newline character at the end

    const char *s = strtok(buf, " ");
    if (s == NULL)
    { // 改行だけ入力された場合
        return UNKNOWN;
    }
    // The first token corresponds to command
    if (strcmp(s, "line") == 0)
    {
        int p[4] = {0}; // p[0]: x0, p[1]: y0, p[2]: x1, p[3]: x1
        char *b[4];
        for (int i = 0; i < 4; i++)
        {
            b[i] = strtok(NULL, " ");
            if (b[i] == NULL)
            {
                return ERRLACKARGS;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            char *e;
            long v = strtol(b[i], &e, 10);
            if (*e != '\0')
            {
                return ERRNONINT;
            }
            p[i] = (int)v;
        }

        draw_line(c, p[0], p[1], p[2], p[3]);
        return LINE;
    }

    if (strcmp(s, "rect") == 0)
    {
        int p[4] = {0}; // p[0]: x0, p[1]: y0, p[2]: width, p[3]: height
        char *b[4];
        for (int i = 0; i < 4; i++)
        {
            b[i] = strtok(NULL, " ");
            if (b[i] == NULL)
            {
                return ERRLACKARGS;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            char *e;
            long v = strtol(b[i], &e, 10);
            if (*e != '\0')
            {
                return ERRNONINT;
            }
            p[i] = (int)v;
        }

        --p[2];
        --p[3];

        char *bb;
        bb = strtok(NULL, " ");
        if (bb != NULL && strcmp(bb, "fill") == 0)
        {
            for (int y = p[1]; y <= p[1] + p[3]; ++y)
            {
                draw_line(c, p[0], y, p[0] + p[2], y);
            }
        }
        else
        {
            draw_line(c, p[0], p[1], p[0] + p[2], p[1]);
            draw_line(c, p[0], p[1], p[0], p[1] + p[3]);
            draw_line(c, p[0] + p[2], p[1], p[0] + p[2], p[1] + p[3]);
            draw_line(c, p[0], p[1] + p[3], p[0] + p[2], p[1] + p[3]);
        }
        return RECT;
    }

    if (strcmp(s, "circle") == 0)
    {
        int p[4] = {0}; // p[0]: x0, p[1]: y0, p[2]: r
        char *b[4];
        for (int i = 0; i < 3; i++)
        {
            b[i] = strtok(NULL, " ");
            if (b[i] == NULL)
            {
                return ERRLACKARGS;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            char *e;
            long v = strtol(b[i], &e, 10);
            if (*e != '\0')
            {
                return ERRNONINT;
            }
            p[i] = (int)v;
        }

        char *bb;
        bb = strtok(NULL, " ");
        if (bb != NULL && strcmp(bb, "fill") == 0)
        {
            for (int x = p[0] - p[2]; x <= p[0] + p[2]; ++x)
            {
                const int dx = x - p[0];
                int dy = sqrt(p[2] * p[2] - dx * dx);
                draw_line(c, x, p[1] - dy, x, p[1] + dy);
            }
            for (int y = p[1] - p[2]; y <= p[1] + p[2]; ++y)
            {
                const int dy = y - p[1];
                int dx = sqrt(p[2] * p[2] - dy * dy);
                draw_line(c, p[0] - dx, y, p[0] + dx, y);
            }
        }
        else
        {
            for (int x = p[0] - p[2]; x <= p[0] + p[2]; ++x)
            {
                const int dx = x - p[0];
                int dy = sqrt(p[2] * p[2] - dx * dx);
                draw_line(c, x, p[1] - dy, x, p[1] - dy);
                draw_line(c, x, p[1] + dy, x, p[1] + dy);
            }
            for (int y = p[1] - p[2]; y <= p[1] + p[2]; ++y)
            {
                const int dy = y - p[1];
                int dx = sqrt(p[2] * p[2] - dy * dy);
                draw_line(c, p[0] - dx, y, p[0] - dx, y);
                draw_line(c, p[0] + dx, y, p[0] + dx, y);
            }
        }

        return CIRCLE;
    }

    if (strcmp(s, "save") == 0)
    {
        s = strtok(NULL, " ");
        save_history(s, his);
        return SAVE;
    }

    if (strcmp(s, "output") == 0)
    {
        output_bmp(c);
        return OUTPUT;
    }

    if (strcmp(s, "load") == 0)
    {
        char *filename = strtok(NULL, " ");
        if (filename == NULL)
        {
            filename = "history.txt";
        }
        FILE *fp = fopen(filename, "r");
        if (fp == NULL)
        {
            return ERRFILEERROR;
        }

        reset_canvas(c);
        his->begin = NULL;

        size_t bufsize = 100;
        char buf[bufsize];
        c->pen = '*';
        while (fgets(buf, bufsize, fp) != NULL)
        {
            interpret_command(buf, his, c);
            push_command(his, bufsize, buf);
        }
        return LOAD;
    }

    if (strcmp(s, "cp") == 0)
    {
        int p[6] = {0}; // p[0]: x0, p[1]: y0, p[2]: width, p[3]: height, p[4]: x1, p[5]: y1
        char *b[6];
        for (int i = 0; i < 6; i++)
        {
            b[i] = strtok(NULL, " ");
            if (b[i] == NULL)
            {
                return ERRLACKARGS;
            }
        }
        for (int i = 0; i < 6; i++)
        {
            char *e;
            long v = strtol(b[i], &e, 10);
            if (*e != '\0')
            {
                return ERRNONINT;
            }
            p[i] = (int)v;
        }

        --p[2];
        --p[3];

        int tmp[p[3] + 1][p[2] + 1];
        for (int y = p[1]; y <= p[1] + p[3]; ++y)
        {
            for (int x = p[0]; x <= p[0] + p[2]; ++x)
            {
                if ((x >= 0) && (x < c->width) && (y >= 0) && (y < c->height))
                {
                    tmp[y - p[1]][x - p[0]] = c->canvas[y][x];
                }
            }
        }
        for (int y = 0; y <= p[3]; ++y)
        {
            for (int x = 0; x <= p[2]; ++x)
            {
                if ((p[4] + x >= 0) && (p[4] + x < c->width) && (p[5] + y >= 0) && (p[5] + y < c->height))
                {
                    c->canvas[p[5] + y][p[4] + x] = tmp[y][x];
                }
            }
        }
        return CP;
    }

    if (strcmp(s, "chcolor") == 0)
    {
        char *new_pen = strtok(NULL, " ");
        if (new_pen == NULL || strlen(new_pen) != 1)
        {
            return ERRINVALIDCOLOR;
        }
        const int clr = (*new_pen) - '0';
        if (clr < 0 || 8 < clr)
        {
            return ERRINVALIDCOLOR;
        }
        c->pen = clr;
        return CHCOLOR;
    }

    if (strcmp(s, "undo") == 0)
    {
        reset_canvas(c);
        if (get_hsize(his) != 0)
        {
            c->pen = '*';
            Command *cmd = his->begin, *prev_cmd = NULL;
            const size_t hsize = get_hsize(his);
            for (int i = 0; i < hsize - 1; i++)
            {
                interpret_command(cmd->str, his, c);
                prev_cmd = cmd;
                cmd = cmd->next;
            }

            free(cmd->str);
            free(cmd);
            if (hsize == 1)
            {
                his->begin = NULL;
            }
            else
            {
                prev_cmd->next = NULL;
            }
        }
        return UNDO;
    }

    if (strcmp(s, "clear") == 0)
    {
        reset_canvas(c);
        his->begin = NULL;
        return CLEAR;
    }

    if (strcmp(s, "quit") == 0)
    {
        return EXIT;
    }

    return UNKNOWN;
}

char *strresult(Result res)
{
    switch (res)
    {
    case EXIT:
        break;
    case LOAD:
        return "history loaded";
    case SAVE:
        return "history saved";
    case OUTPUT:
        return "image saved";
    case LINE:
        return "1 line drawn";
    case RECT:
        return "1 rect drawn";
    case CIRCLE:
        return "1 circle drawn";
    case CHCOLOR:
        return "color changed";
    case CP:
        return "copied & pasted";
    case UNDO:
        return "undo!";
    case CLEAR:
        return "cleared";
    case UNKNOWN:
        return "error: unknown command";
    case ERRNONINT:
        return "Non-int value is included";
    case ERRLACKARGS:
        return "Too few arguments";
    case ERRFILEERROR:
        return "File error";
    case ERRINVALIDCOLOR:
        return "Invalid color";
    }
    return NULL;
}
