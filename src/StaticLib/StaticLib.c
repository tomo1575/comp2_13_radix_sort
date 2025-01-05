#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義

#include "../include/lib_func.h"


// 基数ソート(引数が不適切であればfalseを返す)
// begin: ソートする配列の先頭要素へのポインタ
// end: ソートする配列の末尾要素の次の要素へのポインタ
// radix: 基数
bool radix_sort(item* begin, const item* end, int radix)
{
	// ToDo: 基数ソートを実装する
	return false;
}
