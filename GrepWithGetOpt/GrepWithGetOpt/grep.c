#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#define SIZE 1024

// 1:49
//

// int getopt(int argc, char **argv, char *optsting);

// int opt;
// while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
// switch (opt) {

typedef struct arguments {
    int e; // ✅/❌ segmentation fault with two "-e"
    int i; // ✅/❌ segmentation fault
    int v; // ✅/❌ segmentation fault
    int c; // ✅/❌ segmentation fault
    int l; // ✅/❌ segmentation fault
    int n; // ✅/❌ segmentation fault
    int h; // ✅/❌ segmentation fault
    int s; // ✅/❌ segmentation fault
    int f; // ✅/❌ bad address
    int o; // ✅/❌ segmentation fault
    char pattern[SIZE];
    int lenPattern;
    // int mem_pattern;
} arguments;

void outputLine(char *line, int n);
void processFile(arguments arg, char *path, regex_t * reg);
void processLine(char path);
void printMatchByFlagO(arguments arg, regex_t *reg, char *line, char *path, int lineCount);
void addRegularExpressionFromFile(arguments *arg, char *fulepath);
void outputFromAllFiles(arguments arg, int argc, char **argv);
void addPattern(arguments *arg, char *pattern);
void printMatch(regex_t *regularExpression, char *line);
// void processLine(char *line);
arguments argumentParser(int argc, char **argv);

char *optarg; // global variable

int main(int argc, char **argv) {
    arguments arg = argumentParser(argc, argv);
    outputFromAllFiles(arg, argc, argv);
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
    arguments arg = {0};
    int option;
    
    while ((option = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
        
        // Check the result of the func getop_long and analize it
        switch (option) {
            case 'n':
                arg.n = 1;
                break;
            case 's':
                arg.s = 1;
                break;
            case 'e':
                arg.e = 1;
                addPattern(&arg, optarg);
                break;
            case 'l':
                arg.l = 1;
                arg.c = 1;
                break;
            case 'i':
                arg.i = REG_ICASE;
                break;
            case 'h':
                arg.h = 1;
                break;
            case 'o':
                arg.o = 1;
                break;
            case 'f':
                arg.f = 1;
                addRegularExpressionFromFile(&arg, optarg);
                break;
            case 'v':
                arg.v = 1;
                break;
            case 'c':
                arg.c = 1;
                break;
            case '?': // if func will return ? print an error in specific place
                perror("ERROR"); // the same as printf but prints an error to the different stream
                exit(1); // stop executing the program
                break;
            default: // if there is no any flag print and error
                perror("Error");
                exit(1);
                break;
        }
    }
    if (arg.lenPattern == 0) {
        addPattern(&arg, argv[optind]);
        optind++;
    }
    if (argc - optind == 1) {
        arg.h = 1;
    }
    return arg;
}

void processFile(arguments arg, char *path, regex_t * regularExpression) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        if (!arg.s) {
            perror(path);
            exit(-1);
        }
    }
    char *line = NULL;
    size_t memlen = 0; // special data type to measure memory size for strings
    int read = 0;
    int lineCount = 0;
    int c = 0;
    while (read != -1) {
        int result = regexec(regularExpression, line, 0, NULL, 0); // return 0 if found match
        if (arg.n) {
            printf("%d:", lineCount);
        }
        if ((result == 0 && !arg.v) || (arg.v && result != 0)) {
            if (!arg.c && !arg.l) {
                if (!arg.h) {
                    printf("%s:", path);
                }
                if (arg.n) {
                    printf("%d:", lineCount);
                }
                if (arg.o) {
                    printMatch(regularExpression, line);
                } else {
                    outputLine(line, read);
                }
            }
            c++;
        }
        read = getline(&line, &memlen, file);
        lineCount++;
    }
    free(line);
    if (arg.c && !arg.l) {
        if (!arg.h) {
            printf("%s:", path);
        }
        printf("%d\n", c);
    }
    if (arg.l && c > 0) {
        printf("%s\n", path);
    }
    fclose(file);
}

void outputFromAllFiles(arguments arg, int argc, char **argv) {
    regex_t regularExpression;
    printf("%s", arg.pattern);
    // if there is a "-i" flag - apply it, otherwise our struct anyway filled with zeroes
    int error = regcomp(&regularExpression, arg.pattern, REG_EXTENDED | arg.i); //bitwise operator needed
    if (error) {
        perror("Error");
    }
    for (int i = optind; i < argc; i++) {
        processFile(arg, argv[i], &regularExpression);
    }
}

void addRegularExpressionFromFile(arguments *arg, char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        if (!arg->s) {
            perror(path);
            exit(-1);
        }
    }
    char *line = NULL;
    size_t memlen = 0;
    int read = getline(&line, &memlen, file);
    
    while (read != -1) {
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        addPattern(arg, line);
        read = getline(&line, &memlen, file);
    }
    free(line);
    fclose(file);
}

void addPattern(arguments *arg, char *pattern) {
 //   int n = strlen(pattern);
//    if (pattern[n - 1]) == '\n') {
//        pattern[n - 1]);
//    }
    if (arg->lenPattern != 0) {
        strcat(arg->pattern + arg->lenPattern, "|");
        arg->lenPattern++;
    }
    arg->lenPattern += sprintf(arg->pattern + arg->lenPattern, "(%s)", pattern);
}

void printMatch(regex_t *regularExpression, char *line) {
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
