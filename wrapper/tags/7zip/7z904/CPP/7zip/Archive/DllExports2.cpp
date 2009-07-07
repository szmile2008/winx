// DLLExports.cpp

#include "StdAfx.h"

#define INITGUID
#include "../../Common/MyInitGuid.h"
#include "../../Common/ComTry.h"
#include "../../Common/Types.h"
#include "../../Windows/PropVariant.h"
#if defined(_WIN32) && defined(_7ZIP_LARGE_PAGES)
#include "../../../C/Alloc.h"
#endif

#include "IArchive.h"
#include "../ICoder.h"
#include "../IPassword.h"

HINSTANCE g_hInstance;
#ifndef _UNICODE
#ifdef _WIN32
bool g_IsNT = false;
static bool IsItWindowsNT()
{
  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  if (!::GetVersionEx(&versionInfo))
    return false;
  return (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);
}
#endif
#endif

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
  if (dwReason == DLL_PROCESS_ATTACH)
  {
    g_hInstance = hInstance;
    #ifndef _UNICODE
    #ifdef _WIN32
    g_IsNT = IsItWindowsNT();
    #endif
    #endif
  }
  return TRUE;
}

DEFINE_GUID(CLSID_CArchiveHandler,
0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x00);

static const UInt16 kDecodeId = 0x2790;

DEFINE_GUID(CLSID_CCodec,
0x23170F69, 0x40C1, kDecodeId, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

STDAPI CreateCoder(const GUID *clsid, const GUID *iid, void **outObject);
STDAPI CreateArchiver(const GUID *classID, const GUID *iid, void **outObject);

STDAPI CreateObject(const GUID *clsid, const GUID *iid, void **outObject)
{
  // COM_TRY_BEGIN
  *outObject = 0;
  if (*iid == IID_ICompressCoder || *iid == IID_ICompressCoder2 || *iid == IID_ICompressFilter)
  {
    return CreateCoder(clsid, iid, outObject);
  }
  else
  {
    return CreateArchiver(clsid, iid, outObject);
  }
  // COM_TRY_END
}

STDAPI SetLargePageMode()
{
  #if defined(_WIN32) && defined(_7ZIP_LARGE_PAGES)
  SetLargePageSize();
  #endif
  return S_OK;
}

//
// by xushiweizh@gmail.com
//

#include "../Common/FileStreams.h"
#include "../Common/FileStreams.cpp"

STDAPI CreateInFileStream(LPCWSTR szFile, IInStream** ppstm)
{
    CInFileStream *fileSpec = new CInFileStream;
	if (fileSpec->Open(szFile))
	{
		fileSpec->AddRef();
		*ppstm = fileSpec;
		return S_OK;
	}
	delete fileSpec;
	*ppstm = NULL;
	return E_ACCESSDENIED;
}

STDAPI CreateOutFileStream(LPCWSTR szFile, IOutStream** ppstm)
{
	COutFileStream *fileSpec = new COutFileStream;
	if (fileSpec->Open(szFile, CREATE_ALWAYS))
	{
		fileSpec->AddRef();
		*ppstm = fileSpec;
		return S_OK;
	}
	delete fileSpec;
	*ppstm = NULL;
	return E_ACCESSDENIED;
}
