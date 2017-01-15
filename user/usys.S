// System call numbers

#define SYS_get_pid    1
#define SYS_puts       2
#define SYS_yield      3

#define syscall(name) \
.globl name; \
name: \
	push {r4};\
	mov r4, r3;\
	mov r3, r2;\
	mov r2, r1;\
	mov r1, r0;\
	mov r0, #SYS_ ## name;\
	swi 0x00;\
	pop {r4};\
	bx lr;

syscall(get_pid)
syscall(puts)
syscall(yield)
