#include <arch/i386/fs.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

inline static bool is_directory_node(fs_node_t* node) {
  return (node->flags & 0x7) == FS_DIRECTORY;
}

fs_node_t* fs_root = 0; // The root of the filesystem.

uint32_t read_fs(fs_node_t* node, uint32_t bytes,
                 uint32_t offset, uint8_t* buffer) {
  if (node->read_fn != NULL) {
    return node->read_fn(node, offset, bytes, buffer);
  }
    
  return 0;
}

uint32_t write_fs(fs_node_t* node, uint32_t bytes,
                  uint32_t offset, uint8_t* buffer) {
  if (node->write_fn != NULL) {
    return node->write_fn(node, offset, bytes, buffer);
  }
    
  return 0;
}

void open_fs(fs_node_t* node) {
  if (node->open_fn != NULL) {
    return node->open_fn(node);
  }
}

void close_fs(fs_node_t* node) {
  if (node->close_fn != NULL) {
    return node->close_fn(node);
  }
}

dirent_t* readdir_fs(fs_node_t* node, uint32_t index) {
  if (is_directory_node(node) && node->readdir_fn != NULL) {
    return node->readdir_fn(node, index);
  }
  
  return 0;
}

fs_node_t* finddir_fs(fs_node_t* node, char* name) {
  if (is_directory_node(node) && node->finddir_fn != NULL) {
    return node->finddir_fn(node, name);
  }

  return 0;
}