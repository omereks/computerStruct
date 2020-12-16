.data

.section	.rodata	
format_du:       .string "%hhu"
format_s:       .string "%s"
format_d:       .string "%d"





# Pstring hello = '5' , 'h' , 'e' , 'l' , 'l' , 'o' , '/0'

# getting two pstrings. length. optaionSelect.
# send to run_func optaionSelect. two pstring adresses.
.text
.globl	run_main
.type	run_main, @function
run_main:
    pushq	%rbp		# save the old frame pointer
	movq	%rsp, %rbp	# create the new frame pointer

    

    # first Pstring
    sub     $256, %rsp
    pushq   %r14              #temp registers for pointers to pstr
    pushq   %r15
    # get first size of pstring1
    movq    $0, %rax          
    movq    $format_d, %rdi
    leaq    (%rsp), %rsi
    call    scanf
    leaq    (%rsp), %r14
    # get first string of pstring1
    movq    $0, %rax          
    movq    $format_s, %rdi
    leaq    1(%rsp), %rsi
    call    scanf


    # second Pstring
    sub     $256, %rsp
    # get first size of pstring2
    movq    $0, %rax          
    movq    $format_d, %rdi
    leaq    (%rsp), %rsi
    call    scanf
    leaq    (%rsp), %r15
    # get first string of pstring2
    movq    $0, %rax          
    movq    $format_s, %rdi
    leaq    1(%rsp), %rsi
    call    scanf

    
    # get user option
    sub     $16, %rsp 
    movq    $0, %rax          
    movq    $format_d, %rdi
    leaq    (%rsp), %rsi
    call    scanf
    

    # build run_func
    movq    (%rsp), %rdi   # user option
    movq    %r14, %rsi     # Pstring 1
    movq    %r15, %rdx     # Pstring 2     
    call    run_func

    popq    %r15               #free the stack space
    popq    %r14

    
    movq    %rbp, %rsp
    popq    %rbp
    ret
