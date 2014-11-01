//------------------------------------------------------------------------------------
// Summary        ：newで確保した動的配列外アクセス（読み取り/書き込みとも）検出
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
//   RobustArrayNew.hをincludeすると、newで確保した動的配列外アクセス（読み取り/書き込みとも）により例外が発生します。
//   そのままそのままJust-In-Timeデバッグすることで、原因特定が非常に容易となります。
//   Microsoftの提供するcrtdbg機能を使用すると、解放漏れ、二重解放、newで確保した動的配列をオーバーしてアクセス（書き込みのみ）を検出します。
//   RobustArrayNewとcrtdbgは同時に使用できませんが、この2つの機構を使い分けることで、メモリ関係の問題の大多数を特定できます。
//
//   注意：
//   RobustArrayNewはあくまで、バグ調査に使用するものなので非常に非効率かつ危険です。
//   この機能を埋め込んでリリースしてはいけません。
//   静的配列の配列外アクセスは、RobustArrayNew, crtdbgのいずれでも検出できません。
// 
#pragma once

#include <iostream>


namespace RobustArrayNew{
	const size_t pageSize = 1024 * 4;				///< ページサイズ
	const size_t sizeAllocSize = sizeof(size_t);	///< サイズ領域サイズ
	const size_t guardAllocSize = pageSize;			///< ガード領域サイズ
};


//------------------------------------------------------------------------------------
/// <summary>
/// 動的配列外アクセス（読み取り/書き込みとも）を検出する配列new
/// </summary>
/// <param type="size_t" name="size">[i] 確保するバイトサイズ</param>
/// <returns>確保したアドレス</returns>
/// <remarks>
/// 仕組み：<para/>
/// Windowsはページ単位でメモリの属性を変更出来るため<para/>
/// [詰め物領域] + [サイズ領域] + [実領域] + [ガード領域]という構成のメモリを確保します。<para/>
/// [詰め物領域] + [サイズ領域] + [実領域]を、ページサイズ境界に合わせます。<para/>
/// [ガード領域]を読み取り/書き込み不可にします。<para/>
/// こうすることで、[実領域]を超えてアクセスすると例外が発生するようになります。<para/>
/// <para/>
/// 補足：<para/>
/// 例外は、構造化例外でcatchすることができます。<para/>
/// ただし、一度例外を発生させた[ガード領域]の読み取り/書き込み不可属性はなくなるため<para/>
/// catchするメリットはありません。<para/>
/// <para/>
/// History<para/>
/// 2014/11/02 FileVer 1.0.0.0      新規作成       paburica(paburica@gmail.com)<para/>
/// </remarks>
void* operator new[](size_t size)
{
	if (size == 0) size = 1;
	
	const size_t controlSize = RobustArrayNew::sizeAllocSize + size;
	const size_t paddingSize = (controlSize >= RobustArrayNew::pageSize)? RobustArrayNew::pageSize - (controlSize % RobustArrayNew::pageSize) : RobustArrayNew::pageSize - controlSize;
	const size_t realSize = paddingSize + controlSize + RobustArrayNew::guardAllocSize;
	//std::cout << "size = " << std::dec << size << ", 0x" << std::hex << size << std::endl;
	//std::cout << "realSize = " << std::dec << realSize << ", 0x" << std::hex << realSize << std::endl;
	
#pragma warning(push)
#pragma warning(disable : 4127)
	while(true)
#pragma warning(pop) 
	{
		// 詰め物領域 + サイズ領域 + 実領域 + ガード領域を、末尾がページサイズ境界になるように確保
		void* pBase = ::VirtualAlloc(NULL, realSize, MEM_COMMIT, PAGE_READWRITE);
		if (pBase == NULL)
		{
			std::cout << "failed VirtualAlloc = " << ::GetLastError() << std::endl;
		}
		else
		{
			//std::cout << "success VirtualAlloc = " << std::hex << pBase << std::endl;
			
			// サイズ領域にサイズを記録
			// VirtualFreeはページ単位で解放するので、このサイズには詰め物領域を含めない
			memcpy((static_cast<char*>(pBase) + paddingSize), &controlSize, sizeof(controlSize));
			
			// 後ろをガード
			DWORD old = 0;
			if (::VirtualProtect((static_cast<char*>(pBase)+paddingSize + controlSize),
				RobustArrayNew::guardAllocSize, (PAGE_READONLY | PAGE_GUARD), &old) == 0)
			{
				std::cout << "failed VirtualProtect = " << ::GetLastError() << std::endl;
				::VirtualFree(pBase, realSize, MEM_DECOMMIT);
			}
			else
			{
				return static_cast<char*>(pBase)+paddingSize + RobustArrayNew::sizeAllocSize;
			}
		}
		
		std::new_handler handler = std::set_new_handler(NULL);
		std::set_new_handler(handler);
		if (handler == NULL)
		{
			throw std::bad_alloc();
		}
		handler();
	}
}

//------------------------------------------------------------------------------------
/// <summary>
/// 動的配列外アクセス（読み取り/書き込みとも）を検出する配列newの解放
/// </summary>
/// <param type="void*" name="po">[i/o] 対象アドレス</param>
/// <returns>なし</returns>
/// <remarks>
/// 
/// History<para/>
/// 2014/11/02 FileVer 1.0.0.0      新規作成       paburica(paburica@gmail.com)<para/>
/// </remarks>
void operator delete[](void* po)
{
	if (po != NULL)
	{
		//std::cout << "delete address = " << std::hex << po << std::endl;

		// サイズを取得
		size_t controlSize = 0;
		memcpy(&controlSize, (static_cast<char*>(po)-RobustArrayNew::sizeAllocSize), sizeof(controlSize));

		// 解放
		// ページ単位で解放するので、paddingSizeは不要
		if (::VirtualFree((static_cast<char*>(po)-RobustArrayNew::sizeAllocSize), (controlSize + RobustArrayNew::guardAllocSize), MEM_DECOMMIT) == 0)
		{
			std::cout << "failed VirtualFree = " << ::GetLastError() << std::endl;
		}
	}
}
