#ifndef _FIT_H_
#define _FIT_H_

#include "garmin/fit_types.h"
#include "garmin/fit_definitions.h"

typedef enum {
    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_ENUM,
    TYPE_TIME,
    TYPE_STRING,
    TYPE_POS,
} fit_field_type_t;

struct fit_record_data {
    const int    index;
    const char  *label;
    const int    type;
    const int    offset;
    const double scale;
    const char  *units;
};

void print_record_data(void *data, struct fit_record_data *record, int nelms);
void print_header(struct fit_header *hdr);
int check_header(struct fit_header *hdr);
void print_field_desc(struct fit_field_desc *field);
void print_record_definition(struct fit_record_definition *record);
void print_record_hdr(uint8_t record_hdr);
void fetch_record_definition(uint8_t record_num);
int read_data(void *data, size_t len, struct fit_definition *def, struct fit_record_data *record_data, int record_data_nelms);
struct fit_record_data *search_for_data(int record_num, int *data_nelms);

int fit_process_file(void);

#endif /* _FIT_H_ */
