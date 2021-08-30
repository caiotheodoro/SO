#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    

    FILE* fp = fopen("oi-logo.png", "rb");

    fseek(fp, 0, SEEK_END);
    int pos = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    char* buffer = (char*)malloc(sizeof(char) * pos);
    printf("oi.txt - pos %d\n", pos);

    fread(buffer, sizeof(char), pos, fp);
    // memcpy(buffer, fp, pos);

    fclose(fp);
    //$$$$$$$$$$$$$$$$$$$$$

    FILE* file = fopen("xandaoehgay.png", "wb");
    
    fwrite(buffer, sizeof(char), pos, file);
    fclose(file);
}