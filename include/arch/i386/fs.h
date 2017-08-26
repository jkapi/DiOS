#ifndef _FS_H_
#define _FS_H_

#include <stdint.h>

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?

#define MAX_FILENAME_SIZE 256

struct dirent;
struct fs_node;

// Function types
typedef uint32_t (*read_fn_t) (struct fs_node*, uint32_t,
	                           uint32_t, unsigned char*);
typedef uint32_t (*write_fn_t) (struct fs_node*, uint32_t,
	                            uint32_t, unsigned char*);
typedef void (*open_fn_t) (struct fs_node*);
typedef void (*close_fn_t) (struct fs_node*);
typedef struct dirent* (*readdir_fn_t) (struct fs_node*, uint32_t);
typedef struct fs_node* (*finddir_fn_t) (struct fs_node*, char*);

// A node in the File System
typedef struct fs_node {
  char name[MAX_FILENAME_SIZE];     // Filename
  uint8_t mask;                     // The permissions mask of this node.
  uint32_t uid;                     // The owning user of this node.
  uint32_t gid;                     // The owning group of this node.
  uint32_t flags;                   // The type of this node.
  uint32_t inode;                   // Device-specific
  uint32_t length;                  // Size of the file, in bytes.
  uint32_t impl;                    // An implementation-defined number.
  struct fs_node* ptr;              // Used by mountpoints and symlinks.

  read_fn_t read_fn;
  write_fn_t write_fn;
  open_fn_t open_fn;
  close_fn_t close_fn;
  readdir_fn_t readdir_fn;
  finddir_fn_t finddir_fn;
} fs_node_t;

// A directory entry
typedef struct dirent {
    char name[MAX_FILENAME_SIZE]; // Filename
    uint32_t inode_num;           // Inode number
} dirent_t;

extern fs_node_t* fs_root; // The root of the filesystem.

// Standard read/write/open/close functions for file nodes.
uint32_t read_fs(fs_node_t* node, uint32_t bytes,
	             uint32_t offset, unsigned char* buffer);
uint32_t write_fs(fs_node_t* node, uint32_t bytes,
	              uint32_t offset, unsigned char* buffer);
void open_fs(fs_node_t* node);
void close_fs(fs_node_t* node);

// readdir_fs is 1-indexed, readdir results only live until next readdir call
dirent_t* readdir_fs(fs_node_t* node, uint32_t index);
fs_node_t* finddir_fs(fs_node_t* node, char* name);

#endif  // _FS_H_
