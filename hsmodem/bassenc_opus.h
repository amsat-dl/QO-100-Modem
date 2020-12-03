/*
	BASSenc_OPUS 2.4 C/C++ header file
	Copyright (c) 2016 Un4seen Developments Ltd.

	See the BASSENC_OPUS.CHM file for more detailed documentation
*/

#ifndef BASSENC_OPUS_H
#define BASSENC_OPUS_H

#include "bassenc.h"

#if BASSVERSION!=0x204
#error conflicting BASS and BASSenc_OPUS versions
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSENCOPUSDEF
#define BASSENCOPUSDEF(f) WINAPI f
#endif

DWORD BASSENCOPUSDEF(BASS_Encode_OPUS_GetVersion)();

HENCODE BASSENCOPUSDEF(BASS_Encode_OPUS_Start)(DWORD handle, const char *options, DWORD flags, ENCODEPROC *proc, void *user);
HENCODE BASSENCOPUSDEF(BASS_Encode_OPUS_StartFile)(DWORD handle, const char *options, DWORD flags, const char *filename);

#ifdef __cplusplus
}

#ifdef _WIN32
static inline HENCODE BASS_Encode_OPUS_Start(DWORD handle, const WCHAR *options, DWORD flags, ENCODEPROC *proc, void *user)
{
	return BASS_Encode_OPUS_Start(handle, (const char*)options, flags|BASS_UNICODE, proc, user);
}

static inline HENCODE BASS_Encode_OPUS_StartFile(DWORD handle, const WCHAR *options, DWORD flags, const WCHAR *filename)
{
	return BASS_Encode_OPUS_StartFile(handle, (const char*)options, flags|BASS_UNICODE, (const char*)filename);
}
#endif
#endif

#endif
