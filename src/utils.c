#include <stdlib.h>
#include <stdio.h>

char* substring(char* string, int position, int length){
    char* ptr = malloc(length+1);
    if (ptr == NULL) {
        printf("out of memory");
        exit(EXIT_FAILURE);
    }

    int c;
    for (c=0; c < length; c+=1){
        *(ptr+c) = *(string+position-1);
        string += sizeof(char);
    }
    *(ptr+c) = '\0';

    return ptr;
}
