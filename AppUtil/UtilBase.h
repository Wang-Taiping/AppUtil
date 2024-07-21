#pragma once

#ifndef APPUTIL_H
#define APPUTIL_H

#include <stdint.h>

#ifdef APPUTIL_EXPORT
#error Macro 'APPUTIL_EXPORT' already defined.
#endif // APPUTIL_EXPORT

#ifdef APPUTIL_BUILD
#define APPUTIL_EXPORT	__declspec(dllexport)
#else
#define APPUTIL_EXPORT	__declspec(dllimport)
#endif // APPUTIL_BUILD

#ifdef APPUTIL_STATIC
#undef APPUTIL_EXPORT
#define APPUTIL_EXPORT
#endif // APPUTIL_STATIC

#define APPUTIL_API			__stdcall
#define APPUTIL_CALLBACK	__stdcall

#define APPUTIL_TYPENAME(name)	struct __##name { int unused; }; typedef struct __##name* name

typedef char ubool;

typedef enum Access
{
	AccessNone,
	AccessRead,
	AccessWrite,
	AccessAll
} Access;

typedef enum ConflictBehavior
{
	ConflictError,
	ConflictForce,
	ConflictSkip,
	ConflictCancel
} ConflictBehavior;

#endif // !APPUTIL_H
