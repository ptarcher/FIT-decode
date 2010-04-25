#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include <linux/limits.h>

#include "garmin/garmin.h"
#include "garmin/fit.h"
#include "garmin/fit_definitions.h"
#include "print_csv.h"
/*****************************************************************************
 *                               T Y P E S
*****************************************************************************/
#define SIZEOFARRAY(a) (sizeof(a)/sizeof(a[0]))

/*****************************************************************************
 *                        G L O B A L    V A R I A B L E S
*****************************************************************************/
#define PROGRAM_NAME         "fitdecode"
#define PROGRAM_AUTHOR       "Paul Archer"
#define PROGRAM_AUTHOR_EMAIL "ptarcher@gmail.com"
#define PROGRAM_VERSION      "0.1"

int nbytes;
bool flag_verbose;

FILE *fp;

csv_display_t display;
/*****************************************************************************/

void printUsage(void) {
    fprintf(stdout, "Usage: %s [options] filename\n"
                    "       -l --laps\n"
                    "       -s --session\n"
                    "       -r --records\n"
                    "       -v --verbose\n"
                    "       -h --help\n"
                    "\n\n"
                    "%s version %s. \n"
                    "Author: %s\n"
                    "Report bugs to %s",
                    PROGRAM_NAME,
                    PROGRAM_NAME,
                    PROGRAM_VERSION,
                    PROGRAM_AUTHOR,
                    PROGRAM_AUTHOR_EMAIL);
    exit(1);
}

int parseArgs(int argc, char *argv[]) {
    int c;
    int i;

    fp = stdin;

    while (1) {
        int this_option_optind;
        int option_index;
        static struct option long_options[] = {
            {"laps",    no_argument, 0, 'l'},
            {"session", no_argument, 0, 's'},
            {"records", no_argument, 0, 'r'},
            {0, 0, 0, 0},
        };

        this_option_optind = optind ? optind : 1;

        c = getopt_long(argc, argv, "lsrhv", long_options, &option_index);
        if (c == -1) {
            break;
        }

        switch(c) {
            case 0:
                printf("long option %s", long_options[option_index].name);
                if (optarg) {
                    printf(" with arg %s", optarg);
                }
                printf("\n");
                break;
            case 'l':
                if (display) {
                    fprintf(stderr, "Cannot have two modes set\n");
                    exit(1);
                }
                display = DISPLAY_LAPS;
                break;
            case 's':
                if (display) {
                    fprintf(stderr, "Cannot have two modes set\n");
                    exit(1);
                }
                display = DISPLAY_SESSIONS;
                break;
            case 'r':
                if (display) {
                    fprintf(stderr, "Cannot have two modes set\n");
                    exit(1);
                }
                display = DISPLAY_RECORDS;
                break;
            case 'h':
                printUsage();
                break;
            case 'v':
                flag_verbose = true;
                break;
            default:
                printUsage();
                break;
        }
    }


    for (i = optind; i < argc; i++) {
        if (fp != stdin) {
            fprintf(stderr, "more then one filename given %s\n", argv[i]);
            exit(1);
        }

        /* Open the input file */
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror(argv[i]);
            return 0;
        }


    }

    return 0;
}

int main(int argc, char *argv[])
{
    init_definitions();

    parseArgs(argc, argv);

    fit_process_file();

    return 0;
}
