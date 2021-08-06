/*
          _____   ____  _____            _____  
    /\   |  __ \ / __ \|  __ \     /\   |  __ \ 
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/ 

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef CSTL_WINDOWS_H
#define CSTL_WINDOWS_H

#include <adorad/core/compilers.h>
#include <adorad/core/misc.h>

// Jason's Mini <Windows.h>
#ifdef CSTL_COMPILER_MSVC
    #define WINAPI      __stdcall
    #define WINAPIV     __cdecl
    #define CALLBACK    __stdcall
    #define MAX_PATH    256
    #define CCHDEVICENAME   32
    #define CCHFORMNAME     32

    typedef unsigned long DWORD;
    typedef int WINBOOL;
    typedef unsigned short WORD;
    // typedef float FLOAT; // conflicts with a TokenKind in Adorad
    typedef int INT;
    typedef unsigned int UINT;
    typedef short SHORT;
    typedef long LONG;
    typedef long long LONG;
    typedef unsigned short USHORT;
    typedef unsigned long ULONG;
    typedef unsigned long long ULONGLONG;

    typedef UINT WPARAM;
    typedef LONG LPARAM;
    typedef LONG LRESULT;

    #ifndef _HRESULT_DEFINED
        #define _HRESULT_DEFINED
        typedef LONG HRESULT
    #endif // _HRESULT_DEFINED

    #ifndef XFree86Server
        typedef unsigned char BYTE;
        typedef WORD ATOM;
        #ifndef __OBJC__
            typedef WINBOOL BOOL;
        #else
            #define BOOL WINBOOL
        #endif // __OBJC__
    #endif // XFree86Server

    typedef void* HGDIOBJ;
    typedef void* HANDLE;
    #define DECLARE_HANDLE(name) \
        typedef HANDLE name

    DECLARE_HANDLE(HLOCAL);
    DECLARE_HANDLE(HGLOBAL);
    DECLARE_HANDLE(GLOBALHANDLE);
    DECLARE_HANDLE(LOCALHANDLE);
    DECLARE_HANDLE(HACCEL);
    DECLARE_HANDLE(HBITMAP);
    DECLARE_HANDLE(HBRUSH);
    DECLARE_HANDLE(HCOLORSPACE);
    DECLARE_HANDLE(HDC);
    DECLARE_HANDLE(HGLRC);
    DECLARE_HANDLE(HDESK);
    DECLARE_HANDLE(HENHMETAFILE);
    DECLARE_HANDLE(HFONT);
    DECLARE_HANDLE(HICON);
    DECLARE_HANDLE(HKEY);
    typedef HKEY *PHKEY;
    DECLARE_HANDLE(HMENU);
    DECLARE_HANDLE(HMETAFILE);
    DECLARE_HANDLE(HINSTANCE);
    typedef HINSTANCE HMODULE;
    DECLARE_HANDLE(HPALETTE);
    DECLARE_HANDLE(HPEN);
    DECLARE_HANDLE(HRGN);
    DECLARE_HANDLE(HRSRC);
    DECLARE_HANDLE(HSTR);
    DECLARE_HANDLE(HTASK);
    DECLARE_HANDLE(HWND);
    DECLARE_HANDLE(HWINSTA);
    DECLARE_HANDLE(HKL);
    DECLARE_HANDLE(HRAWINPUT);
    DECLARE_HANDLE(HMONITOR);
    #undef DECLARE_HANDLE
#endif // CSTL_COMPILER_MSVC

#endif // CSTL_WINDOWS_H