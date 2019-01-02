#include "i8042.h"
.intel_syntax noprefix

.global _kbc_asm_ih

.extern kbc_code

.data
data:
	.long 0
stat:
	.long 0


_kbc_asm_ih:
BEGIN:
	xor 	eax,	eax
	mov 	dx,		KBD_STATUS
	in 		al,		dx
	mov 	stat, 	al
	mov 	bl, 	stat
	and 	bl,		OBF
	cmp 	bl,		1
	jne     BEGIN
	mov 	al, 	data
	mov 	al, 	kbc_code
	mov 	dx,		OUT_BUF
	in 		al,		dx
	mov 	bl,		stat
	and	 	bl,		(PAR_ERR | TO_ERR)
	cmp		bl,		0
	je 		END
ERRO:
	mov 	al, 	-1
END:
	ret
