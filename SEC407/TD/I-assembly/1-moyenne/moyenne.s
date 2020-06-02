.globl moyenne

moyenne:
    movq $0, %rax                       # i = 0
    movq %rax, %xmm1                    # x = 0

beginloop:
    addsd (%rdi,%rax,8), %xmm1          # t[i] 
    addq $1, %rax                       # i + 1
    cmpq %rsi, %rax                     # if i < n
    
    jl beginloop
    cvtsi2sd %rsi, %xmm2                # result = (double)N
    divsd %xmm2, %xmm1
    movq %xmm1, %xmm0
    ret
    
