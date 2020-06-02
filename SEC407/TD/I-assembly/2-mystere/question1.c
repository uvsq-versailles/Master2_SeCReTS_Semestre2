#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void mystere_product(uint8_t* r, const uint8_t* x, const uint8_t* y);

int main(){
    uint8_t x[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t y[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    uint8_t r[16];
    mystere_product(r,x,y);
    int i;
    for(i=0;i<16;i++){
        printf("%02x", r[i]);
    }
    printf("\n");
    return 0;
}
