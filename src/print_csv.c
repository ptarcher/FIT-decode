#include <stdio.h>
#include <time.h>


#include "garmin/garmin.h"
#include "garmin/fit.h"
#include "print_csv.h"

/*****************************************************************************/

#define SIZEOFARRAY(a) (sizeof(a)/sizeof(a[0]))

/*****************************************************************************/

void printCSVHeader(csv_display_t display) {
    int i;

#if 0
    switch(display) {
        case DISPLAY_LAPS:
            for (i = 0; i < SIZEOFARRAY(lap_data); i++) {
                printf("%s%s", lap_data[i].label, i + 1 < SIZEOFARRAY(lap_data) ? "," : "");
            }
            printf("\n");
            break;
        case DISPLAY_SESSIONS:
            for (i = 0; i < SIZEOFARRAY(session_data); i++) {
                printf("%s%s", session_data[i].label, i + 1 < SIZEOFARRAY(session_data) ? "," : "");
            }
            printf("\n");
            break;
        case DISPLAY_RECORDS;
            for (i = 0; i < SIZEOFARRAY(record_data); i++) {
                printf("%s%s", record_data[i].label, i + 1 < SIZEOFARRAY(record_data) ? "," : "");
            }
            printf("\n");
            break;
    }
#endif
}

void print_csv_record_data(void *data, struct fit_record_data *record, int nelms) {
    int i;
    uintptr_t ptr;

    for (i = 0; i < nelms; i++) {
        ptr = ((uintptr_t)data) + record[i].offset;

        switch(record[i].type) {
            case TYPE_STRING:
            {
                char *str = (char *)ptr;;
                printf("%s,", str);
                break;
            }
            case TYPE_UINT8:
            {
                uint8_t value = *(uint8_t *)ptr;;
                if (value != UINT8_MAX) {
                    printf("%.2f,", value * record[i].scale);
                } else {
                    printf("INVALID,");
                }
                break;
            }
            case TYPE_UINT16:
            {
                uint16_t value = *(uint16_t *)ptr;;
                if (value != UINT16_MAX) {
                    printf("%.2f,", value * record[i].scale);
                } else {
                    printf("INVALID,");
                }
                break;
            }
            case TYPE_UINT32:
            {
                uint32_t value = *(uint32_t *)ptr;;
                if (value != UINT32_MAX) {
                    printf("%.2f,", value * record[i].scale);
                } else {
                    printf("INVALID,");
                }
                break;
            }
            case TYPE_INT8:
            {
                int8_t value = *(int8_t *)ptr;;
                if (value != INT8_MAX) {
                    printf("%.2f,", value * record[i].scale);
                } else {
                    printf("INVALID,");
                }
                break;
            }
            case TYPE_INT16:
            {
                int16_t value = *(int16_t *)ptr;;
                if (value != INT16_MAX) {
                    printf("%.2f,", value * record[i].scale);
                } else {
                    printf("INVALID,");
                }
                break;
            }
            case TYPE_INT32:
            {
                int32_t value = *(int32_t *)ptr;;
                if (value != INT32_MAX) {
                    printf("%.2f,", value * record[i].scale);
                } else {
                    printf("INVALID,");
                }
                break;
            }
                
            case TYPE_POS:
            {
                int32_t value = *(int32_t *)ptr;;
                if (value != INT32_MAX) {
                    printf("%.6f,", SEMI2DEG(value));
                } else {
                    printf("INVALID,");
                }
                break;
            }
            case TYPE_TIME:
            {
                uint32_t value = *(uint32_t *)ptr;;

                if (value != UINT32_MAX) {
                    time_t     tval;
                    struct tm  tmval;
                    char       buf[128];

                    tval = value + TIME_OFFSET;
                    localtime_r(&tval,&tmval);
                    strftime(buf,sizeof(buf)-1,"%FT%T%z",&tmval);

                    printf("%s,", buf);
                } else {
                    printf("INVALID,");
                }

                break;
            }
            default:
                printf("Unknwon type for field '%s'\n", record[i].label);
        }
    }
    printf("\n");
}



