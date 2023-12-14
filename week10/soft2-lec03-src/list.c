#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const int maxlen = 1000;

typedef struct node Node;
typedef struct list
{
    Node *begin;
} List;

struct node
{
    char *str;
    Node *next;
};

Node *push_front(List list, const char *str)
{
    Node *begin = list.begin;
    Node *p = (Node *)malloc(sizeof(Node));
    char *s = (char *)malloc(strlen(str) + 1);
    strcpy(s, str);

    *p = (Node){.str = s, .next = begin};

    return p;
}

Node *pop_front(List list)
{
    Node *begin = list.begin;
    assert(begin != NULL);
    Node *p = begin->next;

    free(begin->str);
    free(begin);

    return p;
}

Node *push_back(List list, const char *str)
{
    Node *begin = list.begin;
    if (begin == NULL)
    {
        return push_front(list, str);
    }

    Node *p = begin;
    while (p->next != NULL)
    {
        p = p->next;
    }

    Node *q = (Node *)malloc(sizeof(Node));
    char *s = (char *)malloc(strlen(str) + 1);
    strcpy(s, str);

    *q = (Node){.str = s, .next = NULL};
    p->next = q;

    return begin;
}

// Let's try: pop_back の実装
Node *pop_back(List list)
{
    Node *begin = list.begin;
    if (begin == NULL)
    {
        return NULL;
    }

    Node *p = begin, *q;
    while (p->next != NULL)
    {
        q = p;
        p = p->next;
    }
    q->next = NULL;
    free(p->str);
    free(p);

    return begin;
}

Node *remove_all(List list)
{
    Node *begin = list.begin;
    while ((begin = pop_front(list)))
        ; // Repeat pop_front() until the list becomes empty
    return begin;
}

Node *insert(Node *p, const char *str)
{
    if (p == NULL)
    {
        return NULL;
    }

    Node *q = (Node *)malloc(sizeof(Node));
    char *s = (char *)malloc(strlen(str) + 1);
    strcpy(s, str);

    *q = (Node){.str = s, .next = p->next};
    p->next = q;
    return p;
}

int main()
{
    List list = {.begin = NULL};

    char buf[maxlen];
    while (fgets(buf, maxlen, stdin) != NULL)
    {
        // begin = push_front(begin, buf);
        list.begin = push_back(list, buf); // Try this instead of push_front()
    }

    // begin = pop_front(begin); // What will happen if you do this?
    // begin = pop_back(begin);  // What will happen if you do this?

    // begin = remove_all(begin); // What will happen if you do this?

    for (Node *p = list.begin; p != NULL; p = p->next)
    {
        if (strcmp(p->str, "Komagome\n") == 0)
        {
            insert(p, "Sugamo\n");
        }
    }
    push_back(list, "Takanawa-Gateway\n");

    for (const Node *p = list.begin; p != NULL; p = p->next)
    {
        printf("%s", p->str);
    }

    return EXIT_SUCCESS;
}
