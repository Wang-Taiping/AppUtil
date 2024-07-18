
.model flat

.data

.const

.code

_OverflowAddNum8 proc ; *ret, a, b; ecx, ebx, eax

	pop ecx
	pop eax
	pop ebx
	add bl, al
	jc ofl8

	mov eax, 0
	jmp ctnu8
ofl8:
	mov eax, 1
ctnu8:
	mov byte ptr [ecx], bl
	ret

_OverflowAddNum8 endp

_OverflowAddNum16 proc ; *ret, a, b

	pop ecx
	pop eax
	pop ebx
	add bx, ax
	jc ofl16

	mov eax, 0
	jmp ctnu16
ofl16:
	mov eax, 1
ctnu16:
	mov word ptr [ecx], bx
	ret

_OverflowAddNum16 endp

_OverflowAddNum32 proc ; *ret, a, b

	pop ecx
	pop eax
	pop ebx
	add ebx, eax
	jc ofl32

	mov eax, 0
	jmp ctnu32
ofl32:
	mov eax, 1
ctnu32:
	mov dword ptr [ecx], ebx
	ret

_OverflowAddNum32 endp
end
