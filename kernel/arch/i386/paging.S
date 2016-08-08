.global enable_paging
enable_paging:
  # Move directory into CR3
  mov 4(%esp), %eax
  mov %eax, %cr3
  
  # Enable paging
  mov %cr0, %eax
  or $0x80000000, %eax
  mov %eax, %cr0
  ret