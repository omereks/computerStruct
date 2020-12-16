
.data


.section	.rodata	
format_dd:       .string "%hhu \n"
format_c:       .string "%c"
format_invalid:  .string "invalid input!\n"
format_ok:       .string "ok"               # ###################################



.text	                    # the beginnig of the code
.globl	pstrlen	        # the label "pstrlen" is used to state the initial point of this program
.type	pstrlen, @function	# the label "pstrlen" representing the beginning of a function

pstrlen:
    
    movq    (%rdi),%rcx
    movb    %cl, %al
    
    ret








.globl	replaceChar	        # the label "replaceChar" is used to state the initial point of this program
.type	replaceChar, @function	# the label "replaceChar" representing the beginning of a function

replaceChar:
    # rdi *pstring
    # rsi char old
    # rdx char new
    movq    %rdi, %r9       # keeping the pointer
    movq    %rdi, %r12      # keeping the pointer
    movq    %rdi, %r13      # keeping the pointer
    movq    $0, %rax
    call    pstrlen
    movq    $0, %r12
    movq    %rax, %r12          # r12 is now the number of letters in the loop  

    jmp     .Loop
    ret
    
    .Loop:
        cmpq    $0, %r12        # stop mode
        je      .LoopDone
        
        leaq    1(%r9) , %r9    # getting the next char
        movq    (%r9), %r13
    
        cmpb    %r13b, -8(%rbp) # check if nescesry to swap chars
        je      .changeChar
    .Loopb:
        leaq     -1(%r12), %r12 # i-- 
        jmp     .Loop

    .changeChar:
        movb    -16(%rbp), %r13b    # swithcing the chars
        movb    %r13b, (%r9)
        jmp     .Loopb

    .LoopDone:
        movq    %r9, %rax  
        ret






.globl	pstrijcpy	        # the label "replaceChar" is used to state the initial point of this program
.type	pstrijcpy, @function	# the label "replaceChar" representing the beginning of a function

pstrijcpy:
    # pstring 1 in rdi
    # pstring 2 in rsi
    # i in rdx edx
    # j in rcx ecx
    
    pushq   %rdi
    
    # check when invalid input
    # j >src.length
    cmpb    %cl, (%rsi)
    jbe     .invalidInput
    # j >dst.length
    cmpb    %cl, (%rdi)
    jbe     .invalidInput
    # i <= 0
    cmpb    $0, %dl
    jb     .invalidInput
    # j <= 0
    cmpb    $0, %cl
    jb     .invalidInput
    # i>j
    cmpb    %dl, %cl
    jb     .invalidInput
    

    popq    %rdi
    movq    %rdi, %rax
    # set the pointer to i
    
    # addq    $1, %rdi
    # addq    (%rdx), %rdi
    # addq    $1, %rsi
    # addq    (%rdx), %rsi


    leaq 1(%rdi,%rdx), %rdi     #move dst and src pstr to i'th place
    leaq 1(%rsi,%rdx), %rsi
    
    .loopCop:
        
        movb    (%rsi), %r8b
        movb    %r8b,  (%rdi)
        
        add     $1, %rdx # i++ 
        leaq    1(%rdi), %rdi
        leaq    1(%rsi), %rsi
        
        cmpb    %cl, %dl        # stop mode
        jbe      .loopCop
        ret

    .invalidInput:
        movq    $0, %rax
        movq    $format_invalid, %rdi
        call    printf
        popq    %rax
        ret



.globl	swapCase	        # the label "swapCase" is used to state the initial point of this program
.type	swapCase, @function	# the label "swapCase" representing the beginning of a function

swapCase:
    # pstring in rdi
    pushq   %rdi
    
    movq    %rdi, %r9       # keeping the pointer
    movq    $0, %rax
    call    pstrlen
    movq    $0, %r12
    movq    %rax, %r12          # r12 is now the number of letters in the loop  

    .LoopSwap:
        cmpq    $0, %r12        # stop mode
        je      .LoopDoneSwap
        
        leaq    1(%r9) , %r9    # getting the next char
        movq    (%r9), %r13
    # capital_letter:   %r13b, (%r9)
        cmpb $65, (%r9)        # less then 65
        jl .LoopbSwapEnd
        cmpb $90, (%r9)        # more then 90
        ja .small_letter
        # Else
        addb $32, (%r9)
        jmp .LoopbSwapEnd
    
    .small_letter:
        cmpb $97, (%r9)        # less then 97
        jl .LoopbSwapEnd
        cmpb $122, (%r9)        # more then 122
        ja .LoopbSwapEnd
        # Else
        subb $32, (%r9)
        jmp .LoopbSwapEnd

    .LoopbSwapEnd:
        leaq     -1(%r12), %r12 # i-- 
        jmp     .LoopSwap

    .LoopDoneSwap:
        popq    %rdi
        movq    %rdi, %rax  
        ret





.globl	pstrijcmp	        # the label "pstrijcmp" is used to state the initial point of this program
.type	pstrijcmp, @function	# the label "pstrijcmp" representing the beginning of a function

pstrijcmp:
    # pstring 1 in rdi
    # pstring 2 in rsi
    # i in rdx edx
    # j in rcx ecx
    
    # check when invalid input
    # j >src.length
    cmpb    %cl, (%rsi)
    jbe     .invalidInputCopy
    # j >dst.length
    cmpb    %cl, (%rdi)
    jbe     .invalidInputCopy
    # i <= 0
    cmpb    $0, %dl
    jb     .invalidInputCopy
    # j <= 0
    cmpb    $0, %cl
    jb     .invalidInputCopy
    # i>j
    cmpb    %dl, %cl
    jb     .invalidInputCopy
    


    .invalidInputCopy:
        movq    $0, %rax
        movq    $format_invalid, %rdi
        call    printf
        ret



    movq    $0, %rax
    ret
