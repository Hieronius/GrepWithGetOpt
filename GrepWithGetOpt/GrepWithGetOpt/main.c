#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <regex.h>

// 39:17
// https://www.youtube.com/watch?v=TBVEkpIMpYQ

// int getopt(int argc, char **argv, char *optsting);

// int opt;
// while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
// switch (opt) {

typedef struct arguments {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
    char *pattern;
    // int lenPattern;
    // int mem_pattern;
} arguments;

void outputLine(char *line, int n);
void processFile(arguments arg, char *path, regex_t * reg);
void processLine(char path);
void printMatchByFlagO(arguments arg, regex_t *reg, char *line, char *path, int lineCount);
void addRegularFromFile(arguments *arg, char *fulepath);
void outputFromAllFiles(arguments arg, int argc, char **argv);
arguments argumentParser(int argc, char **argv);

char *optarg; // global variable

int main(int argc, const char **argv) {
    arguments arg = argumentParser(argc, argv);
    outputFromAllFiles(arg, argc, argv);
    printf("Hello, World!\n");
    return 0;
}

void outputLine(char *line, int n) {
    for (int i = 0; i < n; i++) {
        putchar(line[i]);
    }
}

arguments argumentParser(int argc, char **argv) {
    arguments arg = {0};
    int opt;
    
    while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
        
        // Check the result of the func getop_long and analize it
        switch (opt) {
            case 'n':
                arg.n = 1;
                break;
            case 's':
                arg.s = 1;
                break;
            case 'e':
                arg.e = 1;
                arg.pattern = optarg;
                break;
            case 'l':
                arg.l = 1;
                arg.c = 1;
                break;
            case 'i':
                arg.i = 1;
                break;
            case 'h':
                arg.h = 1;
                break;
            case 'o':
                arg.o = 1;
                break;
            case 'f':
                arg.f = 1;
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
    if (arg.pattern == NULL) {
        arg.pattern = argv[optind];
        optind++;
    }
    return arg;
}

void processFile(arguments arg, char *path, regex_t * reg) {
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
    
    while (read != -1) {
        int result = regexec(reg, line, 0, NULL, 0); // return 0 if found match
        if (result == 0) {
            outputLine(line, read);
        }
        read = getline(&line, &memlen, file);
    }
    free(line);
    fclose(file);
}

void processLine(char path) {
    
}

void outputFromAllFiles(arguments arg, int argc, char **argv) {
    regex_t regular;
    int error = regcomp(&regular, arg.pattern, 0);
    if (error) {
        perror("Error");
    }
    for (int i = optind; i < argc; i++) {
        processFile(arg, argv[i], &regular);
    }
}

void addRegularFromFile(arguments *arg, char *path) {
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
        read = getline(&line, &memlen, file);
    }
    free(line);
}

// Applying flag "-o"
//void printMatchByFlagO(arguments arg, regex_t *reg, char *line, char *path, int lineCount) {
//
//    regmatch_t = match;
//    int offset = 0;
//
//    // infinite cycle until all possible matches will meet it's requirements
//    while (1) {
//
//        // find matches in string. Each time change position for offset
//        int result = regexec(reg, line +offset, 1, &match, 0);
//
//        // if there is no matches let's break the cycle
//        if (result != 0) {
//            break;
//        }
//
//        // print line or file number if there is some flags
//        printFlags(arg, path, lineCount);
//
//        // print the result of the match
//        for (int i = match.rm_so; i < match.rm_eo) {
//            putchar(line[offset +i]);
//        }
//        putchar('\n');
//
//        // move our offset so we can continue from other place
//        offset += match.rm_eo;
//    }
//}
