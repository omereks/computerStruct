# 315870345 Liav Trabelsy
.section .rodata
#string for printf
invalid_input:
    .string "invalid input!\n"

.text
#functions
.global pstrlen
.global replaceChar
.global pstrijcpy
.global swapCase
.global pstrijcmp

#******************************************
#pstrlen (case 50/60)
#input: rdi - pstring str.
#output: return the length of the pstring.
#******************************************

pstrlen:
    movzbq (%rdi), %rax         #move the byte of the size to rax
    ret

#******************************************************************
#replaceChar (case 52)
#input: rdi - pstring str, rsi - char oldchar, rdx - char newchar.
#output: change each old char to new char.
#******************************************************************

replaceChar:
    movq %rdi, %rax             #keep rdi in rax for the return
     .WhileLoop1: #while
        leaq 1(%rdi), %rdi      #skip the length byte
        cmpb $0, (%rdi)         #check if ptr[i] is \0
        je .EndLoop1
        cmpb (%rdi), %sil       #check if pstr[i] is old char and replace to new char
        jne .WhileLoop1
        movb %dl, (%rdi)
        jmp .WhileLoop1
     .EndLoop1:
        ret

#**************************************************************************************
#pstrijcpy(case 53)
#input: rdi - pstring str2, rsi - pstring str1, rdx - char index i, rcx - char index j.
#output: copy from str2 to str1 from index i to index j.
#**************************************************************************************

pstrijcpy:
    pushq %rdi                  #save dst pstr

    cmpb %dl, %cl               #check for invalid cases
    jb .Invalid1
    cmpb $0, %cl
    jb .Invalid1
    cmpb $0, %dl
    jb .Invalid1
    cmpb %cl, (%rdi)
    jbe .Invalid1
    cmpb %cl, (%rsi)
    jbe .Invalid1

    popq %rdi                   #move dst ptr in rax(ret value)
    movq %rdi, %rax

    leaq 1(%rdi,%rdx), %rdi     #move dst and src pstr to i'th place
    leaq 1(%rsi,%rdx), %rsi
    jmp .Loop2
    #print invalid
    .Invalid1:
        xor %rax, %rax
        movq $invalid_input, %rdi
        call printf
        xor %rax, %rax
        popq %rax
        ret
    .Loop2:
        movb (%rsi), %r8b       #dst pstr[i] = src pstr[i]
        movb %r8b,(%rdi)
        add $1, %dl             #add 1 to i
        leaq 1(%rdi), %rdi      #promote pstr by 1 byte
        leaq 1(%rsi), %rsi      #promote pstr by 1 byte
        cmpb %cl, %dl           #check if i <= j
        jbe .Loop2
    ret

#***************************************************************
#swapCase (case 54)
#input: rdi - pstring str.
#output: change small letter to capital letter and the opposite
#***************************************************************
swapCase:
    movq %rdi, %rax
    .WhileLoop3: #while
        leaq 1(%rdi), %rdi      #skip the length byte
        cmpb $0, (%rdi)         #check if ptr[i] is \0
        je .EndLoop3
    #check if the current char is a capital letter
    .Capital_check:
        cmpb $65, (%rdi)
        jl .WhileLoop3
        cmpb $90, (%rdi)
        ja .Small_check

        addb $32, (%rdi)        #change to small letter
        jmp .WhileLoop3
    #check if the current char is a small letter
    .Small_check:
        cmpb $97, (%rdi)
        jl .WhileLoop3
        cmpb $122, (%rdi)
        ja .WhileLoop3

        subb $32, (%rdi)        #change to capital letter
        jmp .WhileLoop3
    .EndLoop3:
        ret

#**************************************************************************************
#pstrijcmp (case 55)
#input: rdi - pstring str2, rsi - pstring str1, rdx - char index i, rcx - char index j.
#output: compare the 2 pstr from i'th index to j'th
#        1 if pstr1>pstr2, 0 if equal and -1 pstr1<pstr2.
#**************************************************************************************
pstrijcmp:
    pushq %rdi                  #save dst pstr

    cmpb %dl, %cl               #check for invalid cases
    jb .Invalid2
    cmpb $0, %cl
    jb .Invalid2
    cmpb $0, %dl
    jb .Invalid2
    cmpb %cl, (%rdi)
    jbe .Invalid2
    cmpb %cl, (%rsi)
    jbe .Invalid2

    popq %rdi
    leaq 1(%rdi,%rdx), %rdi     #move dst and src pstr to i'th place
    leaq 1(%rsi,%rdx), %rsi
    jmp .Loop3
    #print invalid
    .Invalid2:
        xor %rax, %rax
        movq $invalid_input, %rdi
        call printf
        xor %rax, %rax
        popq %rax

        movq $-2, %rax          #return -2
        ret
    .Loop3:
        movb (%rsi), %r8b       #cmp pstr2[i] with pstr1[i]
        cmpb (%rdi), %r8b       #if bigger
        ja .Sec_bigger
        cmpb (%rdi), %r8b       #if smaller
        jl .First_bigger
        add $1, %dl             #add 1 to i
        leaq 1(%rdi), %rdi      #promote pstr by 1 byte
        leaq 1(%rsi), %rsi      #promote pstr by 1 byte
        cmpb %cl, %dl           #check if i <= j
        jbe .Loop3
        movq $0, %rax
        ret
    #return -1 if second bigger
    .Sec_bigger:
        movq $-1, %rax
        ret
    #return 1 if first bigger
    .First_bigger:
        movq $1, %rax
        ret




