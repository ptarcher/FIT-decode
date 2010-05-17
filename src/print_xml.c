#include <stdio.h>
#include <time.h>
#include <string.h>


#include "garmin/garmin.h"
#include "garmin/fit.h"
#include "print_xml.h"

/*****************************************************************************/
static void print_spaces(FILE *fp, int spaces);

#define SIZEOFARRAY(a) (sizeof(a)/sizeof(a[0]))

/*****************************************************************************/

void print_xml_header() {
    //int i;

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

void print_xml_record_data(void *data, struct fit_record_data *record, int nelms, FILE *f, int spaces) {
    int i;
    uintptr_t ptr;

    for (i = 0; i < nelms; i++) {
        ptr = ((uintptr_t)data) + record[i].offset;

        switch(record[i].type) {
            case TYPE_STRING:
            {
                char *str = (char *)ptr;
                print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                fprintf(f, "%s", str);
                print_xml_close_tag(record[i].label, stdout, 0);
                fprintf(f, "\n");
                break;
            }
            case TYPE_ENUM:
            {
                uint8_t value = *(uint8_t *)ptr;;
                if (value != UINT8_MAX) {
                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%d", value);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
            case TYPE_UINT8:
            {
                uint8_t value = *(uint8_t *)ptr;;
                if (value != UINT8_MAX) {
                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%.2f", value * record[i].scale);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
            case TYPE_UINT16:
            {
                uint16_t value = *(uint16_t *)ptr;;
                if (value != UINT16_MAX) {
                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%.2f", value * record[i].scale);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
            case TYPE_UINT32:
            {
                uint32_t value = *(uint32_t *)ptr;;
                if (value != UINT32_MAX) {
                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%.2f", value * record[i].scale);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
            case TYPE_INT8:
            {
                int8_t value = *(int8_t *)ptr;;
                if (value != INT8_MAX) {
                    print_xml_open_tag_with_units(record[i].label,record[i].units, stdout, spaces);
                    fprintf(f, "%.2f", value * record[i].scale);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
            case TYPE_INT16:
            {
                int16_t value = *(int16_t *)ptr;;
                if (value != INT16_MAX) {
                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%.2f", value * record[i].scale);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
            case TYPE_INT32:
            {
                int32_t value = *(int32_t *)ptr;;
                if (value != INT32_MAX) {
                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%.2f", value * record[i].scale);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
                
            case TYPE_POS:
            {
                int32_t value = *(int32_t *)ptr;;
                if (value != INT32_MAX) {
                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%.6f", SEMI2DEG(value));
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
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

                    print_xml_open_tag_with_units(record[i].label, record[i].units, stdout, spaces);
                    fprintf(f, "%s", buf);
                    print_xml_close_tag(record[i].label, stdout, 0);
                    fprintf(f, "\n");
                }
                break;
            }
            default:
                fprintf(stderr, "Unknwon type for field '%s'\n", record[i].label);
        }
    }
}


static void
print_spaces ( FILE * fp, int spaces )
{
  int i;

  for ( i = 0; i < spaces; i++ ) {
    fprintf(fp," ");
  }
}


void
print_xml_open_tag ( const char * tag, FILE * fp, int spaces ) 
{
  print_spaces(fp,spaces);
  fprintf(fp,"<%s>",tag);
}

void
print_xml_open_tag_with_units(const char *tag, const char *units, FILE * fp, int spaces ) 
{
  print_spaces(fp,spaces);
  if (strlen(units)) {
      fprintf(fp,"<%s units=\"%s\">", tag, units);
  } else {
      fprintf(fp,"<%s>", tag);
  }
}


void
print_xml_close_tag ( const char * tag, FILE * fp, int spaces ) 
{
  print_spaces(fp,spaces);
  fprintf(fp,"</%s>",tag);
}



