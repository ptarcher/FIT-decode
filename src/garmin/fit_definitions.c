#include <stdint.h>
#include <stdlib.h>

#include "garmin/fit_definitions.h"
/*****************************************************************************/
struct fit_definitions defs;
/*****************************************************************************/

void init_definitions(void) {
    defs.nelms = 0;
    defs.defs  = malloc(sizeof(*(defs.defs)));
    defs.size  = 1;
}

void insert_definition(struct fit_definition *def) {
    if (defs.size == (defs.nelms + 1)) {
        int new_size;
        struct fit_definition **new_defs;
        
        new_size = defs.size * 2;
        new_defs = realloc(defs.defs, sizeof(*new_defs)*new_size);

        if (new_defs == NULL) {
            return;
        }

        defs.defs = new_defs;
        defs.size = new_size;
    }

    defs.defs[defs.nelms++] = def;
}

struct fit_definition *search_for_def(uint8_t record_num) {
    int i;

    for (i = 0; i < defs.nelms; i++) {
        if (defs.defs[i]->record_num == record_num) {
            return defs.defs[i];
        }
    }

    return NULL;
}


