//------------------------------------------------------------------------------------
// Summary        �FRobustArrayNew.h�̓���T���v��
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
// History        �F
//                  2014/11/02 FileVer 1.0.0.0      �V�K�쐬       paburica(paburica@gmail.com)
//                  �����̍s�̏�ɗ�����ǉ����܂���
//                  �����̒ǉ���AddNewHistory���A�R�s�[&�y�[�X�g�ōs���ĉ������B
// 
// Remarks        �F
//   RobustArrayNew.h���g�p����ƁAnew�Ŋm�ۂ������I�z����I�[�o�[���ăA�N�Z�X�i�ǂݎ��ł��j����Ɨ�O���������܂��B
//   ���̂܂܃f�o�b�O���邱�ƂŁA�������肪���ɗe�ՂƂȂ�܂��B
//   RobustArrayNew.h��crtdbg�͓����Ɏg�p�ł��Ȃ����߁A���̃T���v���ł�ENABLED_ROBUST_ARRAY_NEW����`����Ă��邩�ɂ����
//   �����؂�ւ��Ă��܂��B
//   ���̃T���v���ł́AENABLED_ROBUST_ARRAY_NEW���A�v���W�F�N�g�̃v���p�e�B�Őݒ肵�Ă��܂��B
//   crtdbg�̓�����m�F����ꍇ�́A[C/C++] - [�R�}���h���C��]����ENABLED_ROBUST_ARRAY_NEW���폜���Ă��������B
// 
#include "stdafx.h"
#include <windows.h>	///< MFC���g�p����ꍇ�́A���̃C���N���[�h���폜���Ă�������
#include <iostream>


// �G���g���|�C���g�imain, _tmain, WinMain, dllmain, etc...�j�̂���\�[�X�Ɉȉ��̃}�N�����`���܂�
// �v���W�F�N�g����#define new DEBUG_NEW������ꍇ�́A�폜���Ă�������
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
/// �G���g���|�C���g
/// </summary>
/// <param type="int" name="argc">[i] </param>
/// <param type="_TCHAR*" name="argv[]">[i] </param>
/// <returns>����</returns>
/// <remarks>
/// 
/// History<para/>
/// 2014/11/02 FileVer 1.0.0.0      �V�K�쐬       paburica(paburica@gmail.com)<para/>
/// </remarks>
int _tmain(int /*argc*/, _TCHAR* /*argv[]*/)
{
// �G���g���|�C���g�imain, _tmain, WinMain, dllmain, etc...�j�擪�t�߂Ɉȉ��̃}�N�����`���܂�
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

	// �z��O�A�N�Z�X�i�ǂݍ��݁j
	// RobustArrayNew�Ŕ����ł��܂�
	// ��O���������邽�߁A���̂܂�Just-In-Time�f�o�b�O�ŊY���R�[�h�ɔ�ׂ܂�
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

	// �z��O�A�N�Z�X�i�������݁j
	// RobustArrayNew, crtdbg�Ŕ����ł��܂�
	// RobustArrayNew�̏ꍇ�́A��O���������邽�߁A���̂܂�Just - In - Time�f�o�b�O�ŊY���R�[�h�ɔ�ׂ܂�
	// crtdbg�̏ꍇ�́Adebugview.exe���W���G���[�o�͂Ń\�[�X�ƍs�ԍ����m�F�ł��܂�
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

	// ���������[�N
	// crtdbg�Ŕ����ł��܂�
	// debugview.exe���W���G���[�o�͂Ń\�[�X�ƍs�ԍ����m�F�ł��܂�
	{
		std::cout << "@memory leak test start..." << std::endl;
		char* c = new char[16];
		std::cout << "@memory leak test end" << std::endl;
	}

	// ��d���
	// crtdbg�Ŕ����ł��܂�
	// debugview.exe���W���G���[�o�͂Ń\�[�X�ƍs�ԍ����m�F�ł��܂�
	// �ꍇ�ɂ���Ă͗�O���������邽�߁A���̂܂�Just-In-Time�f�o�b�O�ŊY���R�[�h�ɔ�ׂ܂�
	{
		std::cout << "@doubel free test start..." << std::endl;
		char* c = new char[32];
		delete[] c;
		delete[] c;
		std::cout << "@doubel free test end" << std::endl;
	}

	return 0;
}

