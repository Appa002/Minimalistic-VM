#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "instructions.h"
#include "stack.h"

void usage(){
    printf("Usage vm [FILE]");
}

int load_file(const char *filename, unsigned char **result) {
    size_t size = 0;
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        *result = NULL;
        return -1;
    }
    fseek(f, 0, SEEK_END);
    size = (unsigned int)ftell(f);
    fseek(f, 0, SEEK_SET);
    *result = (unsigned char *)malloc(size+1);
    if (size != fread(*result, sizeof(char), size, f))
    {
        free(*result);
        return -2; // -2 means file reading fail
    }
    fclose(f);
    (*result)[size] = 0;
    return 0;
}

int main(int argc, char** argv) {
    if(argc < 2){
        usage();
        return 1;
    }

    stack_t stack = new_stack(1024);
    uint8_t* ip;

    if(load_file(argv[1], &ip) != 0)
        return -1;

    instruction* opt = malloc(sizeof(instruction*) * 256);
    register_instructions(opt);

    object_t entry_ptr;
    entry_ptr.type = OBJECT_POINTER;
    entry_ptr.ptr = ip;
    stack.data[0] = entry_ptr;

    while(*ip != 'h'){
        ip = opt[*ip](ip, &stack);
    }

    free(opt);
    del_stack(&stack);
    return 0;
}