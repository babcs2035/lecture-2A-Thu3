# knapsack について

ナップサック問題を動的計画法を用いて解いた．
以下のコマンドで実行できる．

```bash
$ make knapsack
$ bin/knapsack data/knapsack.in
```

入力ファイルの中身の形式は以下のようになっている．

```
品目の個数 N 重さの上限 W
品目 1 の重さ 品目 1 の価値
品目 2 の重さ 品目 2 の価値
...
```

なお，入力データと実行結果の検証には[こちらのデータ](https://atcoder.jp/contests/abc032/tasks/abc032_d)を用いた．

このアルゴリズムの計算量は O(NW) であり，同封した入力データに対しては十分高速に動作する．

# tsp について

巡回セールスマン問題を bit DP を用いて解いた．
以下のコマンドで実行できる．

```bash
$ make tsp
$ bin/tsp data/tsp.in
```

入力ファイルの中身の形式は以下のようになっている．

```
都市の数
都市 1 の x 座標 都市 1 の y 座標
都市 2 の x 座標 都市 2 の y 座標
...
```

なお，入力データと実行結果の検証には[こちらのデータ](https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_cv)を用いた．

このアルゴリズムの計算量は O(2^N \* N^2) であり，同封した入力データに対しては十分高速に動作する．

