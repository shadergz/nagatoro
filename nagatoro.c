#include <stdio.h>
#include <stdbool.h>

#include <getopt.h>
#include <stdint.h>
#include <ctype.h>

#include <string.h>

#include <assert.h>

static char *pattern = "%%";

FILE *outFile = NULL;

char *stopWord = NULL;
static bool stop = false;

struct pattern
{
    char value;
    bool isPattern;
    struct parent *parent;
};

void produce(struct pattern *patternArray, uintptr_t patternCount)
{
    char strbuffer[30] = {0};
    char *bufferptr = strbuffer;

    for (uintptr_t index = 0; index < patternCount; index++)
        *bufferptr++ = patternArray[index].value;

    if (stopWord)
        if(strncmp(strbuffer, stopWord, sizeof(strbuffer)) == 0)
            stop = true;
    
    fprintf(outFile, "%s\n", strbuffer);
}

bool inctoend(struct pattern *patternArray, const uintptr_t patternCount)
{
    bool inc = false;
    struct pattern *parent = NULL;

    uintptr_t index = patternCount;

    for (; index > 0 && inc == false; )
    {
        struct pattern *patt = patternArray + --index;
        if (patt->isPattern != true)
            continue;
        
        char value = patt->value;

        if (value >= '0' && value <= '9')
        {
            if (value == '9') { parent = patt; continue; }
            value++;
        }
        else
        {
            if (value == 'Z') value = '`';
            if (value == 'z') { parent = patt; continue; }
            
            value++;
        }

        if (parent != NULL)
        {
            for (; parent != (patternArray + patternCount); parent++)
            {
                if (parent->isPattern == true)
                {
                    if (parent->value == '9')
                        parent->value = '0';
                    else if (parent->value == 'z')
                        parent->value = 'A';
                }
            }
        }

        patt->value = value;
        inc = true;
    }
    return inc;
}

int main(int argc, char **argv)
{
    static struct pattern patternArray[300] = {0};
    uintptr_t patternCount = 0;
    static bool merge = false;

    outFile = stdout;

    int c = 0;

    while ((c = getopt(argc, argv, "p:o:S:m")) != -1)
    switch (c)
    {
    case 'p': pattern = optarg; break;
    case 'm': merge = true; break;
    case 'o':
        if (merge)
            outFile = fopen(optarg, "a"); 
        else 
            outFile = fopen(optarg, "w");
        break;
    case 'S': stopWord = optarg; break;
    }

    char *solverPattern = pattern;
    char value;

    while ((value = *solverPattern++) != '\0')
    {
        switch (value)
        {
        case '%':
            patternArray[patternCount].value = '0';
            patternArray[patternCount++].isPattern = true;
            break;
        case '$':
            patternArray[patternCount].value = 'A';
            patternArray[patternCount++].isPattern = true;
            break;
        default:
            patternArray[patternCount++].value = value;
        }

    }

    produce(patternArray, patternCount);

    for (; inctoend(patternArray, patternCount) && (stop == false); )
        produce(patternArray, patternCount);

    fflush(outFile);

    if (outFile != stdout)
        fclose(outFile);

}


