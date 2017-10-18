// #include "stellar_sdk.h"
#include <stdlib.h>
#include <mathisart.h>
#include "sds/sds.h"
#include "sds/sdsalloc.h"


// ------------------------------------------------------------------------------------------------
// Helper functions!

// void m_sep(){
//   puts("----------------------------------------------------------------------");
// }

// void m_puts(){
//   puts(" ");
// }

void show_buffer_char(uint8* buffer, uint64 nbytes){
  for(uint i=0; i<nbytes; ++i)
    putchar(buffer[i]);
  m_puts();
}


// ------------------------------------------------------------------------------------------------
int main(int argc, char** argv){
  m_sep();
  uint8 TEMPLATE0[] = "Hello! %s %d";
  uint8 OUTPUT_BUFFER[1<<10];
  sprintf(OUTPUT_BUFFER, TEMPLATE0, "lala", 123);
  puts(OUTPUT_BUFFER);
  // show_buffer_char(OUTPUT_BUFFER, strlen(OUTPUT_BUFFER));

  // ----------------------------------------------------------------------
  m_sep();
  sds str0 = sdsnew("Hiya!");
  puts(str0);
  sdsfree(str0);

  // ----------------------------------------------------------------------
  m_exit_success();
}
