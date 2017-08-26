#ifndef _INITRD_H_
#define _INITRD_H_

#include <arch/i386/fs.h>
#include <stddef.h>
#include <stdint.h>

// Header of the Initial Ramdisk
typedef struct {
    size_t num_files;
} initrd_header_t;

// Header of one file in Initial Ramdisk
typedef struct {
    char file_name[MAX_FILENAME_SIZE];
    uint8_t checksum;
    uint32_t offset;   // Offset in the initrd that the file starts.
    uint32_t length;
    uint32_t parent_dir_inode;
} initrd_file_header_t;

// One directory table
typedef struct {
    
} directory_table_t;

// Initialises the Initial Ramdisk from an address to the initrd multiboot
// module. Returns a completed filesystem node.
fs_node_t* load_initrd(uint32_t initrd_module_addr);

// Free memory allocated by the initrd. If nothing us allocated, does nothing.
void delete_initrd();

#endif // _INITRD_H_
