#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#define SIZE 1024

typedef struct arguments {
    int e; // ✅
    int i; // ✅
    int v; // ✅
    int c; // ✅
    int l; // ✅
    int n; // ✅
    int h; // ✅
    int s; // ✅
    int f; // ✅/❌ working but my make testF won't complete
    int o; // ✅
    char pattern[SIZE];
    int patternLength;
} arguments;

void outputLine(char *line, int n);
void processFile(arguments arguments, char *path, regex_t * regularExpression);
void processLine(char path);
void printMatchByFlagO(arguments arguments, regex_t *regularExpression, char *line, char *path, int lineCount);
void addRegularExpressionFromFile(arguments *arguments, char *filePath);
void outputFromAllFiles(arguments arguments, int argc, char **argv);
void addPattern(arguments *arguments, char *pattern);
void printMatchWithO(regex_t *regularExpression, char *line);
arguments argumentParser(int argc, char **argv);

int main(int argc, char **argv) {
    arguments arguments = argumentParser(argc, argv);
    outputFromAllFiles(arguments, argc, argv);
    return 0;
}

void outputLine(char *line, int n) {
    for (int i = 0; i < n; i++) {
        putchar(line[i]);
    }
    if (line[n - 1] != '\n') {
        putchar('\n');
    }
}

arguments argumentParser(int argc, char **argv) {
    arguments arguments = {0};
    int option;
    
    while ((option = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
        
        // Check the result of the func getop_long and analize it
        switch (option) {
            case 'n':
                arguments.n = 1;
                break;
            case 's':
                arguments.s = 1;
                break;
            case 'e':
                arguments.e = 1;
                addPattern(&arguments, optarg);
                break;
            case 'l':
                arguments.l = 1;
                arguments.c = 1;
                break;
            case 'i':
                arguments.i = REG_ICASE;
                break;
            case 'h':
                arguments.h = 1;
                break;
            case 'o':
                arguments.o = 1;
                break;
            case 'f':
                arguments.f = 1;
                addRegularExpressionFromFile(&arguments, optarg);
                break;
            case 'v':
                arguments.v = 1;
                break;
            case 'c':
                arguments.c = 1;
                break;
            default: // if there is no any flag print and error
                perror("Error");
                exit(1);
                break;
        }
    }
    if (arguments.patternLength == 0) {
        addPattern(&arguments, argv[optind]);
        optind++;
    }
    if (argc - optind == 1) {
        arguments.h = 1;
    }
    return arguments;
}

// MARK: Problem can be here
void processFile(arguments arguments, char *path, regex_t *regularExpression) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        if (!arguments.s) {
            perror(path);
            exit(1);
        }
        exit(-1);
    }
    char *line = NULL;
    size_t memlen = 0; // special data type to measure memory size for strings
    int read = 0;
    int lineCount = 1;
    int counter = 0;
    read = getline(&line, &memlen, file);
    
    while (read != -1) {
        int result = regexec(regularExpression, line, 0, NULL, 0); // return 0 if found match
        
        if ((result == 0 && !arguments.v) || (arguments.v && result != 0)) {
            
            if (!arguments.c && !arguments.l) {
                
                if (!arguments.h) {
                    printf("%s:", path);
                }
                if (arguments.n) {
                    printf("%d:", lineCount);
                }
                if (arguments.o) {
                    printMatchWithO(regularExpression, line);
                } else {
                    outputLine(line, read);
                }
            }
            counter++;
        }
        read = getline(&line, &memlen, file);
        lineCount++;
    }
    free(line);
    if (arguments.c && !arguments.l) {
        if (!arguments.h) {
            printf("%s:", path);
        }
        printf("%d\n", counter);
    }
    if (arguments.l && counter > 0) {
        printf("%s\n", path);
    }
    fclose(file);
}

void outputFromAllFiles(arguments arguments, int argc, char **argv) {
    regex_t regularExpression;
    // printf("%s", arguments.pattern);
    // if there is a "-i" flag - apply it, otherwise our struct anyway filled with zeroes
    int error = regcomp(&regularExpression, arguments.pattern, REG_EXTENDED | arguments.i); //bitwise operator needed
    if (error) {
        perror("Error");
    }
    for (int i = optind; i < argc; i++) {
         processFile(arguments, argv[i], &regularExpression);
    }
    regfree(&regularExpression);
}

void addRegularExpressionFromFile(arguments *arguments, char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        if (!arguments->s) {
            perror(path);
            exit(1);
        }
        exit(-1);
    }
    char *line = NULL;
    size_t memoryLenght = 0;
    int read = getline(&line, &memoryLenght, file);
    
    while (read != -1) {
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        addPattern(arguments, line);
        read = getline(&line, &memoryLenght, file);
    }
    free(line);
    fclose(file);
}

void addPattern(arguments *arguments, char *pattern) {
    if (arguments->patternLength != 0) {
        strcat(arguments->pattern + arguments->patternLength, "|");
        arguments->patternLength++;
    }
      arguments->patternLength += sprintf(arguments->pattern + arguments->patternLength, "(%s)", pattern);
}

void printMatchWithO(regex_t *regularExpression, char *line) {
    regmatch_t match;
    int offset = 0;
    while (1) {
        int result = regexec(regularExpression, line + offset, 1, &match, 0);
        
        if (result != 0) {
            break;
        }
        
        for (int i = match.rm_so; i < match.rm_eo; i++) {
            putchar(line[i]);
        }
        putchar('\n');
        offset += match.rm_eo;
    }
}
