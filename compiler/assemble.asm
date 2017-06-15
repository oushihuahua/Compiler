	.data
s0:	.asciiz "*"
s1:	.asciiz "1"
s2:	.asciiz "="
s3:	.asciiz "Should be 1."
s4:	.asciiz "Any number."
s5:	.asciiz "Too big."
s6:	.asciiz "y="

	.text
li	$fp, 60
li	$sp, 60
li	$t0, 60
addi	$sp, $sp, 72
j	entry
# LABlab0 #
lab0:
# WRT"*" #
li	$v0, 4
la	$a0, s0
syscall
# WRTx #
li	$v0, 1
lw	$t2, -16($fp)
lw	$a0, 0($t2)
syscall
# RETprint #
subi	$fp, $fp, 12
move	$sp, $fp
lw	$t0, 0($fp)
lw	$v0, 4($fp)
move	$fp, $t0
sub	$sp, $sp, 4
jr	$ra
# LABlab1 #
lab1:
# ASGqq0 #
li	$t0, 0
sw	$t0, -24($fp)
# RETnull #
subi	$fp, $fp, 16
move	$sp, $fp
lw	$t0, 0($fp)
lw	$v0, 4($fp)
move	$fp, $t0
sub	$sp, $sp, 8
jr	$ra
# LABlab2 #
lab2:
# NQLlab3n1 #
lw	$t2, -4($fp)
lw	$t0, 20($t2)
li	$t1, 1
bne	$t0, $t1, lab3
# RVAL1 #
li	$t0, 1
sw	$t0, -8($fp)
# JMPlab4 #
j	lab4
# LABlab3 #
lab3:
# NQLlab5n2 #
lw	$t2, -4($fp)
lw	$t0, 20($t2)
li	$t1, 2
bne	$t0, $t1, lab5
# RVAL2 #
li	$t0, 2
sw	$t0, -8($fp)
# JMPlab6 #
j	lab6
# LABlab5 #
lab5:
# SAVE #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# NEQ$01 #
li	$t0, 1
neg	$t0, $t0
sw	$t0, 8($fp)
# ADD$1$0n #
lw	$t0, 8($fp)
lw	$t2, -4($fp)
lw	$t1, 20($t2)
add	$t0, $t0, $t1
sw	$t0, 12($fp)
# PUSHf1$1 #
lw	$t0, 12($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALFlab2$2 #
move	$t0, $fp
addi	$sp, $sp, 4
sw	$t0, -4($sp)
addi	$sp, $sp, 4
lw	$t1, -4($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
move	$fp, $sp
addi	$sp, $sp, 32
jal	lab2
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 16($fp)
# SAVE #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# SUB$3n2 #
lw	$t2, -4($fp)
lw	$t0, 20($t2)
li	$t1, 2
sub	$t0, $t0, $t1
sw	$t0, 20($fp)
# PUSHf1$3 #
lw	$t0, 20($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALFlab2$4 #
move	$t0, $fp
addi	$sp, $sp, 4
sw	$t0, -4($sp)
addi	$sp, $sp, 4
lw	$t1, -4($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
move	$fp, $sp
addi	$sp, $sp, 32
jal	lab2
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 24($fp)
# ADD$5$2$4 #
lw	$t0, 16($fp)
lw	$t1, 24($fp)
add	$t0, $t0, $t1
sw	$t0, 28($fp)
# RVAL$5 #
lw	$t0, 28($fp)
sw	$t0, -8($fp)
# LABlab6 #
lab6:
# LABlab4 #
lab4:
# RETf #
subi	$fp, $fp, 12
move	$sp, $fp
lw	$t0, 0($fp)
lw	$v0, 4($fp)
move	$fp, $t0
sub	$sp, $sp, 4
jr	$ra
# LABlab7 #
lab7:
# NQLlab8num0 #
lw	$t0, -16($fp)
li	$t1, 0
bne	$t0, $t1, lab8
# RVAL1 #
li	$t0, 1
sw	$t0, -8($fp)
# WRT"1" #
li	$v0, 4
la	$a0, s1
syscall
# JMPlab9 #
j	lab9
# LABlab8 #
lab8:
# SAVE #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# SUB$6num1 #
lw	$t0, -16($fp)
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 0($fp)
# PUSHfac1$6 #
lw	$t0, 0($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALFlab7$7 #
move	$t0, $fp
addi	$sp, $sp, 4
sw	$t0, -4($sp)
addi	$sp, $sp, 4
lw	$t1, -4($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
move	$fp, $sp
addi	$sp, $sp, 12
jal	lab7
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 4($fp)
# MUL$8num$7 #
lw	$t0, -16($fp)
lw	$t1, 4($fp)
mul	$t0, $t0, $t1
sw	$t0, 8($fp)
# RVAL$8 #
lw	$t0, 8($fp)
sw	$t0, -8($fp)
# SAVE #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# PUSHprint1num #
la	$t0, -16($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALPlab0 #
move	$t0, $fp
addi	$sp, $sp, 4
sw	$t0, -4($sp)
addi	$sp, $sp, 4
lw	$t1, -4($t0)
sw	$t1, 0($sp)
addi	$sp, $sp, 4
move	$fp, $sp
addi	$sp, $sp, 0
jal	lab0
lw	$31, -4($sp)
subi	$sp, $sp, 4
# LABlab9 #
lab9:
# RETfac #
subi	$fp, $fp, 12
move	$sp, $fp
lw	$t0, 0($fp)
lw	$v0, 4($fp)
move	$fp, $t0
sub	$sp, $sp, 4
jr	$ra
# LABentry #
entry:
# REDnum #
li	$v0, 5
syscall
sw	$v0, 8($fp)
# BGTlab10num20 #
lw	$t0, 8($fp)
li	$t1, 20
bgt	$t0, $t1, lab10
# SAVE #
addi	$sp, $sp, 4
sw	$31, -4($sp)
# PUSHfac1num #
lw	$t0, 8($fp)
sw	$t0, 0($sp)
addi	$sp, $sp, 4
# CALFlab7$9 #
move	$t0, $fp
addi	$sp, $sp, 4
sw	$t0, -4($sp)
addi	$sp, $sp, 4
sw	$t0, 0($sp)
addi	$sp, $sp, 4
move	$fp, $sp
addi	$sp, $sp, 12
jal	lab7
lw	$31, -4($sp)
subi	$sp, $sp, 4
sw	$v0, 64($fp)
# ASGw$9 #
lw	$t0, 64($fp)
sw	$t0, 0($fp)
# WRT"=" #
li	$v0, 4
la	$a0, s2
syscall
# WRTw #
li	$v0, 1
lw	$a0, 0($fp)
syscall
# NQLlab12num0 #
lw	$t0, 8($fp)
li	$t1, 0
bne	$t0, $t1, lab12
# WRT"Should be 1." #
li	$v0, 4
la	$a0, s3
syscall
# JMPlab13 #
j	lab13
# LABlab12 #
lab12:
# WRT"Any number." #
li	$v0, 4
la	$a0, s4
syscall
# LABlab13 #
lab13:
# JMPlab11 #
j	lab11
# LABlab10 #
lab10:
# WRT"Too big." #
li	$v0, 4
la	$a0, s5
syscall
# LABlab11 #
lab11:
# ASGy0 #
li	$t0, 0
sw	$t0, 12($fp)
# ASGz10 #
li	$t0, 10
sw	$t0, 16($fp)
# LABlab14 #
lab14:
# SMTlab15z0 #
lw	$t0, 16($fp)
li	$t1, 0
blt	$t0, $t1, lab15
# ADD$10y1 #
lw	$t0, 12($fp)
li	$t1, 1
add	$t0, $t0, $t1
sw	$t0, 68($fp)
# ASGy$10 #
lw	$t0, 68($fp)
sw	$t0, 12($fp)
# SUBzz1 #
lw	$t0, 16($fp)
li	$t1, 1
sub	$t0, $t0, $t1
sw	$t0, 16($fp)
# JMPlab14 #
j	lab14
# LABlab15 #
lab15:
# WRT"y=" #
li	$v0, 4
la	$a0, s6
syscall
# WRTy #
li	$v0, 1
lw	$a0, 12($fp)
syscall
