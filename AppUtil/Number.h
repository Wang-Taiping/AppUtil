#pragma once

#ifndef NUMBER_H
#define NUMBER_H

#include "UtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	APPUTIL_EXPORT ubool APPUTIL_API u8PlusOverflow(uint8_t* result, uint8_t Addend1, uint8_t Addend2);
	APPUTIL_EXPORT ubool APPUTIL_API u16PlusOverflow(uint16_t* result, uint16_t Addend1, uint16_t Addend2);
	APPUTIL_EXPORT ubool APPUTIL_API u32PlusOverflow(uint32_t* result, uint32_t Addend1, uint32_t Addend2);
#ifdef _M_X64
	APPUTIL_EXPORT ubool APPUTIL_API u64PlusOverflow(uint64_t* result, uint64_t Addend1, uint64_t Addend2);
#endif // _M_X64
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !NUMBER_H
