# struct_alignment.c の出力に関する考察

## 構造体 `Student` について

`id`, `age` は 4 byte, `name[]` は 1 \* 100 = 100 byte, `height`, `weight` は 8 byte である．
`id`, `name` を合わせて 100 + 4 = 104 byte となり，`name` のメモリ領域の直後にパディングは発生していない．

## 構造体 `tagged_student1` について

`tag` は 1 byte, `id`, `age` は 4 byte, `name[]` は 1 \* 100 = 100 byte, `height`, `weight` は 8 byte である．
`id` から `weight` までの合計は 4 + 100 + 4 + 8 + 8 = 124 byte であり，`tag` まで含めると 125 byte となる．ここで，`tag` のメモリ領域の直後にパディングが 3 byte 発生している．

## 構造体 `tagged_student2` について

`tag` は 1 byte, `id`, `age` は 4 byte, `name[]` は 1 \* 100 = 100 byte, `height`, `weight` は 8 byte である．
`tag` のメモリ領域の直後にパディングが 3 byte 発生している．

