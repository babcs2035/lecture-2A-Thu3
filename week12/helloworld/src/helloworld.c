#include <stdio.h>

int main(void)
{
    float f[4];

    // 以下に適切な値を設定する
    f[0] = 6581362.000000;
    f[1] = 0.;
    f[2] = 0.;
    f[3] = 0.;

    // "Hello,World\n" と出力してほしい
    printf("%s", (char *)f);

    // f[0] をいじる
    f[0] += 1143141483620823940762435584. - 1143139122437582505939828736.;

    // "hello,World\n" と出力してほしい
    printf("%s", (char *)f);

    return 0;
}
