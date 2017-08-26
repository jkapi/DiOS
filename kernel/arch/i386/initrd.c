#include <arch/i386/fs.h>
#include <arch/i386/initrd.h>
#include <libk/heap.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Holds the state of the initrd.
initrd_file_header_t* file_headers; // The list of file headers.
fs_node_t* initrd_root;             // Our root directory node.
fs_node_t* initrd_dev;              // Director for /dev, so we can mount devfs
fs_node_t* nodes;                   // List of file nodes.

size_t num_files;                   // Number of file nodes.

dirent_t dirent;

static uint32_t initrd_read_fn(fs_node_t* node, uint32_t bytes,
	                             uint32_t offset, unsigned char* buffer) {
    initrd_file_header_t header = file_headers[node->inode];
    uint32_t file_length = header.length; 
    if (offset > file_length) {
      return 0;
    }

    if (offset + bytes > file_length) {
      bytes = file_length - offset;
    }
        
    memcpy(buffer, (unsigned char*) (header.offset + offset), bytes);
    return bytes;
}

static dirent_t* initrd_readdir_fn(fs_node_t* node, uint32_t index) {
  memset(dirent.name, 0, MAX_FILENAME_SIZE);

  if (node == initrd_root && index == 0) {
    memcpy(dirent.name, "dev", 3);
    dirent.inode_num = 0;
    return &dirent;
  }

  if (index - 1 >= num_files) {
    return NULL;
  }

  fs_node_t* result_node = &nodes[index - 1];
  memcpy(dirent.name, result_node->name, sizeof(result_node->name));
  dirent.inode_num = result_node->inode;
  return &dirent;
}

static fs_node_t* initrd_finddir_fn(fs_node_t* node, char* file_name) {
  if (node == initrd_root && memcmp(file_name, "dev", 3) == 0) {
    return initrd_dev;
  }

  for (size_t i = 0; i < num_files; i++) {
    if (memcmp(file_name, nodes[i].name,
               sizeof(nodes[i].name)) == 0) {
      return &nodes[i];
    }
  }

  return NULL;
}

fs_node_t* load_initrd(uint32_t initrd_module_addr) {
  initrd_header_t* initrd_header = (initrd_header_t *) initrd_module_addr;
  num_files = initrd_header->num_files;

	initrd_file_header_t* file_headers =
		(initrd_file_header_t *) sizeof(initrd_header_t) + initrd_module_addr;

  // Initialize the root directory.
  initrd_root = kmalloc(sizeof(fs_node_t));
  memset(initrd_root, 0, sizeof(fs_node_t));
  memcpy(initrd_root->name, "initrd", 6);
  initrd_root->flags = FS_DIRECTORY;
  initrd_root->readdir_fn = &initrd_readdir_fn;
  initrd_root->finddir_fn = &initrd_finddir_fn;

  // Initialize the /dev directory.
  initrd_dev = kmalloc(sizeof(fs_node_t));
  memset(initrd_dev, 0, sizeof(fs_node_t));
  memcpy(initrd_dev->name, "dev", 3);
  initrd_dev->flags = FS_DIRECTORY;
  initrd_dev->readdir_fn = &initrd_readdir_fn;
  initrd_dev->finddir_fn = &initrd_finddir_fn;

  // Initialize the file nodes.
	nodes = kmalloc(sizeof(fs_node_t) * num_files);
  memset(nodes, 0, sizeof(fs_node_t) * num_files);

  for (size_t i = 0; i < num_files; i++) {
    // Edit the file's offset to be relative to relative to the start
    // of memory instead of the start of the ramdisk
    file_headers[i].offset += initrd_module_addr;

    // Create a new file node.
    memcpy(nodes[i].name, &file_headers[i].file_name,
           sizeof(file_headers[i].file_name));
    nodes[i].length = file_headers[i].length;
    nodes[i].inode = i;
    nodes[i].flags = FS_FILE;
    nodes[i].read_fn = &initrd_read_fn;
  }
  return initrd_root;
}

void delete_initrd() {
  kfree(initrd_root);
  kfree(initrd_dev);
  kfree(nodes);
}