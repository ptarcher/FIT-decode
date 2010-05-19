#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "garmin/garmin.h"
#include "garmin/fit.h"
#include "garmin/fit_types.h"
#include "print_csv.h"
#include "print_xml.h"

/*****************************************************************************/
#define SIZEOFARRAY(a) (sizeof(a)/sizeof(a[0]))

extern FILE *fp;
extern int nbytes;
extern int display;
/*****************************************************************************/

#define DATA_RECORD(t, m, i, s, u, vt) \
    {.index = i, .label = #m, .offset = offsetof(t, m), .type = vt, .units = u, .scale = s,}

struct fit_record_data file_info_data[] = {
    DATA_RECORD(struct fit_file_info, serial_number, 3, 1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_file_info, time_created,  4, 1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_file_info, manufacturer,  1, 1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_file_info, product,       2, 1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_file_info, number,        5, 1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_file_info, type,          0, 1, "", TYPE_UINT8),
};

struct fit_record_data file_creator_data[] = {
    DATA_RECORD(struct fit_file_creator, software_version, 0, 1/100.0, "", TYPE_UINT16),
    DATA_RECORD(struct fit_file_creator, hardware_version, 1, 1, "", TYPE_UINT8),
};

struct fit_record_data event_data[] = {
    DATA_RECORD(struct fit_event, timestamp,     253, 1, "", TYPE_TIME),
    DATA_RECORD(struct fit_event, timer_trigger, 3,   1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_event, event,         0,   1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_event, event_type,    1,   1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_event, event_group,   4,   1, "", TYPE_ENUM),
};

struct fit_record_data device_info_data[] = {
    DATA_RECORD(struct fit_device_info, timestamp,         253, 1, "", TYPE_TIME),
    DATA_RECORD(struct fit_device_info, serial_number,     3,   1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_device_info, xxx7,              7,   1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_device_info, xxx8,              8,   1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_device_info, manufacturer,      2,   1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_device_info, product,           4,   1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_device_info, software_version,  5,   1/100.0, "", TYPE_UINT16),
    DATA_RECORD(struct fit_device_info, battery_voltage,   10,  1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_device_info, device_index,      0,   1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_device_info, device_type,       1,   1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_device_info, hardware_version,  6,   1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_device_info, battery_status,    11,  1, "", TYPE_UINT8),
};


struct fit_record_data record_data[] = {
    DATA_RECORD(struct fit_record, timestamp,        253, 1, "",    TYPE_TIME),
    DATA_RECORD(struct fit_record, position_lat,     0, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_record, position_long,    1, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_record, distance,         5, 1/100.0, "m", TYPE_UINT32),
    DATA_RECORD(struct fit_record, time_from_course, 11, 1/100.0, "s", TYPE_INT32),
    DATA_RECORD(struct fit_record, altitude,         2, 1/5.0, "m", TYPE_UINT16),
    DATA_RECORD(struct fit_record, speed,            6, 3.6/1000.0, "km/h", TYPE_UINT16),
    DATA_RECORD(struct fit_record, power,            7, 1, "w",   TYPE_UINT16),
    DATA_RECORD(struct fit_record, grade,            9, 1, "%",   TYPE_INT16),
    DATA_RECORD(struct fit_record, heart_rate,       3, 1, "bpm", TYPE_UINT8),
    DATA_RECORD(struct fit_record, cadence,          4, 1, "",    TYPE_UINT8),
    DATA_RECORD(struct fit_record, resistance,       10, 1, "",   TYPE_UINT8),
    DATA_RECORD(struct fit_record, temperature,      13, 1, "deg.C", TYPE_UINT8),
};

struct fit_record_data session_data[] = {
    DATA_RECORD(struct fit_session, timestamp,           253, 1, "", TYPE_TIME),
    DATA_RECORD(struct fit_session, start_time,          2, 1, "", TYPE_TIME),
    DATA_RECORD(struct fit_session, start_position_lat,  3, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_session, start_position_long, 4, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_session, total_elapsed_time,  7, 1/1000.0, "s", TYPE_UINT32),
    DATA_RECORD(struct fit_session, total_timer_time,    8, 1/1000.0, "s", TYPE_UINT32),
    DATA_RECORD(struct fit_session, total_distance,      9, 1/100.0, "m", TYPE_UINT32),
    DATA_RECORD(struct fit_session, total_cycles,        10, 1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_session, nec_lat,             29, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_session, nec_long,            30, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_session, swc_lat,             31, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_session, swc_long,            32, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_session, message_index,       254, 1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_session, total_calories,      11, 1, "cal", TYPE_UINT16),
    DATA_RECORD(struct fit_session, total_fat_calories,  13, 1, "cal", TYPE_UINT16),
    DATA_RECORD(struct fit_session, avg_speed,           14, 3.6/1000.0, "km/h", TYPE_UINT16),
    DATA_RECORD(struct fit_session, max_speed,           15, 3.6/1000.0, "km/h", TYPE_UINT16),
    DATA_RECORD(struct fit_session, avg_power,           20, 1, "W", TYPE_UINT16),
    DATA_RECORD(struct fit_session, max_power,           21, 1, "W", TYPE_UINT16),
    DATA_RECORD(struct fit_session, total_ascent,        22, 1, "m", TYPE_UINT16),
    DATA_RECORD(struct fit_session, total_descent,       23, 1, "m", TYPE_UINT16),
    DATA_RECORD(struct fit_session, first_lap_index,     25, 1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_session, num_laps,            26, 1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_session, event,               0, 1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_session, event_type,          1, 1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_session, sport,               5, 1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_session, sub_sport,           6, 1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_session, avg_heart_rate,      16, 1, "bpm", TYPE_UINT8),
    DATA_RECORD(struct fit_session, max_heart_rate,      17, 1, "bpm", TYPE_UINT8),
    DATA_RECORD(struct fit_session, avg_cadence,         18, 1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_session, max_cadence,         19, 1, "", TYPE_UINT8),
};

struct fit_record_data lap_data[] = {
    DATA_RECORD(struct fit_lap, timestamp,           253, 1, "",  TYPE_TIME),
    DATA_RECORD(struct fit_lap, start_time,          2, 1, "",    TYPE_TIME),
    DATA_RECORD(struct fit_lap, start_position_lat,  3, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, start_position_long, 4, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, end_position_lat,    5, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, end_position_long,   6, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, total_elapsed_time,  7, 1/1000.0, "s", TYPE_UINT32),
    DATA_RECORD(struct fit_lap, total_timer_time,    8, 1/1000.0, "s", TYPE_UINT32),
    DATA_RECORD(struct fit_lap, total_distance,      9, 1/100.0, "m", TYPE_UINT32),
    DATA_RECORD(struct fit_lap, total_cycles,        10, 1, "", TYPE_UINT32),
    DATA_RECORD(struct fit_lap, nec_lat,             29, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, nec_long,            30, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, swc_lat,             31, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, swc_long,            32, 1, "deg", TYPE_POS),
    DATA_RECORD(struct fit_lap, message_index,       254, 1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, total_calories,      11, 1, "cal", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, total_fat_calories,  12, 1, "cal", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, avg_speed,           13, 3.6/1000.0, "km/h", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, max_speed,           14, 3.6/1000.0, "km/h", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, avg_power,           19, 1, "W", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, max_power,           20, 1, "W", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, total_ascent,        21, 1, "m", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, total_descent,       22, 1, "m", TYPE_UINT16),
    DATA_RECORD(struct fit_lap, event,               0, 1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_lap, event_type,          1, 1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_lap, avg_heart_rate,      15, 1, "bpm", TYPE_UINT8),
    DATA_RECORD(struct fit_lap, max_heart_rate,      16, 1, "bpm", TYPE_UINT8),
    DATA_RECORD(struct fit_lap, avg_cadence,         17, 1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_lap, max_cadence,         18, 1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_lap, intensity,           23, 1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_lap, lap_trigger,         24, 1, "", TYPE_ENUM),
    DATA_RECORD(struct fit_lap, sport,               25, 1, "", TYPE_ENUM),
};

struct fit_record_data activity_data[] = {
    DATA_RECORD(struct fit_activity, timestamp,           253, 1, "", TYPE_TIME),
    DATA_RECORD(struct fit_activity, local_timer_time,    0,   1/1000.0, "s", TYPE_UINT32),
    DATA_RECORD(struct fit_activity, num_sessions,        1,   1, "", TYPE_UINT16),
    DATA_RECORD(struct fit_activity, type,                2,   1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_activity, event,               3,   1, "", TYPE_UINT8),
    DATA_RECORD(struct fit_activity, event_type,          4,   1, "", TYPE_UINT8),
};




/*****************************************************************************/
#if 0
void print_record_data(void *data, struct fit_record_data *record, int nelms) {
    int i;
    uintptr_t ptr;

    for (i = 0; i < nelms; i++) {
        ptr = ((uintptr_t)data) + record[i].offset;

        switch(record[i].type) {
            case TYPE_STRING:
            {
                char *str = (char *)ptr;;
                printf("%20s = %s %s\n", record[i].label, str, record[i].units);
                break;
            }
            case TYPE_UINT8:
            {
                uint8_t value = *(uint8_t *)ptr;;
                if (value != UINT8_MAX) {
                    printf("%20s = %.2f %s\n", 
                            record[i].label, value * record[i].scale, record[i].units);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
                }
                break;
            }
            case TYPE_UINT16:
            {
                uint16_t value = *(uint16_t *)ptr;;
                if (value != UINT16_MAX) {
                    printf("%20s = %.2f %s\n", 
                            record[i].label, value * record[i].scale, record[i].units);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
                }
                break;
            }
            case TYPE_UINT32:
            {
                uint32_t value = *(uint32_t *)ptr;;
                if (value != UINT32_MAX) {
                    printf("%20s = %.2f %s\n", 
                            record[i].label, value * record[i].scale, record[i].units);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
                }
                break;
            }
            case TYPE_INT8:
            {
                int8_t value = *(int8_t *)ptr;;
                if (value != INT8_MAX) {
                    printf("%20s = %.2f %s\n", 
                            record[i].label, value * record[i].scale, record[i].units);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
                }
                break;
            }
            case TYPE_INT16:
            {
                int16_t value = *(int16_t *)ptr;;
                if (value != INT16_MAX) {
                    printf("%20s = %.2f %s\n", 
                            record[i].label, value * record[i].scale, record[i].units);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
                }
                break;
            }
            case TYPE_INT32:
            {
                int32_t value = *(int32_t *)ptr;;
                if (value != INT32_MAX) {
                    printf("%20s = %.2f %s\n", 
                            record[i].label, value * record[i].scale, record[i].units);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
                }
                break;
            }
                
            case TYPE_POS:
            {
                int32_t value = *(int32_t *)ptr;;
                if (value != INT32_MAX) {
                    printf("%20s = %.6f %s\n",
                            record[i].label, SEMI2DEG(value), record[i].units);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
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

                    printf("%20s = %s\n", record[i].label, buf);
                } else {
                    printf("%20s = INVALID\n", record[i].label);
                }

                break;
            }
            default:
                printf("Unknwon type for field '%s'\n", record[i].label);
        }
    }
}

/*****************************************************************************/
void print_header(struct fit_header *hdr) {
    int i;

    printf("h_len = %d, protocol_version = %u, profile_version = %u, xxx2 = %u, file_size = %u\n", 
        hdr->h_len, hdr->protocol_version, hdr->profile_version, hdr->xxx2, hdr->file_size);
    for (i = 0; i < sizeof(hdr->sig); i++) {
        printf("sig[%d]    = %c\n", i, hdr->sig[i]);
    }
    printf("\n");
}

void print_field_desc(struct fit_field_desc *field) {
    printf("index = %d, size = %d, field_type = %d\n", 
        field->index, field->size, field->type);
}

void print_record_definition(struct fit_record_definition *record) {
    printf("reserved   = %u\n", record->reserved);
    printf("endian     = %u\n", record->endian);
    printf("msg_num    = %u\n", record->msg_num);
    printf("nfields    = %u\n", record->nfields);
}

void print_record_hdr(uint8_t record_hdr) {
    if (record_hdr & RECORD_CTH_LOCAL_MESSAGE_TYPE_HDR) {
        printf("CTH_LOCAL_MESSAGE\n");
    }
    if (record_hdr & RECORD_DEFINITION_MESSAGE_HDR) {
        //printf("DEFINITION_MESSAGE\n");
    }
    if (record_hdr & RECORD_COMPRESSED_TIMESTAMP_HDR) {
        printf("COMPRESSED_TIMESTAMP\n");
    }
}
#endif
int check_header(struct fit_header *hdr) {
    if (hdr->h_len != sizeof(*hdr)) {
        fprintf(stderr, 
            "Not a .FIT header (%d != header_length = %d)\n",
            hdr->h_len,
            sizeof(*hdr));
        exit(1);
    } else if (0) {
#define FIT_SIGNATURE ".FIT"

        fprintf(stderr, 
            "Not a .FIT header (%d != signature '.FIT')\n",
            hdr->h_len);
        exit(1);
    }

    return 0;
}



void fetch_record_definition(uint8_t record_num) {
    int nelms;

    struct fit_definition *def;
    def = calloc(1, sizeof(*def));

    /* Fetch the record header */
    if ((nelms = fread(&(def->record), sizeof(def->record), 1, fp)) != 1) {
        fprintf(stderr, "Unable to read fit_record_desc\n");
        exit(1);
    }
    nbytes += sizeof(def->record);
    //printf("\nDEFINITION:\n");
    //print_record_definition(&(def->record));

    def->fields = calloc(1, sizeof(*def->fields)*def->record.nfields);

    /* Fetch the record fields */
    int i;
    for (i = 0; i < def->record.nfields; i++) {
        if ((nelms = fread(&(def->fields[i]), sizeof(*(def->fields)), 1, fp)) != 1) {
            fprintf(stderr, "Unable to read fit_field_desc\n");
            exit(1);
        }
        nbytes += sizeof(*(def->fields));

        //printf("%d: ", i);
        //print_field_desc(&(def->fields[i]));
        def->record_size += def->fields[i].size;
    }
    //printf("\n");

    def->record_num = record_num;

    insert_definition(def);
}

int read_data(void *data, size_t len, struct fit_definition *def, struct fit_record_data *record_data, int record_data_nelms) {
    int i, j;
    int nelms;

    size_t offset;
    uint8_t *buf = data;

    for (i = 0, offset = 0; i < def->record.nfields; i++) {
        /* Search for the offset */
        if (record_data) {
            for (j = 0; j < record_data_nelms; j++) {
                if (record_data[j].index == def->fields[i].index) {
                    offset = record_data[j].offset;
                    break;
                }
            }
        }
        if (record_data == NULL || j == record_data_nelms) {
            /* TODO: Do a temporary read to keep the file lined up */
            //fprintf(stdout, "Unknown index %d\n", def->fields[i].index);
            uint8_t tmp[def->fields[i].size];
            if ((nelms = fread(tmp, sizeof(uint8_t), def->fields[i].size,fp)) != def->fields[i].size) {
                fprintf(stdout, "Unable to read unknown index\n");
            }
            nbytes += sizeof(uint8_t) * def->fields[i].size;
            continue;
        }

        if ((nelms = fread(&(buf[offset]), sizeof(uint8_t), def->fields[i].size,fp)) != def->fields[i].size) {
            fprintf(stdout, "Unable to read data at byte %d\n", nbytes);
            exit(1);

        }
        nbytes += sizeof(uint8_t) * def->fields[i].size;
    }

    return 0;
}

struct fit_record_data *search_for_data(int record_num, int *data_nelms) {
    switch(record_num) {
        case FITFILE_MSGTYPE_FILE_ID:
            *data_nelms = SIZEOFARRAY(file_info_data);
            return file_info_data;
        case FITFILE_MSGTYPE_FILE_CREATOR:
            *data_nelms = SIZEOFARRAY(file_creator_data);
            return file_creator_data;
        case FITFILE_MSGTYPE_EVENT:
            *data_nelms = SIZEOFARRAY(event_data);
            return event_data;
        case FITFILE_MSGTYPE_DEVICE_INFO:
            *data_nelms = SIZEOFARRAY(device_info_data);
            return device_info_data;
        case FITFILE_MSGTYPE_RECORD:
            *data_nelms = SIZEOFARRAY(record_data);
            return record_data;
        case FITFILE_MSGTYPE_LAP:
            *data_nelms = SIZEOFARRAY(lap_data);
            return lap_data;
        case FITFILE_MSGTYPE_SESSION:
            *data_nelms = SIZEOFARRAY(session_data);
            return session_data;
        case FITFILE_MSGTYPE_ACTIVITY:
            *data_nelms = SIZEOFARRAY(activity_data);
            return activity_data;
        default:
            //fprintf(stdout, "Unknown record type %d\n", record_num);
            return NULL;
    }

}

int fit_process_file(void) {
    int nelms;

    /* Check the file is a correct .FIT file by the header */
    struct fit_header hdr;
    if ((nelms = fread(&hdr, sizeof(hdr), 1, fp)) != 1) {
        exit(1);
    }
    //nbytes += sizeof(hdr);
    //print_header(&hdr);
    if (check_header(&hdr)) {
        exit(1);
    }

    print_xml_header();

    while (nbytes < hdr.file_size) {
        uint8_t record_hdr;
        if ((nelms = fread(&record_hdr, sizeof(record_hdr), 1, fp)) != 1) {
        }
        nbytes += sizeof(record_hdr);
        //print_record_hdr(record_hdr);

        if (record_hdr & RECORD_DEFINITION_MESSAGE_HDR) {
            /* The record first contains a definition */
            fetch_record_definition(record_hdr & RECORD_TYPE_MASK);
            continue;
        }

        /* Fetch the definition */
        struct fit_definition *def;
        def = search_for_def(record_hdr & RECORD_TYPE_MASK);
        if (!def) {
            fprintf(stdout, "Unable to find definition for message type\n");
            exit(1);
        }

        /* Search for the data definition */
        struct fit_record_data *data;
        int data_len;
        data = search_for_data(record_hdr & RECORD_TYPE_MASK, &data_len);
        
        uint8_t array[def->record_size];
        read_data(array, sizeof(array), def, data, data_len);

        /* Read the record out */
        switch(record_hdr & RECORD_TYPE_MASK) {
            case FITFILE_MSGTYPE_RECORD:
            {
                //struct fit_record *record = (struct fit_record *) array;
                if (display == DISPLAY_RECORDS) {
                    print_xml_open_tag("record", stdout, 1);
                    printf("\n");
                    print_xml_record_data(array, record_data,
                            SIZEOFARRAY(record_data), stdout, 2);
                    print_xml_close_tag("record", stdout, 1);
                    printf("\n");
                    //print_record_data(array, record_data,
                }
            }
            break;

            case FITFILE_MSGTYPE_LAP:
            {
                //struct fit_lap *lap = (struct fit_lap *) array;
                if (display == DISPLAY_LAPS) {
                    print_xml_open_tag("lap", stdout, 1);
                    printf("\n");
                    print_xml_record_data(array, lap_data, 
                            SIZEOFARRAY(lap_data), stdout, 2);
                    print_xml_close_tag("lap", stdout, 1);
                    printf("\n");
                }
            }
            break;

            case FITFILE_MSGTYPE_SESSION:
            {
                //struct fit_session *session = (struct fit_session *) array;
                if (display == DISPLAY_SESSIONS) {
                    print_xml_open_tag("session", stdout, 1);
                    printf("\n");
                    print_xml_record_data(array, session_data,
                            SIZEOFARRAY(session_data), stdout, 2);
                    print_xml_close_tag("session", stdout, 1);
                    printf("\n");
                }
            }
            break;
            default:
                //fprintf(stdout, "Unknown record type %d\n", record_hdr & RECORD_TYPE_MASK);
                break;
        }

#if 0
        switch(record_hdr & RECORD_TYPE_MASK) {
            case FITFILE_MSGTYPE_FILE_ID:
                printf("FILE_ID\n");
                print_record_data(array, file_info_data,
                                         SIZEOFARRAY(file_info_data));
                break;
            case FITFILE_MSGTYPE_FILE_CREATOR:
                printf("FILE_CREATOR\n");
                print_record_data(array, file_creator_data,
                                         SIZEOFARRAY(file_creator_data));
                break;

            case FITFILE_MSGTYPE_EVENT:
            {
                printf("EVENT\n");
                print_record_data(array, event_data,
                                         SIZEOFARRAY(event_data));
                //struct fit_event *event = (struct fit_event *) array;
            }
            break;

            case FITFILE_MSGTYPE_DEVICE_INFO:
            {
                printf("DEVICE_INFO\n");
                print_record_data(array, device_info_data,
                                         SIZEOFARRAY(device_info_data));

                //struct fit_device_info *device_info = (struct fit_device_info *) array;
            }
            break;

            case FITFILE_MSGTYPE_RECORD:
            {
                printf("RECORD\n");
                print_record_data(array, record_data,
                                         SIZEOFARRAY(record_data));
                //struct fit_record *record = (struct fit_record *) array;
            }
            break;

            case FITFILE_MSGTYPE_LAP:
            {
                printf("LAP\n");
                print_record_data(array, lap_data,
                                         SIZEOFARRAY(lap_data));
                //struct fit_lap *lap = (struct fit_lap *) array;
            }
            break;

            case FITFILE_MSGTYPE_SESSION:
            {
                printf("SESSION\n");
                print_record_data(array, session_data,
                                         SIZEOFARRAY(session_data));
                //struct fit_session *session = (struct fit_session *) array;
            }
            break;
            case FITFILE_MSGTYPE_ACTIVITY:
            {
                printf("ACTIVITY\n");
                print_record_data(array, activity_data,
                                         SIZEOFARRAY(activity_data));
                //struct fit_activity *activity = (struct fit_activity *) array;

            }
            break;

            default:
                fprintf(stdout, "Unknown record type %d\n", record_hdr & RECORD_TYPE_MASK);
        }
#endif
        //printf("\n");
    }

    /* Perform CRC check */
    #if 0
    uint16_t crc;
    if ((nelms = fread(&crc, sizeof(crc), 1, fp)) != 1) {
    }
    printf("crc = 0x%04x\n", crc);
    #endif

    fclose(fp);

    print_xml_footer();

    return 0;
}



