#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#include "headers/instructions.h"
#include "headers/stack.h"

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
    entry_ptr.type = OBJECT_C_POINTER;
    entry_ptr.ptr = ip;
    stack.data[0] = entry_ptr;

    for(size_t i = 0; i < argc - 2; i++){
        object_t o;
        o.type = OBJECT_NUMBER;
        o.signage = 0;
        o.size = sizeof(uint8_t);

        char* end;
        o.value = strtol(argv[i + 2], &end, 10);
        if(argv[i + 3] != end)
            push_stack(&stack, o);
        else{
            for(size_t j = 0; j < strlen(argv[i + 2]); j++){
                object_t obj;
                obj.type = OBJECT_NUMBER;
                obj.signage = 0;
                obj.size = sizeof(uint8_t);
                obj.value = argv[i + 2][j];
                push_stack(&stack, obj);
            }
        }

    }

    while(*ip != 'h'){
        ip = opt[*ip](ip, &stack);
    }

    free(opt);
    del_stack(&stack);
    return 0;
}