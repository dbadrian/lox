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

static void repl(int argc, const char *argv[])
{
    char *line;
    const char *prgname = argv[0];

    // /* Parse options, with --multiline we enable multi line editing. */
    // while (argc > 1)
    // {
    //     argc--;
    //     argv++;
    //     if (!strcmp(*argv, "--multiline"))
    //     { 
    //         linenoiseSetMultiLine(1);
    //         printf("Multi-line mode enabled.\n");
    //     }
    //     else if (!strcmp(*argv, "--keycodes"))
    //     {
    //         linenoisePrintKeyCodes();
    //         exit(0);
    //     }
    //     else
    //     {
    //         fprintf(stderr, "Usage: %s [--multiline] [--keycodes]\n", prgname);
    //         exit(1);
    //     }
    // }

    /* Set the completion callback. This will be called every time the
     * user uses the <tab> key. */
    linenoiseSetCompletionCallback(completion);
    linenoiseSetHintsCallback(hints);

    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

    /* Now this is the main loop of the typical linenoise-based application.
     * The call to linenoise() will block as long as the user types something
     * and presses enter.
     *
     * The typed string is returned as a malloc() allocated string by
     * linenoise, so the user needs to free() it. */

    while ((line = linenoise("lox> ")) != NULL)
    {
        /* Do something with the string. */
        if (line[0] != '\0' && line[0] != '/')
        {
            // printf("echo: '%s'\n", line);
            interpret(line);
            linenoiseHistoryAdd(line);           /* Add to the history. */
            linenoiseHistorySave("history.txt"); /* Save the history on disk. */
        }
        else if (!strncmp(line, "/historylen", 11))
        {
            /* The "/historylen" command will change the history len. */
            int len = atoi(line + 11);
            linenoiseHistorySetMaxLen(len);
        }
        else if (!strncmp(line, "/mask", 5))
        {
            linenoiseMaskModeEnable();
        }
        else if (!strncmp(line, "/unmask", 7))
        {
            linenoiseMaskModeDisable();
        }
        else if (line[0] == '/')
        {
            printf("Unreconized command: %s\n", line);
        }
        free(line);
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
        repl(argc, argv);
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