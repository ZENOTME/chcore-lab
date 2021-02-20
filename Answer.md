


## E4:<br>
- No!LMA(load memory address),at the beginning,the machine didn't use mmu,so the LMA is physical address.That may probably cause the diffierent between the VMA and LMA.

- Virtual memory.

## Homework 1 ##
### E6: ###
For Bootloader:<br>
- SP set in boot\start.S.
- in bss.
- define a global char array.

For kernel:<br>
- SP set in kernel\head.S
- in bss.
- define a global char array.
- FP(x29)didn't set(can't find it(I guess that the x29 is reset at the beginning so it's 0

### E7: ###
- x29 x30 x19
- x29 is the FP
- x30 is the return address
- preserve the x29 in old frame

## Homework4 ##
### E1: ###
- spinlock in single-processor will cause deadlock beacause the spinlock will not give up the processor when it spin.

### E2: ###
- The writer couldn't write if the comming of reader is constant. 
- Imposing the limit to the number of reader requirement when the writer is waiting.

### E3: ###
- R>=P*(N-1)+1 
- valid by banker algorithm