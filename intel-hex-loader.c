/*
 *  Copyright 2024 James Burlingame
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      https://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "config.h"
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef EXIT_USAGE
#define EXIT_USAGE 2
#endif

typedef struct {
    bool        isVerbose;
    const char *outputFilename;
    const char *patchFilename;
    const char *programName;
    unsigned    programSize;
} loaderOptionsType;

typedef struct {
    uint8_t     memory[64*1024];
    uint16_t    memoryLimit;
} loaderMemoryType;

uint8_t   getNibble(unsigned lineNumber, int c, uint8_t *pNibble) {
    if ('0' <= c && c <= '9') {
        *pNibble = c - '0';
    } else if ('A' <= c && c <= 'F') {
        *pNibble = c - 'A' + 10;
    } else if ('a' <= c && c <= 'f') {
        *pNibble = c - 'a' + 10;
    } else {
        fprintf(stderr, "ERROR: Line: %u: Invalid hex character: %c\n", lineNumber, c);
        return false;
    }
    return true;
}

bool   getByte (FILE *fp, unsigned lineNumber, uint8_t *pChecksum, uint8_t *pByte) {
    uint8_t high, low;
    int c1 = fgetc(fp);
    int c2 = fgetc(fp);
    high = 0;
    low = 0;
    if ((c2 == EOF) || !getNibble(lineNumber, c1, &high) || !getNibble(lineNumber, c2, &low)) {
        return false;
    }
    uint8_t   byte = (high << 4) | low;

    *pChecksum += byte;
    *pByte = byte;
    return true;
}

bool skipToColon(FILE *fp, unsigned *pLineNumber) {
    int c;
    do {
        c = fgetc(fp);
        if (c == EOF) {
            return false;
        } else if (c == '\n') {
            (*pLineNumber) += 1;
        }
    } while (c != ':');
    return true;
}

#define LINE_LOADED 0
#define LINE_ERROR  1
#define LINE_EOF    2

int loadLine(FILE *fp, loaderOptionsType *pOptions, loaderMemoryType *pMemory, unsigned *pLineNumber) {
    if (!skipToColon(fp, pLineNumber)) {
        fflush(stdout);
        fprintf(stderr, "\nWarning: premature End-of-file.\n");
        return LINE_EOF;
    }
    uint8_t checksum = 0;
    uint8_t byteCount, addressHigh, addressLow, recordType, actualSum;
    uint16_t address;
    if (!getByte(fp, *pLineNumber, &checksum, &byteCount) ||
        !getByte(fp, *pLineNumber, &checksum, &addressHigh) ||
        !getByte(fp, *pLineNumber, &checksum, &addressLow) ||
        !getByte(fp, *pLineNumber, &checksum, &recordType)) {
        fflush(stdout);
        fprintf(stderr, "\nERROR: Unable to read start of record. line number: %u\n", *pLineNumber);
        return LINE_ERROR;
    }
    address = (((uint16_t)addressHigh) << 8) | addressLow;
    for (uint8_t n = 0; n < byteCount; n++) {
        uint16_t ea = address + n;
        if (!getByte(fp, *pLineNumber, &checksum, pMemory->memory + ea)) {
            fflush(stdout);
            fprintf(stderr,
                "\nERROR: Unable to read record data @0x%04x. line number: %u. byte offset: %u\n",
                    (unsigned)ea, *pLineNumber, (unsigned)n);
            return LINE_ERROR;
        }
    }
    if (!getByte(fp, *pLineNumber, &checksum, &actualSum)) {
        fprintf(stderr, "\nERROR: Unable to read record checksum. line number: %u\n", *pLineNumber);
        return LINE_ERROR;
    }
    if (checksum != 0) {
        fflush(stdout);
        fprintf(stderr, "\nERROR: checksum mismatch 0x%02x/0x%02x\n", (unsigned)actualSum, (unsigned)checksum);
        return LINE_ERROR;
    }
    if (recordType == 1) {
        // end of file record type, we are done
        return LINE_EOF;
    }
    if (recordType != 0) {
        fflush(stdout);
        fprintf(stderr, "\nERROR: unsupported record type 0x%02x\n", recordType);
        return LINE_ERROR;
    }
    if (0 == byteCount) {
        // a byte count of zero is also used to mark the end of the file
        return LINE_EOF;
    }
    if (0 == pOptions->programSize) {
        uint16_t maxAddress = address + byteCount;
        if (pMemory->memoryLimit < maxAddress) {
            pMemory->memoryLimit = maxAddress;
        }
    }
    return LINE_LOADED;
}

bool loadHexFile(const char *filename, loaderOptionsType *pOptions, loaderMemoryType *pMemory) {
    if (pOptions->isVerbose) {
        printf("loading HEX file     < '%s'\n", filename);
    }
    FILE *fpHex = fopen(filename, "r");
    if (NULL == fpHex) {
        return false;
    }
    unsigned lineNumber = 1;
    int loadResult;
    while (LINE_LOADED == (loadResult = loadLine(fpHex, pOptions, pMemory, &lineNumber))) {
        if (pOptions->isVerbose) {
            printf("X");
        }
    }
    fclose(fpHex);
    if (LINE_ERROR == loadResult) {
        return false;
    }
    if (pOptions->isVerbose) {
        printf("\nloaded.\n");
    }
    return true;
}

void usage(loaderOptionsType *pOptions, FILE *fp) {
    fprintf(fp, "Usage:\n");
    fprintf(fp, "    %s --output|-o output-com-file [--patch|-p original-com-file] [--size|-s pages] hex-file(s)\n\n", pOptions->programName);
    fprintf(fp, "This program is used to create a CP/M COM file from one or\n");
    fprintf(fp, "more Intel HEX format files.\n");
    fprintf(fp, "It should act like the CP/M program LOAD or HEXCOM or\n");
    fprintf(fp, "a DDT and SAVE combination.\n\n");
    fprintf(fp, "*  Either the --output or -o option must be defined.\n");
    fprintf(fp, "*  The --patch or -p option allows for patching an existing .COM file\n");
    fprintf(fp, "*  Size depends upon what is loaded. use --size or -s option to override.\n");
}

unsigned parseSize(const char *size) {
    char *last;
    errno = 0;
    unsigned long value = strtoul(size, &last, 0);
    if ((errno != 0) || (last == NULL) || (*last != '\0')) {
        fprintf(stderr, "Warning: unable to parse '%s' as a number, it is ignored.\n", size);
        return 0;
    }
    return (unsigned)value;
}

void setOption(int action, const char *arg, loaderOptionsType *pOptions) {
    switch (action) {
        case -1:
            break;
        case 'h':
            usage(pOptions, stdout);
            exit(EXIT_SUCCESS);
            /* NOTREACHED */
        case 'o':
            pOptions->outputFilename = strdup(optarg);
            break;
        case 'p':
            pOptions->patchFilename = strdup(optarg);
            break;
        case 's':
            pOptions->programSize = parseSize(optarg);
            break;
        case 'v':
            pOptions->isVerbose = true;
            break;
        case '?':
            usage(pOptions, stderr);
            exit(EXIT_USAGE);
            /* NOTREACHED */
        default:
            fprintf(stderr, "ERROR: setOption: unrecognized action=%d, arg=%s\n",
                action, (arg == NULL) ? "NULL" : arg);
            exit(EXIT_FAILURE);
            /* NOTREACHED */
    }
}

void initializeMemory(loaderOptionsType *pOptions, loaderMemoryType *pMemory) {
    if (0 == pOptions->programSize) {
        pMemory->memoryLimit = 0x0100;
    } else {
        pMemory->memoryLimit = (uint16_t)((pOptions->programSize << 8) | 0x00FF);
    }
    if (pOptions->isVerbose) {
        printf("initial memoryLimit  = 0x%04x\n", pMemory->memoryLimit);
        if (0 == pOptions->programSize) {
            printf("program size setting = %s\n", "none");
        } else {
            printf("program size setting = %u\n", pOptions->programSize);
        }
    }
    memset((void *)pMemory->memory, 0, 64 * 1024);
}

void dumpMemory(loaderOptionsType *pOptions, loaderMemoryType *pMemory) {
    if (0 == pOptions->programSize) {
        // do we have anything to save?
        if (0x0100 == pMemory->memoryLimit) {
            fprintf(stderr, "Warning: nothing to save. Use the --size or -s option to override.\n");
            return;
        }
        // round up to a whole page if we have anything
        if ((pMemory->memoryLimit & 0xFF) != 0) {
            pMemory->memoryLimit = (uint16_t)(pMemory->memoryLimit | 0x00FF);
        }
    }
    size_t memorySize = (pMemory->memoryLimit - 0x0100) + 1;
    if (pOptions->isVerbose) {
        printf("final memoryLimit    = 0x%04x\n", pMemory->memoryLimit);
        printf("file size in bytes   = %lu\n", memorySize);
        printf("dumping memory to    > '%s'\n", pOptions->outputFilename);
    }
    FILE *fpCom = fopen(pOptions->outputFilename, "wb");
    if (NULL == fpCom) {
        fprintf(stderr, "ERROR: Unable to create output file %s\n", pOptions->outputFilename);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }
    size_t nWritten = fwrite((const void *)(pMemory->memory + 0x0100), 1, memorySize, fpCom);
    if (nWritten != memorySize) {
        fprintf(stderr, "ERROR: Unable to save output file %s. %lu bytes written of %lu expected.\n", pOptions->outputFilename, nWritten, memorySize);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }
    fclose(fpCom);
}

void loadPatchFile(loaderOptionsType *pOptions, loaderMemoryType *pMemory) {
    if (NULL == pOptions->patchFilename) {
        return;
    }
    if (pOptions->isVerbose) {
        printf("loading COM file     < '%s'\n", pOptions->patchFilename);
    }
    FILE *fpPatch = fopen(pOptions->patchFilename, "rb");
    if (NULL == fpPatch) {
        fprintf(stderr, "ERROR: Unable to load patch file %s\n", pOptions->patchFilename);
        exit(EXIT_FAILURE);
        /* NOTREACHED*/
    }
    errno = 0;
    size_t nread = fread((void *)(pMemory->memory + 0x0100), 1, 0xFEFF, fpPatch);
    if (0 != errno) {
        fprintf(stderr, "ERROR: unable to read patch file %s, errno=%d\n", pOptions->patchFilename, errno);
        exit(EXIT_FAILURE);
        /* NOTREACHED*/
    }
    if (pOptions->isVerbose) {
        printf("bytes loaded         = %lu\n", nread);
    }
    if (0 == pOptions->programSize) {
        // the limit is the last byte loaded, not the next byte to load
        pMemory->memoryLimit = (uint16_t)(0x0100 + nread - 1);
    }
    fclose(fpPatch);
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        { "verbose", no_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h'},
        { "output", required_argument, NULL, 'o'},
        { "patch", required_argument, NULL, 'p'},
        { "size", required_argument, NULL, 's'},
        { NULL, 0, NULL, 0 }
    };
    int option_index = 0;
    int c;
    loaderOptionsType options;
    memset((void *)&options, 0, sizeof(options));
    options.programName = argv[0];

    loaderMemoryType memory;
    memset((void *)&memory, 0, sizeof(memory));

    do {
        c = getopt_long(argc, argv, "ho:p:s:v", long_options, &option_index);
        setOption(c, optarg, &options);
    } while (c > 0);

    if (NULL == options.outputFilename) {
        fprintf(stderr, "ERROR: --output or -o option is required to define the output file\n");
        exit(EXIT_FAILURE);
    }
    if (optind >= argc) {
        fprintf(stderr, "ERROR: One or more HEX input files are required.\n");
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }
    initializeMemory(&options, &memory);
    if (options.patchFilename != NULL) {
        loadPatchFile(&options, &memory);
    }
    for (int n = optind; n < argc; n++) {
        if (!loadHexFile(argv[n], &options, &memory)) {
            fprintf(stderr, "ERROR: Unable to load HEX input file '%s'\n", argv[n]);
            exit(EXIT_FAILURE);
            /* NOTREACHED */
        }
    }
    dumpMemory(&options, &memory);

    exit(EXIT_SUCCESS);
    /* NOTREACHED */
}
