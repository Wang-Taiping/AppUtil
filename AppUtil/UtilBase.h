#pragma once

#ifndef APPUTIL_H
#define APPUTIL_H

#include <stdint.h>

#ifdef APPUTIL_EXPORT
#undef APPUTIL_EXPORT
#endif // APPUTIL_EXPORT
#ifdef APPUTIL_BUILD
#define APPUTIL_EXPORT __declspec(dllexport)
#else
#define APPUTIL_EXPORT __declspec(dllimport)
#endif // APPUTIL_BUILD

#define TYPEHANDLE(name)	typedef struct __##name { int unused; } *name

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
