


## E4:<br>
- No!LMA(load memory address),at the beginning,the machine didn't use mmu,so the LMA is physical address.That may probably cause the diffierent between the VMA and LMA.

- Virtual memory.


## E6:<br>
For Bootloader:<br>
- SP set in boot\start.S.
- in bss.
- define a global char array.

For kernel:<br>
- SP set in kernel\head.S
- in bss.
- define a global char array.
- FP(x29)didn't set(can't find it(I guess that the x29 is reset at the beginning so it's 0

## E7:<br>
- x29 x30 x19
- x29 is the FP
- x30 is the return address
- preserve the x29 in old frame
