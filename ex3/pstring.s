
.data


.section	.rodata	
format_dd:       .string "%lu \n"
format_c:       .string "%c "
format_ch:       .string "ok"               # ###################################



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

