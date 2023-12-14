// コマンドラインペイントプログラム

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h> // for error catch

// Structure for canvas
typedef struct
{
    int width;
    int height;
    char **canvas;
    char pen;
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
Canvas *init_canvas(int width, int height, char pen);
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
    UNDO,
    SAVE,
    UNKNOWN,
    ERRNONINT,
    ERRLACKARGS
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
    char pen = '*';

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
        if (r == LINE)
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
        rewind_screen(2);          // command results
        clear_command();           // command itself
        rewind_screen(height + 2); // rewind the screen to command input
    }

    clear_screen();
    free_canvas(c);

    return 0;
}

Canvas *init_canvas(int width, int height, char pen)
{
    Canvas *new = (Canvas *)malloc(sizeof(Canvas));
    new->width = width;
    new->height = height;
    new->canvas = (char **)malloc(width * sizeof(char *));

    char *tmp = (char *)malloc(width * height * sizeof(char));
    memset(tmp, ' ', width * height * sizeof(char));
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
    memset(c->canvas[0], ' ', width * height * sizeof(char));
}

void print_canvas(Canvas *c)
{
    const int height = c->height;
    const int width = c->width;
    char **canvas = c->canvas;

    // 上の壁
    printf("+");
    for (int x = 0; x < width; x++)
        printf("-");
    printf("+\n");

    // 外壁と内側
    for (int y = 0; y < height; y++)
    {
        printf("|");
        for (int x = 0; x < width; x++)
        {
            const char c = canvas[x][y];
            putchar(c);
        }
        printf("|\n");
    }

    // 下の壁
    printf("+");
    for (int x = 0; x < width; x++)
        printf("-");
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
    char pen = c->pen;

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

    if (strcmp(s, "save") == 0)
    {
        s = strtok(NULL, " ");
        save_history(s, his);
        return SAVE;
    }

    if (strcmp(s, "undo") == 0)
    {
        reset_canvas(c);
        if (get_hsize(his) != 0)
        {
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
    case SAVE:
        return "history saved";
    case LINE:
        return "1 line drawn";
    case UNDO:
        return "undo!";
    case UNKNOWN:
        return "error: unknown command";
    case ERRNONINT:
        return "Non-int value is included";
    case ERRLACKARGS:
        return "Too few arguments";
    }
    return NULL;
}
