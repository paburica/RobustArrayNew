//------------------------------------------------------------------------------------
// Summary        �Fnew�Ŋm�ۂ������I�z��O�A�N�Z�X�i�ǂݎ��/�������݂Ƃ��j���o
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
//   RobustArrayNew.h��include����ƁAnew�Ŋm�ۂ������I�z��O�A�N�Z�X�i�ǂݎ��/�������݂Ƃ��j�ɂ���O���������܂��B
//   ���̂܂܂��̂܂�Just-In-Time�f�o�b�O���邱�ƂŁA�������肪���ɗe�ՂƂȂ�܂��B
//   Microsoft�̒񋟂���crtdbg�@�\���g�p����ƁA����R��A��d����Anew�Ŋm�ۂ������I�z����I�[�o�[���ăA�N�Z�X�i�������݂̂݁j�����o���܂��B
//   RobustArrayNew��crtdbg�͓����Ɏg�p�ł��܂��񂪁A����2�̋@�\���g�������邱�ƂŁA�������֌W�̖��̑命�������ł��܂��B
//
//   ���ӁF
//   RobustArrayNew�͂����܂ŁA�o�O�����Ɏg�p������̂Ȃ̂Ŕ��ɔ�������댯�ł��B
//   ���̋@�\�𖄂ߍ���Ń����[�X���Ă͂����܂���B
//   �ÓI�z��̔z��O�A�N�Z�X�́ARobustArrayNew, crtdbg�̂�����ł����o�ł��܂���B
// 
#pragma once

#include <iostream>


namespace RobustArrayNew{
	const size_t pageSize = 1024 * 4;				///< �y�[�W�T�C�Y
	const size_t sizeAllocSize = sizeof(size_t);	///< �T�C�Y�̈�T�C�Y
	const size_t guardAllocSize = pageSize;			///< �K�[�h�̈�T�C�Y
};


//------------------------------------------------------------------------------------
/// <summary>
/// ���I�z��O�A�N�Z�X�i�ǂݎ��/�������݂Ƃ��j�����o����z��new
/// </summary>
/// <param type="size_t" name="size">[i] �m�ۂ���o�C�g�T�C�Y</param>
/// <returns>�m�ۂ����A�h���X</returns>
/// <remarks>
/// �d�g�݁F<para/>
/// Windows�̓y�[�W�P�ʂŃ������̑�����ύX�o���邽��<para/>
/// [�l�ߕ��̈�] + [�T�C�Y�̈�] + [���̈�] + [�K�[�h�̈�]�Ƃ����\���̃��������m�ۂ��܂��B<para/>
/// [�l�ߕ��̈�] + [�T�C�Y�̈�] + [���̈�]���A�y�[�W�T�C�Y���E�ɍ��킹�܂��B<para/>
/// [�K�[�h�̈�]��ǂݎ��/�������ݕs�ɂ��܂��B<para/>
/// �������邱�ƂŁA[���̈�]�𒴂��ăA�N�Z�X����Ɨ�O����������悤�ɂȂ�܂��B<para/>
/// <para/>
/// �⑫�F<para/>
/// ��O�́A�\������O��catch���邱�Ƃ��ł��܂��B<para/>
/// �������A��x��O�𔭐�������[�K�[�h�̈�]�̓ǂݎ��/�������ݕs�����͂Ȃ��Ȃ邽��<para/>
/// catch���郁���b�g�͂���܂���B<para/>
/// <para/>
/// History<para/>
/// 2014/11/02 FileVer 1.0.0.0      �V�K�쐬       paburica(paburica@gmail.com)<para/>
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
		// �l�ߕ��̈� + �T�C�Y�̈� + ���̈� + �K�[�h�̈���A�������y�[�W�T�C�Y���E�ɂȂ�悤�Ɋm��
		void* pBase = ::VirtualAlloc(NULL, realSize, MEM_COMMIT, PAGE_READWRITE);
		if (pBase == NULL)
		{
			std::cout << "failed VirtualAlloc = " << ::GetLastError() << std::endl;
		}
		else
		{
			//std::cout << "success VirtualAlloc = " << std::hex << pBase << std::endl;
			
			// �T�C�Y�̈�ɃT�C�Y���L�^
			// VirtualFree�̓y�[�W�P�ʂŉ������̂ŁA���̃T�C�Y�ɂ͋l�ߕ��̈���܂߂Ȃ�
			memcpy((static_cast<char*>(pBase) + paddingSize), &controlSize, sizeof(controlSize));
			
			// �����K�[�h
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
/// ���I�z��O�A�N�Z�X�i�ǂݎ��/�������݂Ƃ��j�����o����z��new�̉��
/// </summary>
/// <param type="void*" name="po">[i/o] �ΏۃA�h���X</param>
/// <returns>�Ȃ�</returns>
/// <remarks>
/// 
/// History<para/>
/// 2014/11/02 FileVer 1.0.0.0      �V�K�쐬       paburica(paburica@gmail.com)<para/>
/// </remarks>
void operator delete[](void* po)
{
	if (po != NULL)
	{
		//std::cout << "delete address = " << std::hex << po << std::endl;

		// �T�C�Y���擾
		size_t controlSize = 0;
		memcpy(&controlSize, (static_cast<char*>(po)-RobustArrayNew::sizeAllocSize), sizeof(controlSize));

		// ���
		// �y�[�W�P�ʂŉ������̂ŁApaddingSize�͕s�v
		if (::VirtualFree((static_cast<char*>(po)-RobustArrayNew::sizeAllocSize), (controlSize + RobustArrayNew::guardAllocSize), MEM_DECOMMIT) == 0)
		{
			std::cout << "failed VirtualFree = " << ::GetLastError() << std::endl;
		}
	}
}
