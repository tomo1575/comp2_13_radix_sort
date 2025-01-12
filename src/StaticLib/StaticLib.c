#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義

#include <stdio.h>
#include <stdlib.h>

#include "../include/lib_func.h"


// 基数ソート(引数が不適切であればfalseを返す)
// begin: ソートする配列の先頭要素へのポインタ
// end: ソートする配列の末尾要素の次の要素へのポインタ
// radix: 基数
bool radix_sort(item* begin, const item* end, int radix)
{
    if (begin == NULL || end == NULL || begin >= end || radix <= 0) {
        return false; // 不適切な引数の場合
    }

    int size = end - begin;
    item* buffer = (item*)malloc(size * sizeof(item)); // 一時バッファ
    if (buffer == NULL) {
        return false; // メモリ割り当てに失敗
    }

    int max_key = 0;

    // 配列内の最大値を見つける
    for (item* it = begin; it != end; ++it) {
        if (it->key < 0) {
            free(buffer); // メモリ解放
            return false; // 負の値が含まれている場合は不適切
        }
        if (it->key > max_key) {
            max_key = it->key;
        }
    }

    // 各桁に対する処理を実行
    for (int exp = 1; max_key / exp > 0; exp *= radix) {
        int* count = (int*)calloc(radix, sizeof(int)); // カウント配列
        if (count == NULL) {
            free(buffer); // メモリ解放
            return false; // メモリ割り当てに失敗
        }

        // カウント配列の作成
        for (item* it = begin; it != end; ++it) {
            int digit = (it->key / exp) % radix;
            count[digit]++;
        }

        // 累積和の計算
        for (int i = 1; i < radix; i++) {
            count[i] += count[i - 1];
        }

        // 安定した配置を実現
        for (item* it = end - 1; it >= begin; --it) {
            int digit = (it->key / exp) % radix;
            buffer[--count[digit]] = *it;
        }

        // 元の配列にコピー
        for (int i = 0; i < size; ++i) {
            begin[i] = buffer[i];
        }

        free(count); // カウント配列の解放
    }

    free(buffer); // 一時バッファの解放
    return true;
}
