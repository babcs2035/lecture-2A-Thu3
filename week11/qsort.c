/*
 * 標準ライブラリ関数qsortを使ってみる
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  // strtol のエラー判定用
#include <string.h> // strerror(errno) のため

// qsort は、比較用に二つのconst voidポインタをとり、int で正負 (1 / -1には限定されない) か0を返す関数へのポインタを引数にとる
// 2引数が指すデータの大小関係を比較しそれによって正負か0（=等しい）かを返す関数を用途に応じて設計する
// 以下では、わかりやすくするために型変換を1行ずつ実行している

// 上記の関数は以下のように書いてもよい
// （実際には正負 と 0 の情報だけを用いているため）
/*
  int comp_int(const void *x0, const void *x1)
  {
  return *(int*)x0 - *(int*)x1;
  }
*/

/* strtol と エラー判定を一つの関数に入れておく*/
