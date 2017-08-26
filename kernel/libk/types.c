// string* new_string(char* data, size_t size);
// void delete_string(string* str);

// uint32_t string_hash(string* str) {
//   unsigned long hash = 5381;
//   char* data = str.data;
//   int c;

//   while (c = *data++) {
//     hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//   }   

//   return hash;
// }