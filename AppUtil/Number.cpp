#include "Number.h"

extern "C" {
	ubool OverflowAddNum8(uint8_t* result, uint8_t Addend1, uint8_t Addend2);
	ubool OverflowAddNum16(uint16_t* result, uint16_t Addend1, uint16_t Addend2);
	ubool OverflowAddNum32(uint32_t* result, uint32_t Addend1, uint32_t Addend2);
#ifdef _M_X64
	ubool OverflowAddNum64(uint64_t* result, uint64_t Addend1, uint64_t Addend2);
#endif // _M_X64
}

ubool u8PlusOverflow(uint8_t* result, uint8_t Addend1, uint8_t Addend2)
{
	return OverflowAddNum8(result, Addend1, Addend2);
}

ubool u16PlusOverflow(uint16_t* result, uint16_t Addend1, uint16_t Addend2)
{
	return OverflowAddNum16(result, Addend1, Addend2);
}

ubool u32PlusOverflow(uint32_t* result, uint32_t Addend1, uint32_t Addend2)
{
	return OverflowAddNum32(result, Addend1, Addend2);
}

#ifdef _M_X64
ubool u64PlusOverflow(uint64_t* result, uint64_t Addend1, uint64_t Addend2)
{
	return OverflowAddNum64(result, Addend1, Addend2);
}
#endif // _M_X64
