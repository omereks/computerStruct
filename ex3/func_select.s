.data


.section	.rodata	
format_s:       .string "%s \n"
format_d:       .string "%hhu"
format_c:       .string "%c"
formatInvalid:	.string	"invalid option!\n"
format5060:	    .string	"first pstring length: %d, second pstring length: %d\n"
format52:	    .string	"old char: %c, new char: %c, first string: %s, second string: %s\n"
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
    movq    %rsi, %rdi          # getting the first byte for the second arg
    movq    $0, %rax            
    call    pstrlen           
    movq    %rax,%rsi
    
    movq    %rdx, %rdi          # getting the first byte for the third arg
    movq    $0, %rax            
    call    pstrlen           
    movq    %rax,%rdx    

    movq    $format5060, %rdi   # first arg

    movq    $0, %rax
    call   printf
    leave
    ret


.J52:    # 52    
    movq    %rdx, %r15      # kepp pstring2
    movq    %rsi, %r14      # kepp pstring1
  
    sub     $16, %rsp

    movq    $format_c, %rdi
    leaq    -8(%rbp), %rsi
    movq    $0, %rax
    call    scanf           
  
    movq    $format_c, %rdi
    leaq    -8(%rbp), %rsi
    movq    $0, %rax
    call    scanf           

    movq    $format_c, %rdi
    leaq    -16(%rbp), %rsi
    movq    $0, %rax
    call    scanf           
   
    movq    $format_c, %rdi
    leaq    -16(%rbp), %rsi
    movq    $0, %rax
    call    scanf           
     
    # build replace pstring1
    movq    %r14, %rdi      # arg 1 pstring 1     
    movq    -8(%rbp), %rsi  # arg 2 old char
    movq    -16(%rbp), %rdx  # new char arg3
    movq    $0, %rax
    call    replaceChar

 #   movq    %rax, %rsi
  #  movq    $format_s, %rdi
 #   movq    $0, %rax
  #  call    printf
  #  ret

    # build replace pstring1
    movq    %r15, %rdi      # arg 1 pstring 2     
    movq    -8(%rbp), %rsi  # arg 2 old char
    movq    -16(%rbp), %rdx  # new char arg3
    movq    $0, %rax
    call    replaceChar

    # print "old char: %c,new char: %c, first string: %s, second string: %s\n"
    movq    $format52, %rdi     # string
    movq    -8(%rbp), %rsi          # old char
    movq    -16(%rbp), %rdx              # new char
    movq    %r14, %rcx              # first string
    movq    %r15, %r8               # second string
    movq    $0, %rax
    call    printf

    add     $16, %rsp
    leave
    ret




.J53:    # 53

    sub     $16, %rsp

    movq   %rdx, %r14   # pstring 2
    movq   %rsi, %r15   # pstring 1

    # arg 3 - i
    movq    $format_d, %rdi
    leaq    -8(%rbp), %rsi
    movq    $0, %rax
    call    scanf
    xor     %r13, %r13
    movb    -8(%rbp), %r13b

    # arg 4 - j
    movq    $format_d, %rdi
    leaq    -16(%rbp), %rsi
    movq    $0, %rax
    call    scanf
    movq    -16(%rbp), %rcx

    
    
    

    # send to pstrijcpy
    movq    %r15, %rdi  # arg 1
    movq    %r14, %rsi  # arg 2
    movb    %r13b, %dl
    movq    $0, %rax
    call    pstrijcpy
    
    # getting pstring 1 length
    movq    %r15, %rdi          
    movq    $0, %rax            
    call    pstrlen
    movq    $0, %rsi           
    movq    %rax,%rsi
    
    
    # buildprint1   "length: %d, string: %s\n"
    movq    $format5354, %rdi
    movq    %r15, %rdx
    call printf


    # getting pstring 2 length
    movq    %r14, %rdi          
    movq    $0, %rax            
    call    pstrlen
    movq    $0, %rsi           
    movq    %rax,%rsi

    # buildprint2   "length: %d, string: %s\n"
    movq    $format5354, %rdi
    movq    %r14, %rdx
    call printf
    
    leave
    ret

     movq    $format_d, %rdi
    movq    %rcx, %rsi
    movq    $0, %rax
    call    printf
    leave
    ret
    

.J54:    # 54
.J55:    # 55



.Jin:    # 51 invalid default
    movq    $formatInvalid, %rdi
    movq	$0, %rax
    call	printf
    leave
    ret

