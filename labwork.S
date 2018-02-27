  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro  PUSH reg
    addi    $sp,$sp,-4
    sw  \reg,0($sp)
.endm

.macro  POP reg
    lw  \reg,0($sp)
    addi    $sp,$sp,4
.endm

.data
.align 2

.text

.global hexasc
hexasc: # 0x30 -> 0x39 , 0x41 -> 0x46
     andi     $t0, $a0, 0xF
     addi     $v0, $t0, 0x30

     jr       $ra
     nop

.global num2string
num2string: #a0 a1
     PUSH $ra  # SAVE $ra (0)
     PUSH $s0  # SAVE $s0 value (1)
     PUSH $s1 # SAVE $s1 value (2)

     add    $s0, $0, $a0     # MOVE MEM-ADRESS
     andi   $s1, $a1, 0xFFFF # MASKING 16 LSB
timeMain:
     move   $a0, $s1  # 1st 'SECOND' DIG.
     jal    hexasc
     nop
     sb     $v0, 4($s0)   # 4 BITS DONE

     srl    $s1, $s1, 4 # 2nd SECOND DIG. // THIS CODE HANDLES '5'
     move   $a0, $s1
     jal    hexasc
     nop
     sb    $v0, 3($s0) # 8 BITS DONE     (3)


     li     $t0, 0x3A
     sb     $t0, 2($s0)   # ADDING DELIMITER (2)


     srl    $s1, $s1, 4   # 1st MIN DIG.
     move   $a0, $s1
     jal    hexasc
     nop
     sb     $v0, 1($s0)   # 12 BITS DONE  (1)

     srl    $s1, $s1, 4  # 2nd MIN DIG
     move   $a0, $s1
     jal    hexasc
     nop
     sb     $v0, 0($s0)   # 16 BITS DONE

     li     $t0, 0x00
     sb     $t0, 5($s0) # ADDING NULL BITS


     POP $s1 # RESTORE $s1 value (2)
     POP $s0  # RESTORE $s0 value (1)
     POP $ra # RESTORE $ra (0)

     jr $ra
     nop