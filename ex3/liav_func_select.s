# 315870345 Liav Trabelsy
.section .rodata #read only data section
#strings for printf and scanf
first_print:
    .string "first pstring length: %d, "
sec_print:
    .string "second pstring length: %d\n"
swap_char:
    .string "old char: %c, new char: %c, first string: %s, second string: %s\n"
invalid:
    .string "invalid option!\n"
char_scf:
    .string " %c"
int_scf:
    .string "%hhu"
case_53_54_print:
    .string "length: %d, string: %s\n"
case_55_print:
    .string "compare result: %d\n"
.align 8

#jump table for switch case
.Jmp_table:
   .quad .Case_50_60 # Case 50
   .quad .Def_case # Case 51
   .quad .Case_52 # Case 52
   .quad .Case_53 # Case 53
   .quad .Case_54 # Case 54
   .quad .Case_55 # Case 55
   .quad .Def_case # Case 56
   .quad .Def_case # Case 57
   .quad .Def_case # Case 58
   .quad .Def_case # Case 59
   .quad .Case_50_60 # Case 60

.text
#functions
.global run_func
#*********************************************************************************
#run_func
#input: rdi - user option, rsi - pointer for pstring1, rdx - pointer for pstring1.
#output: go to the case in the jump table according to the user choice
#*********************************************************************************
run_func:
    leaq -50(%rdi),%rdi         # Compute xi = x-50
    cmpq $10,%rdi               # Compare xi:10
    ja .Def_case                # if >, goto default-case
    jmp *.Jmp_table(,%rdi,8)    # Goto jt[xi]

    .Case_50_60:
        pushq %r12              #temp reg to keep length
        pushq %r13

        movq %rsi, %rdi         #call pstrlen
        call pstrlen
        movq %rax, %r12

        movq %rdx, %rdi
        call pstrlen
        movq %rax, %r13

        xor %rax, %rax          #print the length of each pstr
        movq $first_print, %rdi
        movq %r12, %rsi
        call printf

        xor %rax, %rax
        movq $sec_print, %rdi
        movq %r13, %rsi
        call printf

        popq %r13               #pop the temp reg
        popq %r12
        ret
    .Case_52:
        pushq %rbp              #temp reg to keep the scanf and the pstr
        movq %rsp, %rbp
        sub $16, %rsp

        pushq %r12
        pushq %r13

        movq %rsi, %r12
        movq %rdx, %r13

        xor %rax, %rax          #scan the 2 char
        movq $char_scf, %rdi
        leaq 0(%rsp), %rsi
        call scanf

        xor %rax, %rax
        movq $char_scf, %rdi
        leaq 8(%rsp), %rsi
        call scanf

        xor %rax, %rax          #call replaceChar
        movq %r12, %rdi
        movq 0(%rsp), %rsi
        movq 8(%rsp), %rdx
        call replaceChar
        leaq 1(%rax), %rcx         #save the return value

        xor %rax, %rax          #call replaceChar
        movq %r13, %rdi
        movq 0(%rsp), %rsi
        movq 8(%rsp), %rdx
        call replaceChar
        leaq 1(%rax), %r8          #save the return value

        xor %rax, %rax          #print the 2 new pstr
        movq $swap_char, %rdi
        movq 0(%rsp), %rsi
        movq 8(%rsp), %rdx
        call printf

        popq %r13               #free the temp reg and the scanf place
        popq %r12
        movq %rbp, %rsp
        popq %rbp
        ret
    .Case_53:
        pushq %rbp              #temp reg to keep the scanf and the pstr
        movq %rsp, %rbp
        sub $16, %rsp

        pushq %r12
        pushq %r13

        movq %rsi, %r12
        movq %rdx, %r13

        xor %rax, %rax          #scan the i'th index
        movq $int_scf, %rdi
        leaq 0(%rsp), %rsi
        call scanf


        xor %rax, %rax          #scan the j'th index
        movq $int_scf, %rdi
        leaq 8(%rsp), %rsi
        call scanf

        xor %rdx, %rdx          #call pstrijcpy
        movb (%rsp), %dl
        xor %rcx, %rcx
        movb 8(%rsp), %cl
        movq %r12, %rdi
        movq %r13, %rsi
        call pstrijcpy

        movq %rax, %rdx         #print the case format for 1st pstr
        xor %rax, %rax
        movq $case_53_54_print, %rdi
        movzbq (%r12), %rsi
        leaq 1(%rdx), %rdx
        call printf

        xor %rax, %rax          #print the case format for 2nd pstr
        movq $case_53_54_print, %rdi
        movzbq (%r13), %rsi
        movq %r13, %rdx
        leaq 1(%rdx), %rdx
        call printf

        popq %r13               #free the temp reg and the scanf place
        popq %r12
        movq %rbp, %rsp
        popq %rbp
        ret
    .Case_54:
        pushq %r12              #temp reg to keep pstr
        pushq %r13

        movq %rsi, %r12
        movq %rdx, %r13

        movq %r12, %rdi         #call swap case and save the new pstr in %r12
        call swapCase
        movq %rax, %r12

        xor %rax, %rax          #print the first new pstr
        movq $case_53_54_print, %rdi
        movzbq (%r12), %rsi
        movq %r12, %rdx
        leaq 1(%rdx), %rdx
        call printf

        movq %r13, %rdi         #call swap case and save the new pstr in %r13
        call swapCase
        movq %rax, %r13

        xor %rax, %rax          #print the second new pstr
        movq $case_53_54_print, %rdi
        movzbq (%r13), %rsi
        movq %r13, %rdx
        leaq 1(%rdx), %rdx
        call printf

        popq %r13               #free the temp reg
        popq %r12
        ret
    .Case_55:
        pushq %rbp              #temp reg to keep the scanf and the pstr
        movq %rsp, %rbp
        sub $16, %rsp

        pushq %r12
        pushq %r13

        movq %rsi, %r12
        movq %rdx, %r13

        xor %rax, %rax          #scan the i'th index
        movq $int_scf, %rdi
        leaq 0(%rsp), %rsi
        call scanf

        xor %rax, %rax          #scan the j'th index
        movq $int_scf, %rdi
        leaq 8(%rsp), %rsi
        call scanf

        xor %rdx, %rdx          #call pstrijcmp
        movb (%rsp), %dl
        xor %rcx, %rcx
        movb 8(%rsp), %cl
        movq %r12, %rdi
        movq %r13, %rsi
        call pstrijcmp

        xor %rsi, %rsi          #print the case format
        movq %rax, %rsi
        xor %rax, %rax
        movq $case_55_print, %rdi
        call printf

        popq %r13               #free the temp reg and the scanf place
        popq %r12
        movq %rbp, %rsp
        popq %rbp
        ret
    #print invalid option
    .Def_case:
        xor %rax, %rax
        movq $invalid, %rdi
        call printf
        ret



