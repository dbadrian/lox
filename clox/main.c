#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise.h"

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

void completion(const char *buf, linenoiseCompletions *lc)
{
    if (buf[0] == 'h')
    {
        linenoiseAddCompletion(lc, "hello");
        linenoiseAddCompletion(lc, "hello there");
    }
}

char *hints(const char *buf, int *color, int *bold)
{
    if (!strcasecmp(buf, "hello"))
    {
        *color = 35;
        *bold = 0;
        return " World";
    }
    return NULL;
}

static void repl()
{
    char line[1024];
    for (;;)
    {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char *readFile(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize)
    {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char *path)
{
    char *source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(70);
}

int main(int argc, const char *argv[])
{
    initVM();

    if (argc == 1)
    {
        repl(argv[0]);
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }

    freeVM();
    return 0;
}