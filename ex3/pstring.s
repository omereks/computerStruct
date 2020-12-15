
.data


.section	.rodata	
format_dd:       .string "%lu \n"
format_c:       .string "%c "
format_ch:       .string "ok"



.text	                    # the beginnig of the code
.globl	pstrlen	        # the label "main" is used to state the initial point of this program
.type	pstrlen, @function	# the label "main" representing the beginning of a function

pstrlen:
    
    movq    (%rdi),%rcx
    movb    %cl, %al
    
    ret

.globl	replaceChar	        # the label "main" is used to state the initial point of this program
.type	replaceChar, @function	# the label "main" representing the beginning of a function

replaceChar:
    # rdi *pstring
    # rsi char old
    # rdx char new
    movq    %rdi, %r9        # keeping the pointer
    movq    %rdi, %r12
    movq    %rdi, %r13
    movq    $0, %rax
    call    pstrlen
    movq    $0, %r12
    movq    %rax, %r12          # r12 is now the number of letter in the loop  

    
    
    jmp     .Loop
    ret
    
.Loop:
    cmpq    $0, %r12
    je      .LoopDone
    
    
    leaq    1(%r9) , %r9
    movq    (%r9), %r13
   
        
    # ###

    # movq    $0, %rsi
    # leaq    1(%rdi), %rsi
    # movq    (%rsi), %rsi

   # movq    $format_c, %rdi
   # movq    %r13, %rsi
   # movq	$0, %rax
   # call	printf
   # ret
    # ###


    cmpb    %r13b, -8(%rbp)
    je      .changeChar

    


    leaq     -1(%r12), %r12
    jmp     .Loop

.changeChar:
    # movq    $format_ch, %rdi
    # movq	$0, %rax
    # call	printf

    movb    -16(%rbp), %r13b
    movb    %r13b, (%r9)
    # movb    -16(%rbp), %r13b            # ######################3 how the fuck to swap chars
    ret

.LoopDone:
    movq    %r9, %rax  
    ret

