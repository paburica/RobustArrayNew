//------------------------------------------------------------------------------------
// Summary        ：RobustArrayNew.hの動作サンプル
// 
//                  The MIT License (MIT)
// 
//                  Copyright (C) 2014, paburica.
// 
//                  Permission is hereby granted, free of charge, to any person obtaining a copy
//                  of this software and associated documentation files (the "Software"), to deal
//                  in the Software without restriction, including without limitation the rights
//                  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//                  copies of the Software, and to permit persons to whom the Software is
//                  furnished to do so, subject to the following conditions:
//                  
//                  The above copyright notice and this permission notice shall be included in
//                  all copies or substantial portions of the Software.
//                  
//                  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//                  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//                  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//                  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//                  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//                  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//                  THE SOFTWARE.
// 
// 
// History        ：
//                  2014/11/02 FileVer 1.0.0.0      新規作成       paburica(paburica@gmail.com)
//                  ◇この行の上に履歴を追加します◇
//                  履歴の追加はAddNewHistoryか、コピー&ペーストで行って下さい。
// 
// Remarks        ：
//   RobustArrayNew.hを使用すると、newで確保した動的配列をオーバーしてアクセス（読み取りでも）すると例外が発生します。
//   そのままデバッグすることで、原因特定が非常に容易となります。
//   RobustArrayNew.hとcrtdbgは同時に使用できないため、このサンプルではENABLED_ROBUST_ARRAY_NEWが定義されているかによって
//   動作を切り替えています。
//   このサンプルでは、ENABLED_ROBUST_ARRAY_NEWを、プロジェクトのプロパティで設定しています。
//   crtdbgの動作を確認する場合は、[C/C++] - [コマンドライン]からENABLED_ROBUST_ARRAY_NEWを削除してください。
// 
#include "stdafx.h"
#include <windows.h>	///< MFCを使用する場合は、このインクルードを削除してください
#include <iostream>


// エントリポイント（main, _tmain, WinMain, dllmain, etc...）のあるソースに以下のマクロを定義します
// プロジェクト内に#define new DEBUG_NEWがある場合は、削除してください
#ifdef _DEBUG
#ifdef ENABLED_ROBUST_ARRAY_NEW
	#include "RobustArrayNew.h"		
#else
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	// ENABLED_ROBUST_ARRAY_NEW
#endif	// _DEBUG


//------------------------------------------------------------------------------------
/// <summary>
/// エントリポイント
/// </summary>
/// <param type="int" name="argc">[i] </param>
/// <param type="_TCHAR*" name="argv[]">[i] </param>
/// <returns>結果</returns>
/// <remarks>
/// 
/// History<para/>
/// 2014/11/02 FileVer 1.0.0.0      新規作成       paburica(paburica@gmail.com)<para/>
/// </remarks>
int _tmain(int /*argc*/, _TCHAR* /*argv[]*/)
{
// エントリポイント（main, _tmain, WinMain, dllmain, etc...）先頭付近に以下のマクロを定義します
#ifdef _DEBUG
#ifndef ENABLED_ROBUST_ARRAY_NEW
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif	// ENABLED_ROBUST_ARRAY_NEW
#endif	// _DEBUG

	// 配列外アクセス（読み込み）
	// RobustArrayNewで発見できます
	// 例外が発生するため、そのままJust-In-Timeデバッグで該当コードに飛べます
	{
		std::cout << "@read overrun test start..." << std::endl;
		const size_t size = 10;
		char* c = new char[size];
		for (int i = 0; i <= size; i++)
		{
			std::cout << i << ":" << std::hex << c[i] << std::endl;
		}
		delete[] c;
		std::cout << "@read overrun test end" << std::endl;
	}

	// 配列外アクセス（書き込み）
	// RobustArrayNew, crtdbgで発見できます
	// RobustArrayNewの場合は、例外が発生するため、そのままJust - In - Timeデバッグで該当コードに飛べます
	// crtdbgの場合は、debugview.exeか標準エラー出力でソースと行番号が確認できます
	{
		std::cout << "@write overrun test start..." << std::endl;
		const size_t size = 10;
		char* c = new char[size];
		for (int i = 0; i <= size; i++)
		{
			std::cout << i << std::endl;
			c[i] = 0;
		}
		delete[] c;
		std::cout << "@write overrun test end" << std::endl;
	}

	// メモリリーク
	// crtdbgで発見できます
	// debugview.exeか標準エラー出力でソースと行番号が確認できます
	{
		std::cout << "@memory leak test start..." << std::endl;
		char* c = new char[16];
		std::cout << "@memory leak test end" << std::endl;
	}

	// 二重解放
	// crtdbgで発見できます
	// debugview.exeか標準エラー出力でソースと行番号が確認できます
	// 場合によっては例外が発生するため、そのままJust-In-Timeデバッグで該当コードに飛べます
	{
		std::cout << "@doubel free test start..." << std::endl;
		char* c = new char[32];
		delete[] c;
		delete[] c;
		std::cout << "@doubel free test end" << std::endl;
	}

	return 0;
}

