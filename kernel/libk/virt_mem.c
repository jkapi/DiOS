#include <string.h>
#include <libk/virt_mem.h>

bool alloc_page(pt_entry* entry) {
  void* addr = alloc_block();
  if (!addr) {
    return false;
  }

  pt_entry_set_frame(entry, (physical_addr) addr);
  pt_entry_add_attrib(entry, I86_PTE_PRESENT);
  return true;
}

void free_page(pt_entry* entry) {
  void* block = (void*) pt_entry_frame(*entry);
  if (block) {
    free_block(block);
  }

  pt_entry_del_attrib(entry, I86_PTE_PRESENT);
}

void map_page(void* physical_addr, void* virtual_addr) {
  pd_entry* entry = pdirectory_lookup_entry(cur_directory, virtual_addr);
  if (!pd_entry_is_present(*entry)) {
    // Page Directory Entry not present, allocate it
    page_table* table = (page_table*) alloc_block();
    if (!table)
      return;

    // Clear the newly allocated page
    memset(table, 0, sizeof(page_table));

    // Maps the Page Directory Entry to the new table
    pd_entry_add_attrib(entry, I86_PDE_PRESENT);
    pd_entry_add_attrib(entry, I86_PDE_WRITABLE);
    pd_entry_set_frame(entry, table);
  }

   // Get table address from entry, guaranteed to be set now
   page_table* table = (page_table*) PAGE_GET_PHYSICAL_ADDRESS(entry);

   // Get page table entry
   pt_entry* page = ptable_lookup_entry(table, virtual_addr);

    // Maps the Page Table Entry to the given physical address
   pt_entry_set_frame(page, physical_addr);
   pt_entry_add_attrib(page, I86_PTE_PRESENT);
}

uint32_t virt_to_phys(virtual_addr addr) {
  pd_entry* pd_entry = pdirectory_lookup_entry(cur_directory, addr);
  if (!pd_entry)
    return -1;

  pt_entry* pt_entry = ptable_lookup_entry(cur_directory, addr);
  if (!pt_entry)
    return -2;

  printf("%lx %lx\n", pd_entry, pt_entry);
  return pt_entry_frame(pt_entry);
}

void paging_install() {
   // Allocates default page table
  page_table* table = (page_table*) alloc_block();
  if (!table)
     return;

   // Allocates 3GB page table
   page_table* table2 = (page_table*) alloc_block();
   if (!table2)
      return;

  // Clear allocated page tables
  memset(table, 0, sizeof(page_table));
  memset(table2, 0, sizeof(page_table));

  // Maps first 1MB to 3GB+1MB
  for (int frame = 0x0, virt = 0xC0000000; frame < 0x100000;
      frame += 4096, virt += 4096) {

    pt_entry page = 0;
    pt_entry_add_attrib(&page, I86_PTE_PRESENT);
    pt_entry_set_frame(&page, frame);

    table->m_entries[PAGE_TABLE_INDEX(virt)] = page;
  }

  // Maps kernel to start from 3GB+1MB
  for (int i = 0, frame = 0x100000, virt = 0xC0100000; i < 1024;
    i++, frame += 4096, virt += 4096) {

    pt_entry page = 0;
    pt_entry_add_attrib(&page, I86_PTE_PRESENT);
    pt_entry_set_frame(&page, frame);

    table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
  }

  // Create default directory table
  cur_directory = (page_directory*) alloc_blocks(3);
  if (!cur_directory)
    return;

  memset(cur_directory, 0, sizeof(page_directory));

  pd_entry* entry = pdirectory_lookup_entry(cur_directory, 0x00000000);
  pd_entry_add_attrib(entry, I86_PDE_PRESENT);
  pd_entry_add_attrib(entry, I86_PDE_WRITABLE);
  pd_entry_set_frame(entry, (physical_addr) table);

  pd_entry* entry2 = pdirectory_lookup_entry(cur_directory, 0xC0100000);
  pd_entry_add_attrib(entry2, I86_PDE_PRESENT);
  pd_entry_add_attrib(entry2, I86_PDE_WRITABLE);
  pd_entry_set_frame(entry2, (physical_addr) table2);

  enable_paging((uint32_t) cur_directory);
  printf("Paging installed.\n");
}