	#This is a simple program that calculates stuff
	.data
a:	.quad	1000
b:	.word	0x10
c:	.byte	3

	.section	.rodata			#read only data section
format1:	.string	"%d + 0x%x = %d\n"	#c+b
format2:	.string	"%ld*2^%d = %ld\n"	#a*2^c
	########
	.text	#the beginnig of the code
.globl	main	#the label "main" is used to state the initial point of this program
	.type	main, @function	# the label "main" representing the beginning of a function
main:	# the main function:
	pushq	%rbp		#save the old frame pointer
	movq	%rsp, %rbp	#create the new frame pointer
	pushq	%rbx		#saving a callee save register.

	movq	$b, %rax	#geting the address of label "b"
	movq	$0, %rdx	#making sure %rdx will have zero in all its bits.
	movw	(%rax), %dx	#reading one word that starts at this address (reading 0x10).
	movsbq	2(%rax), %rbx	#reading one byte (with sign extension) from 2 bytes after the label "b" (reading 3).
	#notice that "c" was saved in a "callee save register" - will survive the first call to printf.

	movq	%rdx, %rcx	#%rcx = b
	addq	%rbx, %rcx	#%rcx = b+c

	#passing all the parameters for printf - from the last to the first (FIFO!), note the register usage
	#note that the sum is already in %rcx (4th argument), b is already in %rdx (3rd argument)
	movq	%rbx,%rsi	#passing c
	movq	$format1,%rdi	#the string is the first paramter passed to the printf function.
	movq	$0,%rax
	call	printf		#calling to printf AFTER its arguments are passed (not that many arguments, therefore using registers only).

	#return from printf, start the 2nd calculation:
	#remember the values we had in %rax, %rcx and %rdx are lost.
	# ############
	movq	$a, %rsi	#geting the address of label "a"
	movq	(%rsi), %rsi	#geting a (1000)
	# another option that do the same:
	movq	a, %rsi	#assign a to %rsi (1000)
	# ############
	movb	%bl, %cl	#shift only works with numbers or %cl
	salq	%cl, %rdx	#%rdx = a*2^c

	#passing all the parameters for printf - from the last to the first (FIFO!)
	#note that a is already in %rsi (2nd argument)
	movq	%rdx,%rcx	#passing the result
	movq	%rbx,%rdx	#passing c
	movq	$format2,%rdi	#the string is the first paramter passed to the printf function.
	movq	$0,%rax
	call	printf		#calling to printf AFTER its arguments are passed.

	#return from printf 2nd time - end of program:
	movq	$0, %rax	#return value is zero.
	movq	-8(%rbp), %rbx	#restoring the save register (%rbx) value, for the caller function.
	movq	%rbp, %rsp	#restore the old stack pointer - release all used memory.
	popq	%rbp		#restore old frame pointer (the caller function frame)
	ret			#return to caller function (OS).
