.global _start

_start:
  # Load "42\n\x00" onto the stack
  sub $0x8, %rsp
  movl $0x000a3234, -0x8(%rsp)

  # Set write arguments
  mov $0x1, %rdi          # Write to stdout
  lea -0x8(%rsp), %rsi    # Print chars from stack
  mov $0x3, %rdx          # Write 3 bytes

  # Call write to print to stdout
  mov $0x1, %rax          # SYS_write == 1
  syscall

  # Exit with code 0
  xor %rdi, %rdi
  mov $60, %rax           # SYS_exit == 60
  syscall
