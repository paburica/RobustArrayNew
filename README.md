# RobustArrayNew
- To detect the dynamic array outside access (read / write both) that was allocated by new
- paburica(paburica@gmail.com)

## How to use
- #include "RobustArrayNew.h" in the source that contains the entry point
- Debug build and run
- Exception will occur when the array outside access. And then, Just-In-Time debug

## Supplement
- When combined with crtdbg, you can detect the majority of memory problem.
- However, crtdbg at the same time can not be used
- For more information open the RobustArrayNew.sln

```C++

	// Is defined in the source code that entry point
	// If there is a #define new DEBUG_NEW is in the project, please remove
	#ifdef _DEBUG
	#ifdef ENABLED_ROBUST_ARRAY_NEW
		#include "RobustArrayNew.h"		
	#else
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#endif	// ENABLED_ROBUST_ARRAY_NEW
	#endif	// _DEBUG
	
	// Is defined near the top of the entry point
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
```

## Directory structure
- .git
	- .git management files
- RobustArrayNew
	- RobustArrayNew.h and sample project

## Operation verification environment
- Visual Studio 2008/2010/2013

## Precaution
- Can't detect static array outside access

## License
The MIT License (MIT)

Copyright (C) 2014, paburica.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.



