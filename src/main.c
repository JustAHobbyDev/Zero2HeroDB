#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database filename>\n", argv[0]);
    printf("\t -n\tcreate new database file\n");
    printf("\t -f\t(required) path to database file\n");
}

int main(int argc, char *argv[]) {
    int c;
    char *filepath = NULL;
    bool newfile = false;
    int dbfd = -1;
    struct dbheader_t *dbhdr;

    while ((c = getopt(argc, argv, "nf:")) != -1) {
        switch (c) {
        case 'n':
            newfile = true;
            break;
        case 'f':
            filepath = optarg;
            break;
        case '?':
            printf("Unknown option -%c\n", c);
            break;
        default:
            return -1;
        }
    }

    if (filepath == NULL) {
        printf("Filepath is a required argumen\n");
        print_usage(argv);

        return 0;
    }

    if (newfile) {
        int dbfd = create_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to create database file\n");
            return -1;
        }
    }

    if (create_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
        printf("Failed to create database header\n");
    }

    dbfd = open_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
        printf("Unable to open database file\n"); 
        return -1;
    }

    printf("Newfile: %d\n", newfile);
    printf("Filepath %s\n", filepath);

    return 0;
}
