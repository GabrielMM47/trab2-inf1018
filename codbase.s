pushq %rbp
movq %rsp, %rbp
subq $16, %rsp

movq $0xffff10ffffffffff, %rdi

movq %r9, %rdi

leave
ret