#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>

// AES encryption in CBC-mode of operation.
// Key is 128 bits, or 16 bytes

#define AES128 1
#define AES_BLOCKLEN 16    //Block length in bytes AES is 128b block only
#define AES_KEYLEN 16      // Key length in bytes
#define AES_keyExpSize 176 // 11 rounds * 16 bytes per key is 176 bytes

struct AES_ctx {
  uint8_t RoundKey[AES_keyExpSize]; // 11 Rounds - unique key per round
  uint8_t Iv[AES_BLOCKLEN];         // Initialization vector
};

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv);

// buffer size MUST be mutile of AES_BLOCKLEN;
// Suggest https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
// NOTES: you need to set IV in ctx via AES_init_ctx_iv() or AES_ctx_set_iv()
//        no IV should ever be reused with the same key 
void AES_CBC_encrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);

#endif //_AES_H_
