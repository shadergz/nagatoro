#include <stdio.h>
#include <stdbool.h>

#include <getopt.h>
#include <stdint.h>
#include <ctype.h>

#include <assert.h>

static char *pattern = "%%";

FILE *outFile = NULL;

struct pattern
{
    char value;
    bool isPattern;
};

void produce(struct pattern *patternList, uintptr_t patternCount)
{
    char strbuffer[30] = {0};
    char *bufferptr = strbuffer;

    for (uintptr_t index = 0; index < patternCount; index++)
        *bufferptr++ = patternList[index].value;
    
    fprintf(outFile, "%s\n", strbuffer);
}

bool inctoend(struct pattern *patternList, uintptr_t patternCount)
{    
    bool inc = false;

    uintptr_t index = patternCount;

    uintptr_t lastEndIndex = 0;

    for (; index > 0 && inc == false; index--)
    {
        struct pattern *patt = &patternList[index - 1];
        if (patt->isPattern != true)
            continue;
        
        char value = patt->value;

        if (isdigit(value) != 0)
        {
            if (value == '9') { lastEndIndex = index - 1; continue; }
            value++;
        }
        else
        {
            if (value == 'Z') value = '`';
            if (value == 'z') { lastEndIndex = index - 1; continue; }
            
            value++;
        }

        if (lastEndIndex != 0)
        {
            struct pattern *last = &patternList[lastEndIndex];

            if (last->isPattern == true)
            {
                if (last->value == '9')
                    last->value = '0';
                else if (last->value == 'z')
                    last->value = 'A';
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
    outFile  = stdout;

    int c = 0;

    while ((c = getopt(argc, argv, "p:o:")) != -1)
    switch (c)
    {
    case 'p': pattern = optarg; break;
    case 'o': outFile = fopen(optarg, "w");
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

    while (inctoend(patternArray, patternCount)) 
        produce(patternArray, patternCount);

    if (outFile != stdout)
        fclose(outFile);

}
