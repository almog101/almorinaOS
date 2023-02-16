section .text
    global reboot

reboot:
    PUSH    0FFFFh
    PUSH    0000h
    RETF
    ret