	.data
s0:	.asciiz "Input two letters "
s1:	.asciiz " After m "
s2:	.asciiz " After M "
s3:	.asciiz " Invalid "

	.text
li	$fp, 60
li	$sp, 60
li	$t0, 60
addi	$sp, $sp, 88
j	entry
# LABentry #
entry:
# ASGi11 #
li	$t0, 1
sw	$t0, 8($fp)
# ASGi22 #
li	$t0, 2
sw	$t0, 12($fp)
# ASGvalid0 #
li	$t0, 0
sw	$t0, 24($fp)
# ASGi1 #
li	$t0, 1
sw	$t0, 28($fp)
# LABlab0 #
lab0:
# BGTlab1i3 #
lw	$t0, 28($fp)
li	$t1, 3
bgt	$t0, $t1, lab1
# WRT"Input two letters " #
li	$v0, 4
la	$a0, s0
syscall
# REDc1 #
li	$v0, 12
syscall
sw	$v0, 0($fp)
# REDc2 #
li	$v0, 12
syscall
sw	$v0, 4($fp)
# SMTlab2c1a #
lw	$t0, 0($fp)
li	$t1, 97
blt	$t0, $t1, lab2
# BGTlab4c1m #
lw	$t0, 0($fp)
li	$t1, 109
bgt	$t0, $t1, lab4
# SUB$0c1a #
lw	$t0, 0($fp)
li	$t1, 97
sub	$t0, $t0, $t1
sw	$t0, 32($fp)
# ADD$1$097 #
lw	$t0, 32($fp)
li	$t1, 97
add	$t0, $t0, $t1
sw	$t0, 36($fp)
# ASGordc1$1 #
lw	$t0, 36($fp)
sw	$t0, 20($fp)
# WRTordc1 #
li	$v0, 1
lw	$a0, 20($fp)
syscall
# JMPlab5 #
j	lab5
# LABlab4 #
lab4:
# WRT" After m " #
li	$v0, 4
la	$a0, s1
syscall
# LABlab5 #
lab5:
# JMPlab3 #
j	lab3
# LABlab2 #
lab2:
# SMTlab6c1A #
lw	$t0, 0($fp)
li	$t1, 65
blt	$t0, $t1, lab6
# BGTlab8c1M #
lw	$t0, 0($fp)
li	$t1, 77
bgt	$t0, $t1, lab8
# SUB$2c1A #
lw	$t0, 0($fp)
li	$t1, 65
sub	$t0, $t0, $t1
sw	$t0, 40($fp)
# ADD$3$265 #
lw	$t0, 40($fp)
li	$t1, 65
add	$t0, $t0, $t1
sw	$t0, 44($fp)
# ASGordc1$3 #
lw	$t0, 44($fp)
sw	$t0, 20($fp)
# WRTordc1 #
li	$v0, 1
lw	$a0, 20($fp)
syscall
# JMPlab9 #
j	lab9
# LABlab8 #
lab8:
# WRT" After M " #
li	$v0, 4
la	$a0, s2
syscall
# LABlab9 #
lab9:
# JMPlab7 #
j	lab7
# LABlab6 #
lab6:
# WRT" Invalid " #
li	$v0, 4
la	$a0, s3
syscall
# ASGvalid1 #
li	$t0, 1
sw	$t0, 24($fp)
# LABlab7 #
lab7:
# LABlab3 #
lab3:
# BGTlab10c1c2 #
lw	$t0, 0($fp)
lw	$t1, 4($fp)
bgt	$t0, $t1, lab10
# MUL$4ai1 #
li	$t0, 97
lw	$t1, 8($fp)
mul	$t0, $t0, $t1
sw	$t0, 48($fp)
# MUL$5mi2 #
li	$t0, 109
lw	$t1, 12($fp)
mul	$t0, $t0, $t1
sw	$t0, 52($fp)
# ADD$6$4$5 #
lw	$t0, 48($fp)
lw	$t1, 52($fp)
add	$t0, $t0, $t1
sw	$t0, 56($fp)
# ADD$7am #
li	$t0, 97
li	$t1, 109
add	$t0, $t0, $t1
sw	$t0, 60($fp)
# DIV$8$6$7 #
lw	$t0, 56($fp)
lw	$t1, 60($fp)
div	$t0, $t0, $t1
sw	$t0, 64($fp)
# ASGi3$8 #
lw	$t0, 64($fp)
sw	$t0, 16($fp)
# JMPlab11 #
j	lab11
# LABlab10 #
lab10:
# MUL$9ai2 #
li	$t0, 97
lw	$t1, 12($fp)
mul	$t0, $t0, $t1
sw	$t0, 68($fp)
# MUL$10mi1 #
li	$t0, 109
lw	$t1, 8($fp)
mul	$t0, $t0, $t1
sw	$t0, 72($fp)
# ADD$11$9$10 #
lw	$t0, 68($fp)
lw	$t1, 72($fp)
add	$t0, $t0, $t1
sw	$t0, 76($fp)
# SUB$12am #
li	$t0, 97
li	$t1, 109
sub	$t0, $t0, $t1
sw	$t0, 80($fp)
# DIV$13$11$12 #
lw	$t0, 76($fp)
lw	$t1, 80($fp)
div	$t0, $t0, $t1
sw	$t0, 84($fp)
# ASGi3$13 #
lw	$t0, 84($fp)
sw	$t0, 16($fp)
# LABlab11 #
lab11:
# NQLlab12valid0 #
lw	$t0, 24($fp)
li	$t1, 0
bne	$t0, $t1, lab12
# WRTi3 #
li	$v0, 1
lw	$a0, 16($fp)
syscall
# JMPlab13 #
j	lab13
# LABlab12 #
lab12:
# WRT0 #
li	$v0, 1
li	$a0, 0
syscall
# LABlab13 #
lab13:
# ADDii1 #
lw	$t0, 28($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 28($fp)
# JMPlab0 #
j	lab0
# LABlab1 #
lab1:
