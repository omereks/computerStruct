
	.section	.rodata	
formatInvalid:	.string	"invalid option!\n"
format5060:	    .string	"first pstring length: %d, second pstring length: %d\n"
format52:	    .string	"old char: %c,new char: %c, first string: %s, second string: %s\n"
format5354:	    .string	"length: %d, string: %s\n"
format55:	    .string	"compare result: %d\n"

.Jtable:
    .long   .J50    # 50/60
    .long   .Jin    # 51 invalid
    .long   .J52    # 52    
    .long   .J53    # 53
    .long   .J54    # 54
    .long   .J55    # 55
    .long   .Jin     # 56 invalid
    .long   .Jin    # 57 invalid
    .long   .Jin    # 58 invalid
    .long   .Jin    # 59 invalid
    .long   .J50    # 50/60


	########


.text	                    # the beginnig of the code
.globl	run_func	        # the label "main" is used to state the initial point of this program
.type	run_func, @function	# the label "main" representing the beginning of a function


run_func:	# the main function:
	pushq	%rbp		# save the old frame pointer
	movq	%rsp, %rbp	# create the new frame pointer
	pushq	%rbx		# saving a callee save register.

	# rdi is thr option number 50/60 52 53 54 55 else
    # rsi is pstring1
    # rdx is pstring2
    pushq   %rdi
    subq    $50, %rdi
    cmpq    $10, %rdi
    ja      .Jin
    jmp     *.Jtable(,%rdi,4)
    

	movq	%rbp, %rsp	# restore the old stack pointer - release all used memory.
	popq	%rbp		# restore old frame pointer (the caller function frame)
	ret			# return to caller function (OS).		


.J50:    # 50/60
.J52:    # 52    
.J53:    # 53
.J54:    # 54
.J55:    # 55
.Jin:    # 51 invalid default
    movq    $formatInvalid, %rdi
    movq	$0, %rax
    call	printf
    leave
    ret

