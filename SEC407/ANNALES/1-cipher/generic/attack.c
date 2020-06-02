#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdint.h>

//typedef void (*FONCTION)(char* password, uint8_t salt, uint8_t* ciphertext_buf, int message_len, uint8_t decr);

void decrypt_message(char* password, uint8_t salt, uint8_t* ciphertext_buf, int message_len, uint8_t decr) {
    /*printf("PASSWORD : %s\n", password);
    printf("salt : %d\n", salt);
    int i;
    printf("cipher: ");
    for(i=0;i<message_len;i++){
		printf("%d ", ciphertext_buf[i]);
	}
    printf("\n");
    printf("decr: %d\n", decr);*/
    
    printf("coduehdegdyegydegyge");
    
    //FONCTION laVraieFonction = dlsym(RTLD_NEXT, "decrypt_message");
    exit(0);
}
