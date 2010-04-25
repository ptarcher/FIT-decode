#ifndef _FIT_DEFINITIONS_H_
#define _FIT_DEFINITIONS_H_

#include <stddef.h>
#include <stdint.h>

#include "garmin/fit_types.h"

struct fit_definition {
    uint8_t record_num;
    struct  fit_record_definition record;
    struct  fit_field_desc        *fields;
    size_t  record_size;
};

struct fit_definitions {
    struct fit_definition **defs;
    int nelms;
    int size;
};

void init_definitions(void);
void insert_definition(struct fit_definition *def);
struct fit_definition *search_for_def(uint8_t record_num);

#endif /* _FIT_DEFINITIONS_H_ */
