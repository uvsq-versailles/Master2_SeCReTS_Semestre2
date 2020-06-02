.globl moyenneopti

moyenneopti:
    movq $rdi, %rax     
    leaq (%rdi, %rsi, 8), %rdx          
    vzeroall            

beginloop:
    vaddpd (%rax), %ymm1, %ymm1
    addq $32, %rax
    cmpq %rdx, %rax     
    jl beginloop
    vaddpd %ymm1, %ymm1, %ymm1
    vextractf128 $1, %ymm1, %xmm2
    vextractf128 $0, %ymm1, %xmm1
    addsd %xmm2, %xmm-1

    cvtsi2sd %rsi, %xmm2
    divsd %xmm2, %xmm1
    movq %xmm1, %xmm0
    ret
    
