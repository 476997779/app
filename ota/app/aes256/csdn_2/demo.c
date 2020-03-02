
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/aes.h>
 
 
// out的内存大小需要注意 后边有 out += AES_BLOCK_SIZE 
// 所以out 内存最小不能小于 AES_BLOCK_SIZE 
int aes_encrypt(char* in, char* key, char* out)
{
    if (!in || !key || !out)
    {
        return 0;
    }
 
    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }
 
    int len = strlen(in), en_len = 0;
 
    //输入输出字符串够长。而且是AES_BLOCK_SIZE的整数倍，须要严格限制
    while (en_len < len)
    {
        AES_encrypt((unsigned char*)in, (unsigned char*)out, &aes);
        in	+= AES_BLOCK_SIZE;
        out += AES_BLOCK_SIZE;
        en_len += AES_BLOCK_SIZE;
    }
 
    return 1;
}
 
int aes_decrypt(char* in, char* key, char* out)
{
    if (!in || !key || !out)
    {
        return 0;
    }
 
    AES_KEY aes;
    if (AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }
 
    int len = strlen(in), en_len = 0;
    while (en_len < len)
    {
        AES_decrypt((unsigned char*)in, (unsigned char*)out, &aes);
        in	+= AES_BLOCK_SIZE;
        out += AES_BLOCK_SIZE;
        en_len += AES_BLOCK_SIZE;
    }
 
    return 1;
}
 
int main(int argc, char**argv)
{
    char *data, *key = "1111222", encrypt_buffer[100] = {0}, decrypt_buffer[100] = {0};
    size_t len;
 
    if (argc != 2)
    {
        printf("请输入待加密的明文。\n");
        return -1;
    }
 
    data = argv[1];
    printf("原始数据：%s\n", data);
 
    len = strlen(data);
    printf("明文长度：%d\n", len);
 
    aes_encrypt(data, key, encrypt_buffer);
    printf("加密结果：%s\n", encrypt_buffer);
 
    aes_decrypt(encrypt_buffer, key, decrypt_buffer);
    printf("解密结果：%s\n", decrypt_buffer);
}
