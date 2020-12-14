.section .rodata
format_d .string "%d"




# Pstring hello = '5' , 'h' , 'e' , 'l' , 'l' , 'o' , '/0'

# getting two pstrings. length. optaionSelect.
# send to run_func optaionSelect. two pstring adresses.
.text
.globl	run_main
	.type	run_main, @function
run_main:
    push    %rbp
    movq    %rsp, %rbp

    sub     $256, %rsp
    movq    $format_d, %rdi
    movq    $0, %rax
    call    scanf

    

    



