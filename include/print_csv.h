#ifndef _PRINT_CSV_H_
#define _PRINT_CSV_H_

typedef enum {
    DISPLAY_LAPS,
    DISPLAY_SESSIONS,
    DISPLAY_RECORDS,
} csv_display_t;

void printCSVHeader(csv_display_t display);
void print_csv_record_data(void *data, struct fit_record_data *record, int nelms);

#endif /* _PRINT_CSV_H_ */
