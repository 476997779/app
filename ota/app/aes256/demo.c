#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aes256.h"
#define DUMP(s, i, buf, sz) {printf(s); \        
               for(i = 0; i < sz; i++)  \
                 printf("%x ", buf[i]); \
               printf("\n");}          
int main (int argc, char *argv[])
{
  aes256_context ctx; 
  uint8_t key[32] = {0};
  uint8_t buf[32] = {0};
  uint8_t i;
  for (i = 0; i < sizeof(buf);i++) buf[i] = i * 16 + i;
  for (i = 0; i < sizeof(key);i++) key[i] = i;
  //strcpy(key,"hello");
  //strcpy(buf,"world");
  
  DUMP("txt: ", i, buf, sizeof(buf));
  DUMP("key: ", i, key, sizeof(key));
  printf("---\n");
  aes256_init(&ctx, key);
  aes256_encrypt_ecb(&ctx, buf);
  DUMP("enc: ", i, buf, sizeof(buf));
 // printf("en:%s\n",buf);

  aes256_init(&ctx, key);
  aes256_decrypt_ecb(&ctx, buf);
  //printf("de:%s\n",buf);
  DUMP("dec: ", i, buf, sizeof(buf));
  aes256_done(&ctx);
  return 0;
}
