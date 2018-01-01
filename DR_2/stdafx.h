// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN   
#define NOMINMAX
// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "ChiliWin.h"
#include <wincodec.h>
//#pragma comment(lib, "windowscodec.lib")
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <wchar.h>
#include <math.h>
#include <vector>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <algorithm>
#include "stdio.h"
#include <stdlib.h> 
#include <heapapi.h>
#include <bitset>
#include <array>
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <unordered_map>
// user defined
#include "RandGen.h"
#include "errors.h"
#include "Iter.h"


#define MAX_MAP_SIZE 80

using  vstring = std::vector<std::string>;
using  vint = std::vector<int>;
using  vUnit = std::vector<class Unit*>;




template<typename T,typename ARR>
static std::size_t ElementCount(ARR& arr)
{
	return (std::size_t)(sizeof(arr) / sizeof(T));
}
template<typename T>
static T Sign(T val)
{
	if (val == (T)0)
		return (T)0;
	return std::signbit((float)val) ? (T)-1 : (T)1;
}


template<class Interface>
inline void SafeDelete(Interface **ppInterfaceToFree)
{
	if (*ppInterfaceToFree != NULL)
	{
		delete (*ppInterfaceToFree);

		(*ppInterfaceToFree) = NULL;
	}
}
template<class Interface>
inline void SafeDeleteArray(Interface **ppInterfaceToFree)
{
	if (*ppInterfaceToFree != NULL)
	{
		delete[](*ppInterfaceToFree);

		(*ppInterfaceToFree) = NULL;
	}
}


inline static bool LittleEndian()
{
	int i = 1;
	// get address of i
	char * c = (char*)&i;
	// if it reads the lower bit first ie: i = 1
	return ((int)*c == i);
}

template <typename Type, typename... TArgs>
Type* MakeNew(TArgs&&... mArgs)
{
	Type* obj(new Type(std::forward<TArgs>(mArgs)...));
	return std::move(obj);

}
template <typename Type, typename... TArgs>
std::unique_ptr<Type> MakeUnique(TArgs&&... mArgs)
{
	Type* obj(new Type(std::forward<TArgs>(mArgs)...));
	std::unique_ptr<Type> uPtr{ obj };
	return std::move(uPtr);

}
template <typename T>
inline T sq(T val)
{
	return val * val;
}
template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

///////////////////////////////////////////////////////////
// Image Tages
#define LEVEL1_SPRITES 0


//////////////////////////////////////////////////////////



#ifndef D2_MAT3x2_IDENTITY
#define D2_MATRIX_IDENTITY(m) { (m) = (D2D1::Matrix3x2F::Identity()); }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

template <typename T>
static void delete_ptr(T*  ptr)
{
	SAFE_DELETE(ptr);
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

// TODO: reference additional headers your program requires here
