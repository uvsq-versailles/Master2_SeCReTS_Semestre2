#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef int (*FONCTION)(unsigned char* userKey, int bits, void* blah);

int AES_set_encrypt_key(unsigned char* userKey, int bits, void* blah) {
    printf("%d bits", bits);
    for(int i=0;i<bits/8;i++){
        printf("%02x", userKey[i]);
    }
    printf("\n");
    FONCTION laVraieFonction = dlsym(RTLD_NEXT, "AES_set_encrypt_key");
    return laVraieFonction(userKey, bits, blah);
}
