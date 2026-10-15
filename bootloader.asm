[bits 16]
[org 0x7C00] 
jmp start
msg db 'Hello World!',13,10,0
boot db 'booting...',13,10,0
ermsg db 'Error reading disk!',13,10,0
vgaer db 'erreur vga ',13,10,0
fin:
    jmp $

start:

    xor ax, ax
    mov ds, ax
    mov es, ax

    mov si, msg
    call print_loop

    mov si, boot
    call print_loop
    jmp done

print_loop:
    lodsb
    cmp al, 0
    je printr
    mov ah, 0x0E
    int 0x10
    jmp print_loop
printr:
    ret

done:

    mov ah, 0x02
    mov al, 0x30
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov bx, 0x9000


    int 0x13
    jc erd

    xor ax,ax
    mov es,AX
    mov ds,AX

    mov ax ,0x4f00
    mov di,0x5000
    
    int 0x10

    mov si,[0x5000+0x0e]
    mov ax ,[0x5000+0x10]
    mov gs,ax


    ;mov si,0x5000
bcl:

    

    mov cx,[gs:si]

    xor ax,ax
    mov es,AX
    mov di,0x8000
    


    cmp cx,0xffff
    je fin 



    mov ax,0x4f01
    ;mov cx,0x119
    mov di,0x8000
    int 0x10

    cmp word[0x8000+0x12],1024

    jne skmode

    cmp word[0x8000+0x14],768

    jne skmode

    cmp byte[0x8000+0x19],32

    jne skmode

    jmp setmode

skmode:
    add si,2
    jmp bcl

setmode:
    ;cmp ax,0x004f
    ;jne vaser

    mov eax,[0x8000+40]
    mov [0x500],eax

    mov eax,[0x8000+0x10]
    mov [0x504],eax

    or cx,0x4000
    mov bx,cx

    mov ax,0x4f02
    ;mov bx,0x4119
    int 0x10

    cmp ax,0x004f
    jne vaser

    mov ax,0x1130
    mov bh,0x06
    int 0x10

    xor eax,eax
    mov ax,ES
    shl eax,4
    xor ebx,ebx
    mov bx,bp
    add eax,ebx

    ;mov eax ,[es:bp]
    mov [0x508],eax

    

    cli
    lgdt [gd_descriptor]

    in al,0x92
    or al,2
    out 0x92,al



    mov eax ,cr0
    or eax,1
    mov cr0,eax




    jmp 0x08:init_pm


erd:
    mov al,'t'
    mov ah, 0x0E
    int 0x10
    jmp $

vaser:
    mov si,vgaer
    call print_loop
    jmp $

gdt_start:
    dq 0x0
gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gd_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start




[bits 32 ]

init_pm:
    mov ax ,0x10
    mov ds,ax 
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    mov ebp, 0x90000
    mov esp,ebp



    jmp 0x9000


times 510 - ($ - $$) db 0 
dw 0xAA55 




