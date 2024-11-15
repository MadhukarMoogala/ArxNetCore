// (C) Copyright 2024 by Autodesk, Inc.
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS.
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//- StdAfx.h : include file for standard system include files,
//-      or project specific include files that are used frequently,
//-      but are changed infrequently
//-----------------------------------------------------------------------------

//Madhukar Moogala @ Autodesk


#pragma once
#define ARXNETCORE_MODULE


#ifndef _ALLOW_RTCc_IN_STL
#define _ALLOW_RTCc_IN_STL
#endif


#pragma pack(push, 8)
#pragma warning(disable : 4786 4996)
    //#pragma warning(disable: 4098)

    //-----------------------------------------------------------------------------
#include <windows.h>

    //- ObjectARX and OMF headers needs this
#include <map>
// .NET assmeblies for System
#using < mscorlib.dll>
#using < System.dll>

//.NET assemblies for AutoCAD
#using < acdbmgd.dll>
#using < acmgd.dll>
#using < accoremgd.dll >
#using < AcCui.dll>
            
#include <vcclr.h>
#include "arxHeaders.h"
#pragma pack(pop)

    