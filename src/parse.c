#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees,
                 char *addstring) {}

int read_employees(int fd, struct dbheader_t *dbhdr,
                   struct employee_t **employeesOut) {}

int output_file(int fd, struct dbheader_t *dbhdr,
                struct employee_t *employees) {}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }

    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL) {
        printf("Malloc failed to create dbheader_t *header");
        return STATUS_ERROR;
    }

    if (read(fd, header, sizeof(struct dbheader_t)) !=
        sizeof(struct dbheader_t)) {
        perror("read");
        free(header);
        return STATUS_ERROR;
    }

    header->version  = ntohs(header->version);
    header->count    = ntohs(header->count);
    header->magic    = ntohs(header->magic);
    header->filesize = ntohs(header->filesize);

    if (header->version != 1) {
        printf("Improper header version\n");
        free(header);
        return -1;
    }

    if (header->magic != HEADER_MAGIC) {
        printf("Improper magic version\n");
        free(header);
        return -1;
    }

    struct stat dbstat = {0};
    fstat(fd, &dbstat);
    if (header->filesize != dbstat.st_size) {
        printf("Corrupted database header\n");
        free(header);
        return -1;
    }
}

int create_db_header(int fd, struct dbheader_t **headerOut) {
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL) {
        printf("Malloc failed to create dbheader_t *header");
        return STATUS_ERROR;
    }
    header->version  = 0x1;
    header->count    = 0;
    header->magic    = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);

    *headerOut       = header;

    return STATUS_SUCCESS;
}
