#include <stdio.h>

typedef struct student
{
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} Student;

typedef struct tagged_student1
{
    int id;
    char name[100];
    int age;
    double height;
    double weight;
    char tag;
} TStudent1;

typedef struct tagged_student2
{
    char tag;
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} TStudent2;

int main(int argc, char **argv)
{
    Student s_begin;
    Student s1;
    TStudent1 s2;
    TStudent2 s3;
    TStudent2 s_end;

    // /* 以下に各構造体の先頭アドレスを表示するプログラムを書く */
    // /* Hint: printf で %p フォーマットを指定する*/
    // /* 逆順に表示（s_end, s3, s2, s1, s_begin の順）*/

    // // do_something!!
    // printf("%p, %p, %p,%p,%p\n", &s_end, &s3, &s2, &s1, &s_begin);

    // /* 以下には各構造体のサイズをsizeof演算子で計算し、表示する */
    // /* printf 表示には%zu を用いる*/

    // // do_something!!
    // printf("%zu\n", sizeof(Student));
    // printf("%zu\n", sizeof(TStudent1));
    // printf("%zu\n", sizeof(TStudent2));

    printf("Student:\n");
    printf("\tid:\t%p\n", &s1.id);
    printf("\tname:\t%p\n", &s1.name);
    printf("\tage:\t%p\n", &s1.age);
    printf("\theight:\t%p\n", &s1.height);
    printf("\tweight:\t%p\n", &s1.weight);

    printf("TStudent1:\n");
    printf("\tid:\t%p\n", &s2.id);
    printf("\tname:\t%p\n", &s2.name);
    printf("\tage:\t%p\n", &s2.age);
    printf("\theight:\t%p\n", &s2.height);
    printf("\tweight:\t%p\n", &s2.weight);
    printf("\ttag:\t%p\n", &s2.tag);

    printf("TStudent2:\n");
    printf("\ttag:\t%p\n", &s3.tag);
    printf("\tid:\t%p\n", &s3.id);
    printf("\tname:\t%p\n", &s3.name);
    printf("\tage:\t%p\n", &s3.age);
    printf("\theight:\t%p\n", &s3.height);
    printf("\tweight:\t%p\n", &s3.weight);

    return 0;
}
