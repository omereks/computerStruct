.data


.section	.rodata	
formatInvalid:	.string	"invalid option!\n"
format5060:	    .string	"first pstring length: %d, second pstring length: %d\n"
format52:	    .string	"old char: %c,new char: %c, first string: %s, second string: %s\n"
format5354:	    .string	"length: %d, string: %s\n"
format55:	    .string	"compare result: %d\n"

.align 8
.Jtable:
    .quad   .J50    # 50/60
    .quad   .Jin    # 51 invalid
    .quad   .J52    # 52  
    .quad   .J53    # 53
    .quad   .J54    # 54
    .quad   .J55    # 55
    .quad   .Jin    # 56 invalid
    .quad   .Jin    # 57 invalid
    .quad   .Jin    # 58 invalid
    .quad   .Jin    # 59 invalid
    .quad   .J50    # 50/60

	########


.text	                    # the beginnig of the code
.globl	run_func	        # the label "main" is used to state the initial point of this program
.type	run_func, @function	# the label "main" representing the beginning of a function


run_func:	# the main function:
	pushq	%rbp		# save the old frame pointer
	movq	%rsp, %rbp	# create the new frame pointer
	

	# rdi is thr option number 50/60 52 53 54 55 else
    # rsi is pstring1
    # rdx is pstring2
    leaq    -50(%rdi), %rdi   # rdi = rdi -50  -> getting [0,10]
    cmpq    $10, %rdi
    ja      .Jin
    # leaq    (%rbp, %rdi, 8), %rsp
    jmp     *.Jtable(,%rdi,8)
    

	movq	%rbp, %rsp	# restore the old stack pointer - release all used memory.
	popq	%rbp		# restore old frame pointer (the caller function frame)
	ret			# return to caller function (OS).		






.J50:    # 50/60
    movq    $format5060, %rdi   # first arg

    movq    $0, %rcx            # getting the first byte for arg2 
    movq    (%rsi),%rsi
    movb    %sil, %cl
    movq    $0, %rsi
    movq    %rcx,%rsi

    movq    $0, %rcx            # getting the first byte for arg3
    movq    (%rdx),%rdx
    movb    %dl, %cl
    movq    $0, %rdx
    movq    %rcx,%rdx
    
    movq    $0, %rax
    call   printf
    leave
    ret


.J52:    # 52    
    leave
    ret
.J53:    # 53
.J54:    # 54
.J55:    # 55



.Jin:    # 51 invalid default
    movq    $formatInvalid, %rdi
    movq	$0, %rax
    call	printf
    leave
    ret

