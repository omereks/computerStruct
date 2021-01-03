#   312350192 Omer Eckstein

.data
.section	.rodata	
format_dd:       .string "%hhu"
format_c:       .string " %c"
format_invalid:  .string "invalid input!\n"
format_ok:       .string "ok"               # ###################################


.text	                    # the beginnig of the code
.globl	pstrlen	            
.type	pstrlen, @function	
pstrlen:                    # return the length of a pstring
     # rdi *pstring
    movq    (%rdi),%rcx     # get to the address
    movb    %cl, %al        # get the first bit to return
    ret








.globl	replaceChar	        
.type	replaceChar, @function	

replaceChar:                # get a pstring and 2 chars and replace betwin 2 chars
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
    cmpq    $0, %r12            # stop mode
    je      .LoopDone
    
    leaq    1(%r9) , %r9        # getting the next char
    movq    (%r9), %r13

    cmpb    %r13b, -8(%rbp)     # check if nescesry to swap chars
    je      .changeChar
.Loopb:
    leaq     -1(%r12), %r12     # i-- 
    jmp     .Loop

.changeChar:
    movb    -16(%rbp), %r13b    # swithcing the chars
    movb    %r13b, (%r9)
    jmp     .Loopb

.LoopDone:
    movq    %r9, %rax           # get out the loop 
    ret






.globl	pstrijcpy	        
.type	pstrijcpy, @function	

pstrijcpy:                          # get 2 pstring and 2 indexs and switch the indexs betwin the 2 pstrings
    # pstring 1 in rdi
    # pstring 2 in rsi
    # i in rdx edx
    # j in rcx ecx
    
    pushq   %rdi
    pushq   %rsi
    pushq   %rdx
    pushq   %rcx

    # check when invalid input
    cmpb    %cl, (%rsi)         # j >src.length
    jbe     .invalidInput
    
    cmpb    %cl, (%rdi)         # j >dst.length
    jbe     .invalidInput
    
    cmpb    $0, %dl             # i <= 0
    jb     .invalidInput
    
    cmpb    $0, %cl             # j <= 0
    jb     .invalidInput
    
    cmpb    %dl, %cl            # i>j
    jb     .invalidInput
    
    popq   %rcx
    popq   %rdx
    popq    %rsi
    popq    %rdi
    movq    %rdi, %rax


    
    leaq    1(%rdi,%rdx), %rdi     # set the pointer to i
    leaq    1(%rsi,%rdx), %rsi
    jmp     .loopCop
.loopCop:                   # start loop

    movb (%rsi), %r8b       
    movb %r8b,(%rdi)
    add     $1, %rdx        # i++

    leaq    1(%rdi), %rdi
    leaq    1(%rsi), %rsi
    
    cmpb    %cl, %dl        # stop condition
    jbe      .loopCop
    ret

.invalidInput:
    movq    $0, %rax
    movq    $format_invalid, %rdi
    call    printf          # printing
    popq    %rax            # return value
    ret



.globl	swapCase	        
.type	swapCase, @function	

swapCase:                       # swap from capital letter into small letter and backwords
    # pstring in rdi
    pushq   %rdi
    movq    %rdi, %r14
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
    cmpb $65, (%r9)         # less then 65
    jl .LoopbSwapEnd
    cmpb $90, (%r9)         # more then 90
    ja .small_letter
    # Else
    addb $32, (%r9)
    jmp .LoopbSwapEnd

.small_letter:
    cmpb $97, (%r9)         # less then 97
    jl .LoopbSwapEnd
    cmpb $122, (%r9)        # more then 122
    ja .LoopbSwapEnd
    # Else
    subb $32, (%r9)
    jmp .LoopbSwapEnd

.LoopbSwapEnd:
    leaq     -1(%r12), %r12 # i-- 
    jmp     .LoopSwap       # continue to the next char

.LoopDoneSwap:              # rxit loop
    popq    %rdi
    movq    %r14, %rax  
    ret





.globl	pstrijcmp	        
.type	pstrijcmp, @function	

pstrijcmp:                          # comparing betwin 2 pstrings
    # pstring 1 in rdi
    # pstring 2 in rsi
    # i in rdx edx
    # j in rcx ecx
    
    pushq   %rdi

    # check when invalid input
    cmpb    %cl, (%rsi)             # j >src.length
    jbe     .invalidInputCopy
    
    cmpb    %cl, (%rdi)             # j >dst.length
    jbe     .invalidInputCopy
    
    cmpb    $0, %dl                 # i <= 0
    jb     .invalidInputCopy
    
    cmpb    $0, %cl                 # j <= 0
    jb     .invalidInputCopy
    
    cmpb    %dl, %cl                # i>j
    jb     .invalidInputCopy
    

    popq    %rdi
    movq    %rdi, %rax
    
    
    leaq 1(%rdi,%rdx), %rdi     # set the pointer to i
    leaq 1(%rsi,%rdx), %rsi
    
.loopCopCmp:
    movb (%rsi), %r8b       
    cmpb (%rdi), %r8b       # check if char i in psring 1 an pstring 2 are big or small
    jb .pst1Big
    ja .pst2Big
    
            
    add     $1, %rdx        # i++ 
    leaq    1(%rdi), %rdi
    leaq    1(%rsi), %rsi
    
    cmpb    %cl, %dl        # stop mode
    jbe      .loopCopCmp
    movq    $0, %rax
    ret


.pst1Big:
    movq    $0, %rax
    movq    $1, %rax        # ret 1
    ret
.pst2Big:
    movq    $0, %rax
    movq    $-1, %rax       # ret -1
    ret

.invalidInputCopy:
    popq    %rdi
    movq    $0, %rax
    movq    $format_invalid, %rdi
    call    printf          # printing
    movq    $0, %rax
    movq    $-2, %rax       # ret -2
    ret

