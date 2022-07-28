void default_tests() {
    printf("ceasarEncrypt(gusty, 1): %s\n", caesar_encrypt("gusty", "1"));
    printf("ceasarEncrypt(gusty, 27): %s\n", caesar_encrypt("gusty", "27"));
    printf("ceasarEncrypt(gusty, 3): %s\n", caesar_encrypt("gusty", "3"));
    printf("ceasarEncrypt(gusty, 29): %s\n", caesar_encrypt("gusty", "29"));

    printf("ceasarEncrypt(gusty, 0): %s\n", caesar_encrypt("gusty", "0"));
    printf("ceasarEncrypt(gusty, 26): %s\n", caesar_encrypt("gusty", "26"));

    printf("ceasarDecrypt(ceasarEncrypt(gusty, 1)): %s\n", caesar_decrypt(caesar_encrypt("gusty", "1"), "1"));
    printf("ceasarDecrypt(ceasarEncrypt(gusty, 27)): %s\n", caesar_decrypt(caesar_encrypt("gusty", "27"), "27"));
    printf("ceasarDecrypt(ceasarEncrypt(gusty, 3)): %s\n", caesar_decrypt(caesar_encrypt("gusty", "3"), "3"));
    printf("ceasarDecrypt(ceasarEncrypt(gusty, 29)): %s\n", caesar_decrypt(caesar_encrypt("gusty", "29"), "29"));

    printf("ceasarEncrypt(gusty, 2): %s\n", caesar_encrypt("gusty", "2"));
    printf("augustusEncrypt(gusty, 1): %s\n", augustus_encrypt("gusty", "1"));
    printf("augustusDecript(augustusEncrypt(gusty, 1), 1): %s\n", augustus_decrypt(augustus_encrypt("gusty", "1"), "1"));

    printf("ceasarEncrypt(gusty, 11): %s\n", caesar_encrypt("gusty", "11"));
    printf("augustusEncrypt(gusty, 11): %s\n", augustus_encrypt("gusty", "11"));
    printf("augustusDecript(augustusEncrypt(gusty, 11), 11): %s\n", augustus_decrypt(augustus_encrypt("gusty", "11"), "11"));
    printf("XXaugustusDecript(augustusEncrypt(gusty, 11), 11): %s\n", decrypt_string(AUGUSTUS, encrypt_string(AUGUSTUS, "gusty", "11"), "11"));

    string *str = encrypt_string(CAESAR, "gusty !15:","2");
    printf("%s\n", str->plain);
    printf("%s\n", str->cipher);
    printf("str->print\n");
    str->print(str, PLAIN);
    str->print(str, CIPHER);
    char *plain = decrypt_string(CAESAR, str, "2");
    printf("%s\n", plain);

    str = encrypt_string(AUGUSTUS, "gusty", "1");
    printf("\n%s\n", str->plain);
    printf("%s\n", str->cipher);
    str->print(str, PLAIN);
    str->print(str, CIPHER);
    plain = decrypt_string(AUGUSTUS, str, "1");
    printf("%s\n", plain);

    char *test_str = "ABCDEFGHIJKLMNOPZac"; // test_str has 19 characters
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

    uint8_t in[16];
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    memcpy(in, test_str, 16);               // test_str points to a string literal. 
                                            // Must put into a rd/wr uint8_t buffer
    printf("aes_cbc call demo\n");
    struct AES_ctx ctx;                     // define AES_ctx variable
    AES_init_ctx_iv(&ctx, key, iv);         // initialize IV and Round keys
    AES_CBC_encrypt_buffer(&ctx, in, 16);   // encrypt the buffer
    for (int i = 0; i < 16; i++)            // print hex of encrypted buffer
        printf("%.2x " , in[i]);
    printf("  |  ");
    for (int i = 0; i < 16; i++)            // print char of encrypted buffer
        if (isprint(in[i]))
            printf("%c" , in[i]);
        else
            printf(" ");
    printf("\n");
    AES_init_ctx_iv(&ctx, key, iv);         // initialize IV and Round keys
    AES_CBC_decrypt_buffer(&ctx, in, 16);   // decrypt the buffer
    for (int i = 0; i < 16; i++)            // printt hex of decrypted buffer
        printf("%.2x " , in[i]);
    printf("  |  ");
    for (int i = 0; i < 16; i++)            // print char of encrypted buffer
        if (isprint(in[i]))
            printf("%c" , in[i]);
        else
            printf(" ");
    printf("\n");

    /*
    The next lines demonstrate how to call the string wrapper functions
    1. You just need a string to encrypt and a string key.
    2. The encrypt_string function returns a string *, which contains the encrypted string
       encrypt_string rounds the input string to a multiple of 16 and fills in 0 for the extra bytes.
    3. The decrypt_string decrypts the string * with its char *key.
       decrypt_string returns a char *.
    */
    
    char in_key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 0 };
    string *b = encrypt_string(AES, test_str, in_key); 
    printf("\nHex of plain test_str:\n");
    b->print(b, PLAIN);
    printf("\nHex of cipher test_str:\n");
    b->print(b, CIPHER);
    char *s = decrypt_string(AES, b, in_key);
    printf("Hex of decrypted test_str:\n");
    print_C_string(s);
    printf("Plaintext of decypted test_str: %s\n", s);

    printf("NEW TEST\n");
    char in_key2[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 0 };
    char cipher_text[] = { 0xcb,0x0d,0x58,0xf5,0x16,0xa7,0x4e,0x4a,0xfb,0xb0,0x9e,0xaa,0x92,0x38,0x91,0x4f,
                           0xe0,0x98,0x19,0x99,0x49,0x0d,0xc2,0xef,0xc9,0xaf,0x36,0x75,0xbf,0xe2,0x80,0xd9 };
    string *x = new_cipher(cipher_text, 32, 0);
    x->print(x, CIPHER);
    //s = x->decrypt(AES, x, in_key); 
    s = decrypt_string(AES, x, in_key2);
    //x->print(x, PLAIN);
    x->print(x, CIPHER);
    printf("s: %p\n", s);
    print_C_string(s);

    printf("NEW TEST 2\n");
    string *y = new_plain("Camp Giraffy", 1);
    y = y->encrypt(AES, "Camp Giraffy", in_key);
    y->print(y, PLAIN);
    y->print(y, CIPHER);
    s = y->decrypt(AES, y, in_key);
    print_C_string(s);

    b = encrypt_string(CAESAR, "Be sure to drink your Ovaltine!", "13"); 
    printf("\nHex of plain test_str:\n");
    b->print(b, PLAIN);
    printf("\nHex of cipher test_str:\n");
    b->print(b, CIPHER);
    s = decrypt_string(CAESAR, b, "13");
    printf("Hex of decrypted test_str:\n");
    print_C_string(s);
    printf("Plaintext of decypted test_str: %s\n", s);
}

