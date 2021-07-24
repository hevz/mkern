/* start.S
 * Heiher <r@hev.cc>
 */

#define CORE_BUF_BASE    0x800000001fe01000

    .text

    .align  2
    .global start
    .type   start, @function
start:
    /* Clear BSS */
    la.abs  $a0, _edata
    la.abs  $a2, _end
    beq     $a0, $a2, 2f
1:  st.d    $zero, $a0, 0
    addi.d  $a0, $a0, 4
    bne     $a2, $a0, 1b
2:

    # core 0
    dli     $t0, CORE_BUF_BASE
    st.d    $zero, $t0, 0x38
    dbar    0
    la.abs  $sp, _sp0

    # core 1
    dli     $t0, CORE_BUF_BASE + 0x100
    la.abs  $t1, _sp1
    st.d    $t1, $t0, 0x28
    dbar    0
    la.abs  $t1, _entry
    st.d    $t1, $t0, 0x20

    # core 2
    dli     $t0, CORE_BUF_BASE + 0x200
    la.abs  $t1, _sp2
    st.d    $t1, $t0, 0x28
    dbar    0
    la.abs  $t1, _entry
    st.d    $t1, $t0, 0x20

    # core 3
    dli     $t0, CORE_BUF_BASE + 0x300
    la.abs  $t1, _sp3
    st.d    $t1, $t0, 0x28
    dbar    0
    la.abs  $t1, _entry
    st.d    $t1, $t0, 0x20

_entry:
    csrrd   $a0, 0x20
    bl      cpu_main

1:  b       1b

    .section .bss

    .align  3
    .space  0x10000
_sp0:
    .space  0x10000
_sp1:
    .space  0x10000
_sp2:
    .space  0x10000
_sp3:
