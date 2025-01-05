#pragma once // インクルードガード

#include <stdbool.h>

// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct item_
	{
		unsigned int key;
		char value[256];
	}item;

	// 基数ソート(引数が不適切であればfalseを返す)
	// begin: ソートする配列の先頭要素へのポインタ
	// end: ソートする配列の末尾要素の次の要素へのポインタ
	// radix: 基数
	bool radix_sort(item *begin, const item* end, int radix);

	// C++ でCのライブラリを使うときのおまじない
#ifdef __cplusplus
} // extern "C"
#endif
