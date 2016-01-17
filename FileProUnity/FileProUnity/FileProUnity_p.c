

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Mar 15 22:56:23 2014
 */
/* Compiler settings for FileProUnity.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "FileProUnity_i.h"

#define TYPE_FORMAT_STRING_SIZE   1063                              
#define PROC_FORMAT_STRING_SIZE   493                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _FileProUnity_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } FileProUnity_MIDL_TYPE_FORMAT_STRING;

typedef struct _FileProUnity_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } FileProUnity_MIDL_PROC_FORMAT_STRING;

typedef struct _FileProUnity_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } FileProUnity_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const FileProUnity_MIDL_TYPE_FORMAT_STRING FileProUnity__MIDL_TypeFormatString;
extern const FileProUnity_MIDL_PROC_FORMAT_STRING FileProUnity__MIDL_ProcFormatString;
extern const FileProUnity_MIDL_EXPR_FORMAT_STRING FileProUnity__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IFileProCom_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IFileProCom_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const FileProUnity_MIDL_PROC_FORMAT_STRING FileProUnity__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure OpenFileDlg */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 16 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 24 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 30 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 36 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SelectDirDlg */

/* 42 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 44 */	NdrFcLong( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 58 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 60 */	NdrFcShort( 0x1 ),	/* 1 */
/* 62 */	NdrFcShort( 0x1 ),	/* 1 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strJsonIn */

/* 66 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 68 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 70 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strJsonOut */

/* 72 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 74 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 76 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFileInfo */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x9 ),	/* 9 */
/* 92 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x8 ),	/* 8 */
/* 98 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 100 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 102 */	NdrFcShort( 0x1 ),	/* 1 */
/* 104 */	NdrFcShort( 0x1 ),	/* 1 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strJSonIn */

/* 108 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 110 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 112 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 114 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 116 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 118 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 120 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 122 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteLocalFile */

/* 126 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 128 */	NdrFcLong( 0x0 ),	/* 0 */
/* 132 */	NdrFcShort( 0xa ),	/* 10 */
/* 134 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x8 ),	/* 8 */
/* 140 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 142 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 144 */	NdrFcShort( 0x1 ),	/* 1 */
/* 146 */	NdrFcShort( 0x1 ),	/* 1 */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 150 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 152 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 154 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 156 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 158 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 160 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 162 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 164 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFileMd5 */

/* 168 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0xb ),	/* 11 */
/* 176 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0x8 ),	/* 8 */
/* 182 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 184 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 186 */	NdrFcShort( 0x1 ),	/* 1 */
/* 188 */	NdrFcShort( 0x1 ),	/* 1 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 192 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 194 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 196 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 198 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 200 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 202 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 204 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 206 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenFile */

/* 210 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 216 */	NdrFcShort( 0xc ),	/* 12 */
/* 218 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 224 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 226 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 228 */	NdrFcShort( 0x1 ),	/* 1 */
/* 230 */	NdrFcShort( 0x1 ),	/* 1 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 234 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 236 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 238 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 240 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 242 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 244 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CopyFileCom */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0xd ),	/* 13 */
/* 260 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 266 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 268 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 270 */	NdrFcShort( 0x1 ),	/* 1 */
/* 272 */	NdrFcShort( 0x1 ),	/* 1 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 276 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 282 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 288 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 290 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCopyProgress */

/* 294 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 296 */	NdrFcLong( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0xe ),	/* 14 */
/* 302 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 306 */	NdrFcShort( 0x8 ),	/* 8 */
/* 308 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 310 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 312 */	NdrFcShort( 0x1 ),	/* 1 */
/* 314 */	NdrFcShort( 0x1 ),	/* 1 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 318 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 320 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 322 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 324 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 326 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 328 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 330 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 332 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetJsCallbackFunc */

/* 336 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0xf ),	/* 15 */
/* 344 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x8 ),	/* 8 */
/* 350 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 352 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 356 */	NdrFcShort( 0x1 ),	/* 1 */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter scriptCallback */

/* 360 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 362 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 364 */	NdrFcShort( 0x41c ),	/* Type Offset=1052 */

	/* Return value */

/* 366 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 368 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CancelCopy */

/* 372 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 374 */	NdrFcLong( 0x0 ),	/* 0 */
/* 378 */	NdrFcShort( 0x10 ),	/* 16 */
/* 380 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 384 */	NdrFcShort( 0x8 ),	/* 8 */
/* 386 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 388 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	NdrFcShort( 0x1 ),	/* 1 */
/* 394 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 396 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 398 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 400 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 402 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 404 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAllNewItemTypes */

/* 408 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 410 */	NdrFcLong( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x11 ),	/* 17 */
/* 416 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */
/* 420 */	NdrFcShort( 0x8 ),	/* 8 */
/* 422 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 424 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 426 */	NdrFcShort( 0x1 ),	/* 1 */
/* 428 */	NdrFcShort( 0x1 ),	/* 1 */
/* 430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 432 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 434 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 436 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 438 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 440 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 442 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 446 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateNewFile */

/* 450 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x12 ),	/* 18 */
/* 458 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 460 */	NdrFcShort( 0x0 ),	/* 0 */
/* 462 */	NdrFcShort( 0x8 ),	/* 8 */
/* 464 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 466 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 468 */	NdrFcShort( 0x1 ),	/* 1 */
/* 470 */	NdrFcShort( 0x1 ),	/* 1 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strInJson */

/* 474 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 476 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 478 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter strOutJson */

/* 480 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 482 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 484 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 486 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 488 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const FileProUnity_MIDL_TYPE_FORMAT_STRING FileProUnity__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xe ),	/* Offset= 14 (18) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 16 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 18 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x8 ),	/* 8 */
/* 22 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (6) */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 28 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 30 */	NdrFcShort( 0x0 ),	/* 0 */
/* 32 */	NdrFcShort( 0x4 ),	/* 4 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0xffde ),	/* Offset= -34 (2) */
/* 38 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 40 */	NdrFcShort( 0x6 ),	/* Offset= 6 (46) */
/* 42 */	
			0x13, 0x0,	/* FC_OP */
/* 44 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (18) */
/* 46 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x4 ),	/* 4 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (42) */
/* 56 */	
			0x12, 0x0,	/* FC_UP */
/* 58 */	NdrFcShort( 0x3ce ),	/* Offset= 974 (1032) */
/* 60 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 62 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 64 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 66 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 68 */	NdrFcShort( 0x2 ),	/* Offset= 2 (70) */
/* 70 */	NdrFcShort( 0x10 ),	/* 16 */
/* 72 */	NdrFcShort( 0x2f ),	/* 47 */
/* 74 */	NdrFcLong( 0x14 ),	/* 20 */
/* 78 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 80 */	NdrFcLong( 0x3 ),	/* 3 */
/* 84 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 86 */	NdrFcLong( 0x11 ),	/* 17 */
/* 90 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 92 */	NdrFcLong( 0x2 ),	/* 2 */
/* 96 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 98 */	NdrFcLong( 0x4 ),	/* 4 */
/* 102 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 104 */	NdrFcLong( 0x5 ),	/* 5 */
/* 108 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 110 */	NdrFcLong( 0xb ),	/* 11 */
/* 114 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 116 */	NdrFcLong( 0xa ),	/* 10 */
/* 120 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 122 */	NdrFcLong( 0x6 ),	/* 6 */
/* 126 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (358) */
/* 128 */	NdrFcLong( 0x7 ),	/* 7 */
/* 132 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 134 */	NdrFcLong( 0x8 ),	/* 8 */
/* 138 */	NdrFcShort( 0xff78 ),	/* Offset= -136 (2) */
/* 140 */	NdrFcLong( 0xd ),	/* 13 */
/* 144 */	NdrFcShort( 0xdc ),	/* Offset= 220 (364) */
/* 146 */	NdrFcLong( 0x9 ),	/* 9 */
/* 150 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (382) */
/* 152 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 156 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (400) */
/* 158 */	NdrFcLong( 0x24 ),	/* 36 */
/* 162 */	NdrFcShort( 0x31c ),	/* Offset= 796 (958) */
/* 164 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 168 */	NdrFcShort( 0x316 ),	/* Offset= 790 (958) */
/* 170 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 174 */	NdrFcShort( 0x314 ),	/* Offset= 788 (962) */
/* 176 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 180 */	NdrFcShort( 0x312 ),	/* Offset= 786 (966) */
/* 182 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 186 */	NdrFcShort( 0x310 ),	/* Offset= 784 (970) */
/* 188 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 192 */	NdrFcShort( 0x30e ),	/* Offset= 782 (974) */
/* 194 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 198 */	NdrFcShort( 0x30c ),	/* Offset= 780 (978) */
/* 200 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 204 */	NdrFcShort( 0x30a ),	/* Offset= 778 (982) */
/* 206 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 210 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (966) */
/* 212 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 216 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (970) */
/* 218 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 222 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (986) */
/* 224 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 228 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (982) */
/* 230 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 234 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (990) */
/* 236 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 240 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (994) */
/* 242 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 246 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (998) */
/* 248 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 252 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (1002) */
/* 254 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 258 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (1006) */
/* 260 */	NdrFcLong( 0x10 ),	/* 16 */
/* 264 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 266 */	NdrFcLong( 0x12 ),	/* 18 */
/* 270 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 272 */	NdrFcLong( 0x13 ),	/* 19 */
/* 276 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 278 */	NdrFcLong( 0x15 ),	/* 21 */
/* 282 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 284 */	NdrFcLong( 0x16 ),	/* 22 */
/* 288 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 290 */	NdrFcLong( 0x17 ),	/* 23 */
/* 294 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 296 */	NdrFcLong( 0xe ),	/* 14 */
/* 300 */	NdrFcShort( 0x2ca ),	/* Offset= 714 (1014) */
/* 302 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 306 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1024) */
/* 308 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 312 */	NdrFcShort( 0x2cc ),	/* Offset= 716 (1028) */
/* 314 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 318 */	NdrFcShort( 0x288 ),	/* Offset= 648 (966) */
/* 320 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 324 */	NdrFcShort( 0x286 ),	/* Offset= 646 (970) */
/* 326 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 330 */	NdrFcShort( 0x284 ),	/* Offset= 644 (974) */
/* 332 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 336 */	NdrFcShort( 0x27a ),	/* Offset= 634 (970) */
/* 338 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 342 */	NdrFcShort( 0x274 ),	/* Offset= 628 (970) */
/* 344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x0 ),	/* Offset= 0 (348) */
/* 350 */	NdrFcLong( 0x1 ),	/* 1 */
/* 354 */	NdrFcShort( 0x0 ),	/* Offset= 0 (354) */
/* 356 */	NdrFcShort( 0xffff ),	/* Offset= -1 (355) */
/* 358 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 360 */	NdrFcShort( 0x8 ),	/* 8 */
/* 362 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 364 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 366 */	NdrFcLong( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x0 ),	/* 0 */
/* 374 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 376 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 378 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 380 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 382 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 384 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 394 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 396 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 398 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 400 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 402 */	NdrFcShort( 0x2 ),	/* Offset= 2 (404) */
/* 404 */	
			0x12, 0x0,	/* FC_UP */
/* 406 */	NdrFcShort( 0x216 ),	/* Offset= 534 (940) */
/* 408 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 410 */	NdrFcShort( 0x18 ),	/* 24 */
/* 412 */	NdrFcShort( 0xa ),	/* 10 */
/* 414 */	NdrFcLong( 0x8 ),	/* 8 */
/* 418 */	NdrFcShort( 0x5a ),	/* Offset= 90 (508) */
/* 420 */	NdrFcLong( 0xd ),	/* 13 */
/* 424 */	NdrFcShort( 0x7e ),	/* Offset= 126 (550) */
/* 426 */	NdrFcLong( 0x9 ),	/* 9 */
/* 430 */	NdrFcShort( 0x9e ),	/* Offset= 158 (588) */
/* 432 */	NdrFcLong( 0xc ),	/* 12 */
/* 436 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (636) */
/* 438 */	NdrFcLong( 0x24 ),	/* 36 */
/* 442 */	NdrFcShort( 0x124 ),	/* Offset= 292 (734) */
/* 444 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 448 */	NdrFcShort( 0x140 ),	/* Offset= 320 (768) */
/* 450 */	NdrFcLong( 0x10 ),	/* 16 */
/* 454 */	NdrFcShort( 0x15a ),	/* Offset= 346 (800) */
/* 456 */	NdrFcLong( 0x2 ),	/* 2 */
/* 460 */	NdrFcShort( 0x174 ),	/* Offset= 372 (832) */
/* 462 */	NdrFcLong( 0x3 ),	/* 3 */
/* 466 */	NdrFcShort( 0x18e ),	/* Offset= 398 (864) */
/* 468 */	NdrFcLong( 0x14 ),	/* 20 */
/* 472 */	NdrFcShort( 0x1a8 ),	/* Offset= 424 (896) */
/* 474 */	NdrFcShort( 0xffff ),	/* Offset= -1 (473) */
/* 476 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 478 */	NdrFcShort( 0x4 ),	/* 4 */
/* 480 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 484 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 486 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 488 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 490 */	NdrFcShort( 0x4 ),	/* 4 */
/* 492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 494 */	NdrFcShort( 0x1 ),	/* 1 */
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 500 */	0x12, 0x0,	/* FC_UP */
/* 502 */	NdrFcShort( 0xfe1c ),	/* Offset= -484 (18) */
/* 504 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 506 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 508 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 510 */	NdrFcShort( 0x8 ),	/* 8 */
/* 512 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 514 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 516 */	NdrFcShort( 0x4 ),	/* 4 */
/* 518 */	NdrFcShort( 0x4 ),	/* 4 */
/* 520 */	0x11, 0x0,	/* FC_RP */
/* 522 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (476) */
/* 524 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 526 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 528 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 532 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 538 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 542 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 544 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 546 */	NdrFcShort( 0xff4a ),	/* Offset= -182 (364) */
/* 548 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 550 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 552 */	NdrFcShort( 0x8 ),	/* 8 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	NdrFcShort( 0x6 ),	/* Offset= 6 (562) */
/* 558 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 560 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 562 */	
			0x11, 0x0,	/* FC_RP */
/* 564 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (528) */
/* 566 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 570 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 576 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 580 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 582 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 584 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (382) */
/* 586 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 588 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 590 */	NdrFcShort( 0x8 ),	/* 8 */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x6 ),	/* Offset= 6 (600) */
/* 596 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 598 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 600 */	
			0x11, 0x0,	/* FC_RP */
/* 602 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (566) */
/* 604 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 606 */	NdrFcShort( 0x4 ),	/* 4 */
/* 608 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 614 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 616 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 618 */	NdrFcShort( 0x4 ),	/* 4 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x1 ),	/* 1 */
/* 624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 628 */	0x12, 0x0,	/* FC_UP */
/* 630 */	NdrFcShort( 0x192 ),	/* Offset= 402 (1032) */
/* 632 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 634 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 636 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 638 */	NdrFcShort( 0x8 ),	/* 8 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0x6 ),	/* Offset= 6 (648) */
/* 644 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 646 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 648 */	
			0x11, 0x0,	/* FC_RP */
/* 650 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (604) */
/* 652 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 654 */	NdrFcLong( 0x2f ),	/* 47 */
/* 658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 664 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 666 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 668 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 670 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 672 */	NdrFcShort( 0x1 ),	/* 1 */
/* 674 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 676 */	NdrFcShort( 0x4 ),	/* 4 */
/* 678 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 680 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 682 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 684 */	NdrFcShort( 0x10 ),	/* 16 */
/* 686 */	NdrFcShort( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0xa ),	/* Offset= 10 (698) */
/* 690 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 692 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 694 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (652) */
/* 696 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 698 */	
			0x12, 0x0,	/* FC_UP */
/* 700 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (670) */
/* 702 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 704 */	NdrFcShort( 0x4 ),	/* 4 */
/* 706 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 710 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 712 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 714 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 716 */	NdrFcShort( 0x4 ),	/* 4 */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	NdrFcShort( 0x1 ),	/* 1 */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 726 */	0x12, 0x0,	/* FC_UP */
/* 728 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (682) */
/* 730 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 732 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 734 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 736 */	NdrFcShort( 0x8 ),	/* 8 */
/* 738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 740 */	NdrFcShort( 0x6 ),	/* Offset= 6 (746) */
/* 742 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 744 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 746 */	
			0x11, 0x0,	/* FC_RP */
/* 748 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (702) */
/* 750 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 752 */	NdrFcShort( 0x8 ),	/* 8 */
/* 754 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 756 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 758 */	NdrFcShort( 0x10 ),	/* 16 */
/* 760 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 762 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 764 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (750) */
			0x5b,		/* FC_END */
/* 768 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 770 */	NdrFcShort( 0x18 ),	/* 24 */
/* 772 */	NdrFcShort( 0x0 ),	/* 0 */
/* 774 */	NdrFcShort( 0xa ),	/* Offset= 10 (784) */
/* 776 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 778 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 780 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (756) */
/* 782 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 784 */	
			0x11, 0x0,	/* FC_RP */
/* 786 */	NdrFcShort( 0xfefe ),	/* Offset= -258 (528) */
/* 788 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 790 */	NdrFcShort( 0x1 ),	/* 1 */
/* 792 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 798 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 800 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 802 */	NdrFcShort( 0x8 ),	/* 8 */
/* 804 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 806 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 808 */	NdrFcShort( 0x4 ),	/* 4 */
/* 810 */	NdrFcShort( 0x4 ),	/* 4 */
/* 812 */	0x12, 0x0,	/* FC_UP */
/* 814 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (788) */
/* 816 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 818 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 820 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 822 */	NdrFcShort( 0x2 ),	/* 2 */
/* 824 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 828 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 830 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 832 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 834 */	NdrFcShort( 0x8 ),	/* 8 */
/* 836 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 838 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 840 */	NdrFcShort( 0x4 ),	/* 4 */
/* 842 */	NdrFcShort( 0x4 ),	/* 4 */
/* 844 */	0x12, 0x0,	/* FC_UP */
/* 846 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (820) */
/* 848 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 850 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 852 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 854 */	NdrFcShort( 0x4 ),	/* 4 */
/* 856 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 860 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 862 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 864 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 868 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 870 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 872 */	NdrFcShort( 0x4 ),	/* 4 */
/* 874 */	NdrFcShort( 0x4 ),	/* 4 */
/* 876 */	0x12, 0x0,	/* FC_UP */
/* 878 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (852) */
/* 880 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 882 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 884 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 886 */	NdrFcShort( 0x8 ),	/* 8 */
/* 888 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 892 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 894 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 896 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 898 */	NdrFcShort( 0x8 ),	/* 8 */
/* 900 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 902 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 904 */	NdrFcShort( 0x4 ),	/* 4 */
/* 906 */	NdrFcShort( 0x4 ),	/* 4 */
/* 908 */	0x12, 0x0,	/* FC_UP */
/* 910 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (884) */
/* 912 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 914 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 916 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 918 */	NdrFcShort( 0x8 ),	/* 8 */
/* 920 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 922 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 924 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 926 */	NdrFcShort( 0x8 ),	/* 8 */
/* 928 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 930 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 932 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 934 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 936 */	NdrFcShort( 0xffec ),	/* Offset= -20 (916) */
/* 938 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 940 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 942 */	NdrFcShort( 0x28 ),	/* 40 */
/* 944 */	NdrFcShort( 0xffec ),	/* Offset= -20 (924) */
/* 946 */	NdrFcShort( 0x0 ),	/* Offset= 0 (946) */
/* 948 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 950 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 952 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 954 */	NdrFcShort( 0xfdde ),	/* Offset= -546 (408) */
/* 956 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 958 */	
			0x12, 0x0,	/* FC_UP */
/* 960 */	NdrFcShort( 0xfeea ),	/* Offset= -278 (682) */
/* 962 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 964 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 966 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 968 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 970 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 972 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 974 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 976 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 978 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 980 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 982 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 984 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 986 */	
			0x12, 0x0,	/* FC_UP */
/* 988 */	NdrFcShort( 0xfd8a ),	/* Offset= -630 (358) */
/* 990 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 992 */	NdrFcShort( 0xfc22 ),	/* Offset= -990 (2) */
/* 994 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 996 */	NdrFcShort( 0xfd88 ),	/* Offset= -632 (364) */
/* 998 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1000 */	NdrFcShort( 0xfd96 ),	/* Offset= -618 (382) */
/* 1002 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1004 */	NdrFcShort( 0xfda4 ),	/* Offset= -604 (400) */
/* 1006 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1008 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1010) */
/* 1010 */	
			0x12, 0x0,	/* FC_UP */
/* 1012 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1032) */
/* 1014 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1016 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1018 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1020 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1022 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1024 */	
			0x12, 0x0,	/* FC_UP */
/* 1026 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1014) */
/* 1028 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1030 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1032 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1034 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1036 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1038 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1038) */
/* 1040 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1042 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1044 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1046 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1048 */	NdrFcShort( 0xfc24 ),	/* Offset= -988 (60) */
/* 1050 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1052 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1054 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1056 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1060 */	NdrFcShort( 0xfc14 ),	/* Offset= -1004 (56) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IFileProCom, ver. 0.0,
   GUID={0xDF88EC29,0x172B,0x40AB,{0x92,0x69,0x82,0xE3,0x75,0x15,0xEB,0x68}} */

#pragma code_seg(".orpc")
static const unsigned short IFileProCom_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    42,
    84,
    126,
    168,
    210,
    252,
    294,
    336,
    372,
    408,
    450
    };

static const MIDL_STUBLESS_PROXY_INFO IFileProCom_ProxyInfo =
    {
    &Object_StubDesc,
    FileProUnity__MIDL_ProcFormatString.Format,
    &IFileProCom_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IFileProCom_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    FileProUnity__MIDL_ProcFormatString.Format,
    &IFileProCom_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) _IFileProComProxyVtbl = 
{
    &IFileProCom_ProxyInfo,
    &IID_IFileProCom,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::OpenFileDlg */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::SelectDirDlg */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::GetFileInfo */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::DeleteLocalFile */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::GetFileMd5 */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::OpenFile */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::CopyFileCom */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::GetCopyProgress */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::SetJsCallbackFunc */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::CancelCopy */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::GetAllNewItemTypes */ ,
    (void *) (INT_PTR) -1 /* IFileProCom::CreateNewFile */
};


static const PRPC_STUB_FUNCTION IFileProCom_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IFileProComStubVtbl =
{
    &IID_IFileProCom,
    &IFileProCom_ServerInfo,
    19,
    &IFileProCom_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    FileProUnity__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _FileProUnity_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IFileProComProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _FileProUnity_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IFileProComStubVtbl,
    0
};

PCInterfaceName const _FileProUnity_InterfaceNamesList[] = 
{
    "IFileProCom",
    0
};

const IID *  const _FileProUnity_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _FileProUnity_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _FileProUnity, pIID, n)

int __stdcall _FileProUnity_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_FileProUnity_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo FileProUnity_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _FileProUnity_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _FileProUnity_StubVtblList,
    (const PCInterfaceName * ) & _FileProUnity_InterfaceNamesList,
    (const IID ** ) & _FileProUnity_BaseIIDList,
    & _FileProUnity_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

