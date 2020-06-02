#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// values $rdi, n $rsi
double moyenne(double* values, int64_t n);

//double moyenneopti(double* values, int64_t n);

int main(){
    
    double values [10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    printf("RESULTAT 1 : %lf\n", moyenne(values, 10));
    // printf("RESULTAT 2 : %lf\n", moyenneopti(values, 10));
    return 0;
}
