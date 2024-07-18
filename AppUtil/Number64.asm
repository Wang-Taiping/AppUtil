
.data

.const

.code

OverflowAddNum8 proc ; *ret, a, b

	add dl, r8b
	jc ofl8

	mov eax, 0
	jmp ctnu8
ofl8:
	mov eax, 1
ctnu8:
	mov byte ptr [rcx], dl
	ret

OverflowAddNum8 endp

OverflowAddNum16 proc ; *ret, a, b

	add dx, r8w
	jc ofl16

	mov eax, 0
	jmp ctnu16
ofl16:
	mov eax, 1
ctnu16:
	mov word ptr [rcx], dx
	ret

OverflowAddNum16 endp

OverflowAddNum32 proc ; *ret, a, b

	add edx, r8d
	jc ofl32

	mov eax, 0
	jmp ctnu32
ofl32:
	mov eax, 1
ctnu32:
	mov dword ptr [rcx], edx
	ret

OverflowAddNum32 endp

OverflowAddNum64 proc ; *ret, a, b

	add rdx, r8
	jc ofl64

	mov eax, 0
	jmp ctnu64
ofl64:
	mov eax, 1
ctnu64:
	mov qword ptr [rcx], rdx
	ret

OverflowAddNum64 endp
end
