/* ------------------------------------------------------------------------- */
/* 7.3   Basic Data Types                                                    */
/* ------------------------------------------------------------------------- */

#include "fit.h"

/* conversions */
#define DEGREES      180.0
#define SEMICIRCLES  0x80000000

#define SEMI2DEG(a)  (double)(a) * DEGREES / SEMICIRCLES
#define DEG2SEMI(a)  rint((double)(a) * SEMICIRCLES / DEGREES)

#define DEG2RAD(a)   (a) * M_PI / DEGREES
#define RAD2DEG(a)   (a) * DEGREES / M_PI

/* Number of seconds since Dec 31, 1989, 12:00 AM (UTC) */
#define TIME_OFFSET 631065600


/* Place in a garmin init function */
void init_definitions(void);
int check_header(struct fit_header *hdr);
