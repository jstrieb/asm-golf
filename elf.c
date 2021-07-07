/**
 * Created by Jacob Strieb
 * July 2021
 *
 * Based almost entirely on the documentation for elf(5) accessible via 
 * "man elf"
 */

#include <elf.h>
#include <stdio.h>
#include <string.h>

int main() {
  // Create a zeroed out struct for the whole file
  typedef struct {
    Elf64_Ehdr elf_header;
    Elf64_Phdr prog_header;
    char code[53];
  } exec_t;

  exec_t exec;
  (void)memset((void *)&exec, 0, sizeof(exec));

  // Set the magic elf_header values
  exec.elf_header.e_ident[EI_MAG0] = ELFMAG0;
  exec.elf_header.e_ident[EI_MAG1] = ELFMAG1;
  exec.elf_header.e_ident[EI_MAG2] = ELFMAG2;
  exec.elf_header.e_ident[EI_MAG3] = ELFMAG3;
  exec.elf_header.e_ident[EI_CLASS] = ELFCLASS64;
  exec.elf_header.e_ident[EI_DATA] = ELFDATA2LSB;
  exec.elf_header.e_ident[EI_VERSION] = EV_CURRENT;
  exec.elf_header.e_ident[EI_OSABI] = ELFOSABI_LINUX;

  exec.elf_header.e_type = ET_EXEC;
  exec.elf_header.e_machine = EM_X86_64;
  exec.elf_header.e_version = EV_CURRENT;

  exec.elf_header.e_ehsize = sizeof(exec.elf_header);

  exec.elf_header.e_shstrndx = SHN_UNDEF;

  // Set offsets to start the program
  exec.elf_header.e_entry = 0x400000 + (void *)&exec.code - (void *)&exec;

  exec.elf_header.e_phoff = (void *)&exec.prog_header - (void *)&exec;
  exec.elf_header.e_phentsize = sizeof(exec.prog_header);
  exec.elf_header.e_phnum = 1;

  exec.prog_header.p_type = PT_LOAD;
  exec.prog_header.p_offset = (void *)&exec.code - (void *)&exec;
  exec.prog_header.p_offset = 0;
  exec.prog_header.p_vaddr = 0x400000;
  exec.prog_header.p_filesz = sizeof(exec);
  exec.prog_header.p_memsz = sizeof(exec);
  exec.prog_header.p_flags = PF_X | PF_R;
  exec.prog_header.p_align = 0x1000;

  // Include the code itself
  char code[] = "\x48\x83\xec\x08"                   // sub    $0x8,%rsp
                "\xc7\x44\x24\xf8\x34\x32\x0a\x00"   // movl   $0xa3234,-0x8(%rsp)
                "\x48\xc7\xc7\x01\x00\x00\x00"       // mov    $0x1,%rdi
                "\x48\x8d\x74\x24\xf8"               // lea    -0x8(%rsp),%rsi
                "\x48\xc7\xc2\x03\x00\x00\x00"       // mov    $0x3,%rdx
                "\x48\xc7\xc0\x01\x00\x00\x00"       // mov    $0x1,%rax
                "\x0f\x05"                           // syscall
                "\x48\x31\xff"                       // xor    %rdi,%rdi
                "\x48\xc7\xc0\x3c\x00\x00\x00"       // mov    $0x3c,%rax
                "\x0f\x05"                           // syscall
                ;
  (void)memcpy((void *)&exec.code, (void *)&code, sizeof(exec.code));


  // Write the elf file out
  FILE *out = fopen("solution", "w");
  (void)fwrite((void *)&exec, sizeof(char), sizeof(exec), out);
  (void)fclose(out);

  printf("ELF header size: %lu\n", sizeof(exec.elf_header));
  printf("Program header size: %lu\n", sizeof(exec.prog_header)); 
  printf("Code size: %lu\n", sizeof(exec.code)); 
  printf("Total size (with padding for alignment): %lu\n", sizeof(exec));

  return 0;
}
