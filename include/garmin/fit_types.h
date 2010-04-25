#ifndef _FIT_TYPES_H_
#define _FIT_TYPES_H_

#include <stdint.h>

/* Record Header
 * | Compressed Timestamp (7) | Definition Message (6) | CTH MESSAGE (5) | Type (0-4) |
 */
#define RECORD_TYPE_LENGTH                       4
#define RECORD_CTH_LOCAL_MESSAGE_TYPE_HDR_OFFSET 5
#define RECORD_DEFINITION_MESSAGE_HDR_OFFSET     6
#define RECORD_COMPRESSED_TIMESTAMP_HDR_OFFSET   7

#define RECORD_CTH_LOCAL_MESSAGE_TYPE_HDR (1 << RECORD_CTH_LOCAL_MESSAGE_TYPE_HDR_OFFSET)
#define RECORD_DEFINITION_MESSAGE_HDR (1 << RECORD_DEFINITION_MESSAGE_HDR_OFFSET)
#define RECORD_COMPRESSED_TIMESTAMP_HDR (1 << RECORD_COMPRESSED_TIMESTAMP_HDR_OFFSET)

#define RECORD_TYPE_MASK ((1 << RECORD_TYPE_LENGTH) - 1)

/* header */
struct fit_header {
  uint8_t h_len;
  uint8_t protocol_version;
  uint8_t profile_version;
  uint8_t xxx2; /* ??? */
  uint32_t file_size; /* actually file size (in octets) - 14, excluding this header and trailing CRC-16? */
  uint8_t sig[4]; /* always ".FIT" */
} __attribute__((packed));

/* descriptor of fields of data records */
struct fit_field_desc {
  uint8_t index;
  uint8_t size; /* size of this field in octets */
  uint8_t type; /* type of single datum:
                         140 --> unit ID, unsigned 32 bit integer,
                         139 --> ANT+ sensor ID, unsigned 16 bit integer,
                         134 --> unsigned 32 bit integer,
                         133 --> signed 32 bit integer,
                         132 --> unsigned 16 bit integer,
                         131 --> signed 16 bit integer,
                          10 --> ???, 8 bit integer, appears in the last fit_field_desc of descriptor with rec_type 37,
                           7 --> whole field is a zero-terminated string of (printable?) 8 bit integer, 
                           2 --> unsigned 8 bit integer,
                           1 --> signed 8 bit integer,
                           0 --> C's enum like data type?
                       field may be an array of data of this type,
                       so value of size field must be multiple of size of this type
                       (applicative to multi-octets datum?) */
} __attribute__((packed));

/* record definition */
struct fit_record_definition {
  uint8_t  reserved;
  uint8_t  endian;
  uint16_t msg_num;
  uint8_t  nfields; /* number of succeeding fit_field_desc's */
} __attribute__((packed));

/* defined by record descriptor with rec_type 0 */
struct fit_file_info {
  uint32_t serial_number;
  uint32_t time_created; /* when start button pressed, seconds since 1989-12-31 00:00:00 GMT */
  uint16_t manufacturer; /* FITFILE_MANUFACTURER_* enum */
  uint16_t product;      /* FITFILE_GARMIN_PRODUCT_* enum */
  uint16_t number;
  uint8_t  type;         /* FITFILE_FILE_* enum */
} __attribute__((packed));

/* defined by record descriptor with rec_type 1 */
struct fit_file_creator {
  uint16_t software_version;
  uint8_t  hardware_version;
} __attribute__((packed));

typedef struct fit_file_creator fit_file_creator_t;

/* defined by record descriptor with rec_type 2 */
struct fit_event {
  uint32_t timestamp;
  uint32_t timer_trigger;
  uint8_t  event;
  uint8_t  event_type;
  uint8_t  event_group;
} __attribute__((packed));

/* defined by record descriptor with rec_type 3 */
struct fit_device_info {
  uint32_t timestamp;
  uint32_t serial_number;
  uint32_t xxx7;
  uint32_t xxx8;
  uint16_t manufacturer;
  uint16_t product;
  uint16_t software_version;
  uint16_t battery_voltage;
  uint8_t  device_index;
  uint8_t  device_type;
  uint8_t  hardware_version;
  uint8_t  battery_status;
} __attribute__((packed));

/* record type 4 is missing */

/* defined by record descriptor with rec_type 5 */
struct fit_record {
  uint32_t timestamp;
  int32_t  position_lat;
  int32_t  position_long;
  uint32_t distance;
  int32_t  time_from_course;
  uint16_t altitude; /* scale by 5 */
  uint16_t speed;
  uint16_t power;
  int16_t  grade;
  uint8_t  heart_rate;
  uint8_t  cadence;
  uint8_t  resistance;
  int8_t   temperature;
} __attribute__((packed));

/* defined by record descriptor with rec_type 6 */
struct fit_lap {
  uint32_t timestamp;
  uint32_t start_time;
  int32_t  start_position_lat;
  int32_t  start_position_long;
  int32_t  end_position_lat;
  int32_t  end_position_long;
  uint32_t total_elapsed_time;
  uint32_t total_timer_time;
  uint32_t total_distance;
  uint32_t total_cycles;
  int32_t  nec_lat;
  int32_t  nec_long;
  int32_t  swc_lat;
  int32_t  swc_long;
  uint16_t message_index;
  uint16_t total_calories;
  uint16_t total_fat_calories;
  uint16_t avg_speed;
  uint16_t max_speed;
  uint16_t avg_power;
  uint16_t max_power;
  uint16_t total_ascent;
  uint16_t total_descent;
  uint8_t  event;
  uint8_t  event_type;
  uint8_t  avg_heart_rate;
  uint8_t  max_heart_rate;
  uint8_t  avg_cadence;
  uint8_t  max_cadence;
  uint8_t  intensity;
  uint8_t  lap_trigger;
  uint8_t  sport;
} __attribute__((packed));

/* defined by record descriptor with rec_type 7 */
struct fit_session {
  uint32_t timestamp;
  uint32_t start_time;
  int32_t  start_position_lat;
  int32_t  start_position_long;
  uint32_t total_elapsed_time;
  uint32_t total_timer_time;
  uint32_t total_distance;
  uint32_t total_cycles;
  int32_t  nec_lat;
  int32_t  nec_long;
  int32_t  swc_lat;
  int32_t  swc_long;
  uint16_t message_index;
  uint16_t total_calories;
  uint16_t total_fat_calories;
  uint16_t avg_speed;
  uint16_t max_speed;
  uint16_t avg_power;
  uint16_t max_power;
  uint16_t total_ascent;
  uint16_t total_descent;
  uint16_t first_lap_index;
  uint16_t num_laps;
  uint8_t  event;
  uint8_t  event_type;
  uint8_t  sport;
  uint8_t  sub_sport;
  uint8_t  avg_heart_rate;
  uint8_t  max_heart_rate;
  uint8_t  avg_cadence;
  uint8_t  max_cadence;
} __attribute__((packed));

/* defined by record descriptor with rec_type 8 */
struct fit_activity {
  uint32_t timestamp;
  uint32_t local_timer_time;
  uint16_t num_sessions;
  uint8_t  type;
  uint8_t  event;
  uint8_t  event_type;
};


/*************************************************************************/

/* defined by record descriptor with rec_type 21 */
struct fit_pause {
  uint8_t  rec_id;
  uint32_t date; /* seconds since 1989-12-31 00:00:00 GMT */
  uint8_t  xxx5[7]; /* ??? */
} __attribute__((packed));

/* defined by record descriptor with rec_type 22 */
struct fit_sensor {
  uint8_t rec_id;
  uint32_t date; /* seconds since 1989-12-31 00:00:00 GMT */
  uint8_t xxx0; /* status of power meter or cadence sensor?
                 max. if either power or cadence is lost,
                 2 if either power or cadence is found */
  uint8_t xxx1; /* status of power meter or cadence sensor?
                 max. if either power or cadence is lost,
                 2 if either power or cadence is found */
  uint8_t xxx2; /* ??? */
  uint8_t xxx3; /* ??? */
  uint8_t xxx4; /* ??? */
  uint8_t xxx5; /* ??? */
  uint8_t xxx6; /* ??? */
  uint8_t xxx7; /* ??? */
  uint8_t xxx8; /* ??? */
} __attribute__((packed));

/* defined by record descriptor with rec_type 23 */
struct fit_unit_info {
  uint8_t rec_id;
  uint32_t date; /* seconds since 1989-12-31 00:00:00 GMT */
  uint32_t unit_id; /* or 0 */
  uint32_t xxx0; /* ???, always max.? */
  uint32_t xxx1; /* ???, always max.? */
  uint16_t xxx2; /* ??? */
  uint16_t product_id; /* or max. */
  uint16_t fw_ver; /* major * 100 + minor, or max. */
  uint16_t xxx3; /* ???, always max.? */
  uint8_t xxx4; /* ??? */
  uint8_t xxx5; /* ??? */
  uint8_t xxx6; /* ??? */
  uint8_t xxx7; /* ??? */
} __attribute__((packed));

/* appears before CRC, defined by record descriptor with rec_type 34 */
struct fit_file_trailer {
  uint8_t rec_id;
  uint32_t date; /* date when reset completed?, seconds since 1989-12-31 00:00:00 GMT */
  uint32_t time; /* stop watch time, mili seconds */
  uint16_t xxx0;
  uint8_t xxx1;
  uint8_t xxx2;
  uint8_t xxx3;
} __attribute__((packed));

/* defined by record descriptor with rec_type 49 */
struct fit_firmware_version {
  uint8_t rec_id;
  uint16_t fw_ver; /* major * 100 + minor */
  uint8_t xxx0;    /* ??? */
} __attribute__((packed));

/***********************************/
/* edge500_* are Edge 500 specific */
/***********************************/
/* whole activity, defined by record descriptor with rec_type 18 */
struct edge500_activity {
  uint8_t rec_id;
  uint32_t reset_date;     /* seconds since 1989-12-31 00:00:00 GMT */
  uint32_t start_date;     /* seconds since 1989-12-31 00:00:00 GMT */
  int32_t start_latitude;  /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t start_longitude; /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  uint32_t time_elapsed;   /* mili seconds */
  uint32_t time;           /* stop watch time, mili seconds */
  uint32_t dist;           /* cm */
  uint32_t xxx0;           /* ???, always max.? */
  int32_t xxx1;            /* ???, always max.? */
  int32_t xxx2;            /* ???, always max.? */
  int32_t xxx3;            /* ???, always max.? */
  int32_t xxx4;            /* ???, always max.? */
  uint16_t xxx5;           /* ??? */
  uint16_t kcal;
  uint16_t xxx6;           /* ???, always max.? */
  uint16_t spd_ave;        /* mm/s, max. if no speed source */
  uint16_t spd_max;        /* mm/s, max. if no speed source */
  uint16_t pw_ave;         /* watt, max. unless sensor present */
  uint16_t pw_max;         /* watt, max. unless sensor present */
  uint16_t total_ascent;   /* m, 0xFFFF should be treated as 0? */
  uint16_t total_descent;  /* m, 0xFFFF should be treated as 0? */
  uint16_t xxx7;           /* ??? */
  uint16_t xxx8;           /* ??? */
  uint8_t xxx9;            /* ??? */
  uint8_t xxx10;           /* ??? */
  uint8_t xxx11;           /* ??? */
  uint8_t xxx12;           /* ???, always max.?? */
  uint8_t hr_ave;          /* bpm, max. unless sensor present */
  uint8_t hr_max;          /* bpm, max. unless sensor present */
  uint8_t cad_ave;         /* rpm, max. unless sensor present */
  uint8_t cad_max;         /* rpm, max. unless sensor present */
} __attribute__((packed));

/* lap, defined by record descriptor with rec_type 19 */
struct edge500_lap {
  uint8_t rec_type;
  uint32_t end_date; /* seconds since 1989-12-31 00:00:00 GMT,
                      equals the date of the last track point in this lap
                      if data recording rate is 1Hz
                      and if the lap stopped in the 1st half of the track point */
  uint32_t start_date;     /* seconds since 1989-12-31 00:00:00 GMT */
  int32_t start_latitude;  /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t start_longitude; /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  int32_t end_latitude;    /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t end_longitude;   /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  uint32_t time_elapsed;   /* mili seconds */
  uint32_t time;           /* stop watch time, mili seconds */
  uint32_t dist;           /* cm */
  uint32_t xxx0;           /* ???, always max.? */
  int32_t xxx1;            /* ???, always max.? */
  int32_t xxx2;            /* ???, always max.? */
  int32_t xxx3;            /* ???, always max.? */
  int32_t xxx4;            /* ???, always max.? */
  uint16_t nth;            /* lap number (0, 1, 2, ...) */
  uint16_t kcal;           /* based on what? */
  uint16_t xxx5;           /* ???, always max.?? */
  uint16_t spd_ave;        /* mm/s, max. if no speed source */
  uint16_t spd_max;        /* mm/s, max. if no speed source */
  uint16_t pw_ave; /* watt,
                    excluding data during a power meter lost
                    regardless zero-averaging setting,
                    max. unless sensor present */
  uint16_t pw_max;         /* watt, max. unless sensor present */
  uint16_t total_ascent;   /* m */
  uint16_t total_descent;  /* m */
  uint8_t xxx6;            /* ??? */
  uint8_t xxx7;            /* ??? */
  uint8_t hr_ave;          /* bpm, max. unless sensor present */
  uint8_t hr_max;          /* bpm, max. unless sensor present */
  uint8_t cad_ave;         /* rpm, max. unless sensor present */
  uint8_t cad_max;         /* rpm, max. unless sensor present */
  uint8_t xxx8;            /* ??? */
  uint8_t lap_trigger; /* 0 --> manual,
                        2 --> distance,
                        3 --> position where LAP pressed,
                        ... */
  uint8_t xxx9;            /* ??? */
} __attribute__((packed));

/* track point, defined by record descriptor with rec_type 20 */
struct edge500_track_point {
  uint8_t rec_id;
  uint32_t date;      /* seconds since 1989-12-31 00:00:00 GMT */
  int32_t latitude;   /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t longitude;  /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  uint32_t dist;      /* cm */
  int32_t xxx0;       /* ???, always max.? */
  uint16_t alt;       /* uncertain, m / 0.2 + 2500?
                 (cf. max. is 12607m and altitude of toropospher is about 11000m) */
  uint16_t spd;       /* mm/s, max. if no speed source */
  uint16_t pw;        /* watt, max. unless sensor present */
  int16_t xxx1;       /* ???, always max.? */
  uint8_t hr;         /* bpm, max. unless sensor present */
  uint8_t cad;        /* rpm, max. unless sensor present */
  uint8_t xxx2;       /* ???, always max.? */
  int8_t temperature; /* degree C */
} __attribute__((packed));

/*******************************************/
/* fr310xt_* are Forerunner 310XT specific */
/*******************************************/

/*
They are based on files posted by other people into Garmin forum,
and on the assumption that data formats may be analogous to those of Edge 500,
so less reliable than edge500_*'s.
*/

/* track point of location data only, defined by record descriptor with rec_type 20 */
struct fr310xt_location {
  uint8_t rec_id;
  uint32_t date;     /* seconds since 1989-12-31 00:00:00 GMT */
  int32_t latitude;  /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t longitude; /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  uint16_t alt;      /* uncertain, m / 0.2 + 2500?
                 (cf. max. is 12607m and altitude of toropospher is about 11000m) */
} __attribute__((packed));

/* track point without location data, also defined by record descriptor with rec_type 20 */
struct fr310xt_track_point {
  uint8_t rec_id;
  uint32_t date; /* seconds since 1989-12-31 00:00:00 GMT */
  uint32_t dist; /* cm */
  uint16_t spd;  /* mm/s, max. if no speed source */
  uint16_t pw;   /* watt, max. unless sensor present */
  uint8_t hr;    /* bpm, max. unless sensor present */
  uint8_t cad;   /* rpm, max. unless sensor present */
} __attribute__((packed));

/* lap, defined by record descriptor with rec_type 19 */
struct fr310xt_lap {
  uint8_t rec_id;
  uint32_t end_date; /* seconds since 1989-12-31 00:00:00 GMT,
                      equals the date of the last track point in this lap
                      if data recording rate is 1Hz
                      and if the lap stopped in the 1st half of the track point */
  uint32_t start_date;     /* seconds since 1989-12-31 00:00:00 GMT */
  int32_t start_latitude;  /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t start_longitude; /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  int32_t end_latitude;    /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t end_longitude;   /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  uint32_t time_elapsed;   /* mili seconds */
  uint32_t time;           /* stop watch time, mili seconds */
  uint32_t dist;           /* cm */
  uint32_t xxx0;           /* ??? */
  uint16_t nth;            /* lap number (0, 1, 2, ...) */
  uint16_t kcal;           /* based on what? */
  uint16_t xxx1;           /* ???, always max.?? */
  uint16_t spd_ave;        /* mm/s, max. if no speed source */
  uint16_t spd_max;        /* mm/s, max. if no speed source */
  uint16_t pw_ave;         /* watt, max. unless sensor present */
  uint16_t pw_max;         /* watt, max. unless sensor present */
  uint16_t xxx2;           /* ??? */
  uint16_t xxx3;           /* ??? */
  uint8_t xxx4;            /* ??? */
  uint8_t xxx5;            /* ??? */
  uint8_t hr_ave;          /* bpm, max. unless sensor present */
  uint8_t hr_max;          /* bpm, max. unless sensor present */
  uint8_t cad_ave;         /* rpm, max. unless sensor present */
  uint8_t cad_max;         /* rpm, max. unless sensor present */
  uint8_t xxx6;            /* ??? */
  uint8_t lap_trigger; /* 0 --> manual,
                        2 --> distance,
                        3 --> position where LAP pressed,
                        ... */
  uint8_t xxx7;            /* ??? */
} __attribute__((packed));

/* whole activity, defined by record descriptor with rec_type 18 */
struct fr310xt_activity {
  uint8_t rec_id;
  uint32_t reset_date;    /* seconds since 1989-12-31 00:00:00 GMT */
  uint32_t start_date;    /* seconds since 1989-12-31 00:00:00 GMT */
  int32_t start_latitude; /* 180 / 2^31 degree, north if positive, max. if GPS is off */
  int32_t start_longitude; /* 180 / 2^31 degree, east if positive, max. if GPS is off */
  uint32_t time_elapsed; /* mili seconds */
  uint32_t time; /* stop watch time, mili seconds */
  uint32_t dist; /* cm */
  uint32_t xxx0; /* ??? */
  uint16_t xxx1; /* ??? */
  uint16_t kcal;
  uint16_t xxx2; /* ???, always max.? */
  uint16_t spd_ave; /* mm/s, max. if no speed source */
  uint16_t spd_max; /* mm/s, max. if no speed source */
  uint16_t pw_ave; /* watt, max. unless sensor present */
  uint16_t pw_max; /* watt, max. unless sensor present */
  uint16_t xxx3; /* ??? */
  uint16_t xxx4; /* ???, always max.?? */
  uint16_t xxx5; /* ??? */
  uint16_t xxx6; /* ??? */
  uint8_t xxx7; /* ??? */
  uint8_t xxx8; /* ??? */
  uint8_t xxx9; /* ??? */
  uint8_t xxx10; /* ??? */
  uint8_t hr_ave; /* bpm, max. unless sensor present */
  uint8_t hr_max; /* bpm, max. unless sensor present */
  uint8_t cad_ave; /* rpm, max. unless sensor present */
  uint8_t cad_max; /* rpm, max. unless sensor present */
  uint8_t xxx63; /* ??? */
  uint8_t xxx64; /* ???, always max.? */
} __attribute__((packed));

/****************************************************************************/
/* Possible Definition Record Types */
enum {
    FITFILE_MSGTYPE_FILE_ID,
    FITFILE_MSGTYPE_FILE_CREATOR,
    FITFILE_MSGTYPE_EVENT,
    FITFILE_MSGTYPE_DEVICE_INFO,
    FITFILE_MSGTYPE_DEVICE_SETTINGS, /* TODO: */
    FITFILE_MSGTYPE_RECORD,
    FITFILE_MSGTYPE_LAP,             /* TODO: */
    FITFILE_MSGTYPE_SESSION,
    FITFILE_MSGTYPE_ACTIVITY,        /* TODO: */
    FITFILE_MSGTYPE_SOFTWARE, /* Unknown from here below */
    FITFILE_MSGTYPE_CAPABILITIES,
    FITFILE_MSGTYPE_FILE_CAPABILITIES,
    FITFILE_MSGTYPE_MESG_CAPABILITIES,
    FITFILE_MSGTYPE_FIELD_CAPABILITIES,
    FITFILE_MSGTYPE_USER_PROFILE,
    FITFILE_MSGTYPE_HRM_PROFILE,
    FITFILE_MSGTYPE_ZONES_TARGET,
    FITFILE_MSGTYPE_SPORT,
    FITFILE_MSGTYPE_HR_ZONE,
    FITFILE_MSGTYPE_SPEED_ZONE,
    FITFILE_MSGTYPE_POWER_ZONE,
    FITFILE_MSGTYPE_MET_ZONE,
    FITFILE_MSGTYPE_COURSE,
    FITFILE_MSGTYPE_COURSE_POINT,
    FITFILE_MSGTYPE_WORKOUT,
    FITFILE_MSGTYPE_WORKOUT_STEP,
    FITFILE_MSGTYPE_SCHEDULE,
    FITFILE_MSGTYPE_TOTALS,
    FITFILE_MSGTYPE_WEIGHT_SCALE,
};

enum {
    FITFILE_FILE_DEVICE,
    FITFILE_FILE_SETTING,
    FITFILE_FILE_SPORT,
    FITFILE_FILE_ACTIVITY,
    FITFILE_FILE_WORKOUT,
    FITFILE_FILE_COURSE,
    FITFILE_FILE_SCHEDULE,
    FITFILE_FILE_MONITORING,
    FITFILE_FILE_TOTALS,
};

#if 0
enum {
    FITFILE_MESG_NUM_FILE_ID,
    /* TODO: Add the rest of the message number types */
};
#endif

#if 0
enum {
    FITFILE_FILE_FLAGS_READ  = 0x02,
    FITFILE_FILE_FLAGS_WRITE = 0x04,
    FITFILE_FILE_FLAGS_ERASE = 0x08,
};
#endif

#if 0
enum {
    FITFILE_MESG_COUNT_NUM_PER_FILE,
    FITFILE_MESG_COUNT_MAX_PER_FILE,
    FITFILE_MESG_COUNT_MAX_PER_FILE_TYPE,
};
#endif

#if 0
enum {
    FITFILE_GENDER_FEMALE,
    FITFILE_GENDER_MALE,
};
#endif

#if 0
enum {
    FITFILE_LANGUAGE_ENGLISH,
    /* TODO: Add the rest of the languages */
};
#endif

#if 0
enum {
    FITFILE_DISPLAY_MEASURE_METRIC,
    FITFILE_DISPLAY_MEASURE_STATUE,
};
#endif

#if 0
enum {
    FITFILE_DISPLAY_HR_BPM,
    FITFILE_DISPLAY_HR_MAX_PERCENT,
    FITFILE_DISPLAY_HR_RESERVE,
};
#endif

#if 0
enum {
    FITFILE_DISPLAY_POWER_WATTS,
    FITFILE_DISPLAY_POWER_PERCENT_FTP,
};
#endif

#if 0
enum {
    FITFILE_SWITCH_OFF,
    FITFILE_SWITCH_ON,
    FITFILE_SWITCH_AUTO,
};
#endif

enum {
    FITFILE_MANUFACTURER_GARMIN             = 1,
    FITFILE_MANUFACTURER_GARMIN_FR405_ANTFS = 2,
    FITFILE_MANUFACTURER_ZEPHYR             = 3,
    FITFILE_MANUFACTURER_DAYTON             = 4,
    FITFILE_MANUFACTURER_IDT                = 5,
    FITFILE_MANUFACTURER_SRM                = 6,
    FITFILE_MANUFACTURER_QUARQ              = 7,
    FITFILE_MANUFACTURER_IBIKE              = 8,
    FITFILE_MANUFACTURER_SARIS              = 9,
    FITFILE_MANUFACTURER_SPARK_HK           = 10,
    FITFILE_MANUFACTURER_TANITA             = 11,
    FITFILE_MANUFACTURER_ECHOWELL           = 12,
    FITFILE_MANUFACTURER_DYNASTREAM_OEM     = 13,
    FITFILE_MANUFACTURER_NAUTILUS           = 14,
    FITFILE_MANUFACTURER_DYNASTREAM         = 15,
};

enum {
    FITFILE_GARMIN_PRODUCT_HRM_BIKE        = 0,
    FITFILE_GARMIN_PRODUCT_HRM1            = 1,
    FITFILE_GARMIN_PRODUCT_AXH01           = 2,
    FITFILE_GARMIN_PRODUCT_AXB01           = 3,
    FITFILE_GARMIN_PRODUCT_AXB02           = 4,
    FITFILE_GARMIN_PRODUCT_HRM2SS          = 5,
    FITFILE_GARMIN_PRODUCT_FR405           = 717,
    FITFILE_GARMIN_PRODUCT_FR50            = 782,
    FITFILE_GARMIN_PRODUCT_FR60            = 988,
    FITFILE_GARMIN_PRODUCT_FR310XT         = 1018,
    FITFILE_GARMIN_PRODUCT_EDGE500         = 1036,
    FITFILE_GARMIN_PRODUCT_SDM4            = 10007,
    FITFILE_GARMIN_PRODUCT_TRAINING_CENTER = 20119,
    FITFILE_GARMIN_PRODUCT_CONNECT         = 65534,
};

enum {
    FITFILE_EVENT_TIMER,
    FITFILE_EVENT_WORKOUT,
    FITFILE_EVENT_WORKOUT_STEP,
    FITFILE_EVENT_POWER_DOWN,
    FITFILE_EVENT_POWER_UP,
    FITFILE_EVENT_OFF_COURSE,
    FITFILE_EVENT_SESSION,
    FITFILE_EVENT_LAP,
    FITFILE_EVENT_COURSE_POINT,
    FITFILE_EVENT_BATTERY,
    FITFILE_EVENT_VIRTUAL_PARTNET_PACE,
    FITFILE_EVENT_HR_HIGH_ALERT,
    FITFILE_EVENT_HR_LOW_ALERT,
    FITFILE_EVENT_SPEED_HIGH_ALERT,
    FITFILE_EVENT_SPEED_LOW_ALERT,
    FITFILE_EVENT_CAD_HIGH_ALERT,
    FITFILE_EVENT_CAD_LOW_ALERT,
    FITFILE_EVENT_POWER_HIGH_ALERT,
    FITFILE_EVENT_POWER_LOW_ALERT,
    FITFILE_EVENT_RECOVERY_HR,
    FITFILE_EVENT_BATTERY_LOW,
    FITFILE_EVENT_TIME_DURATION_ALERT,
    FITFILE_EVENT_DISTANCE_DURATION_ALERT,
    FITFILE_EVENT_CALORIE_DURATION_ALERT,
    FITFILE_EVENT_ACTIVITY,
    FITFILE_EVENT_FITNESS_EQUIPMENT,
};

enum {
    FITFILE_EVENT_TYPE_START,
    FITFILE_EVENT_TYPE_STOP,
    FITFILE_EVENT_TYPE_CONSECUTIVE_DEPRECIATED,
    FITFILE_EVENT_TYPE_MARKER,
    FITFILE_EVENT_TYPE_STOP_ALL,
    FITFILE_EVENT_TYPE_BEGIN_DEPRECIATED,
    FITFILE_EVENT_TYPE_END_DEPRECIATED,
    FITFILE_EVENT_TYPE_END_ALL_DEPRECIATED,
    FITFILE_EVENT_TYPE_STOP_DISABLE,
    FITFILE_EVENT_TYPE_STOP_DISABLE_ALL,
};

enum {
    FITFILE_TIMER_TRIGGER_MANUAL,
    FITFILE_TIMER_TRIGGER_AUTO,
    FITFILE_TIMER_TRIGGER_FITNESS_EQUIPMENT,
};

enum {
    FITFILE_DEVICE_TYPE_ANTFS              = 1,
    FITFILE_DEVICE_TYPE_BIKE_POWER         = 11,
    FITFILE_DEVICE_TYPE_ENVIRONMENT_SENSOR = 12,
    FITFILE_DEVICE_TYPE_FITNESS_EQUIPMENT  = 17,
    FITFILE_DEVICE_TYPE_WEIGHT_SCALE       = 119,
    FITFILE_DEVICE_TYPE_HRM                = 120,
    FITFILE_DEVICE_TYPE_BIKE_SPEED_CADENCE = 121,
    FITFILE_DEVICE_TYPE_BIKE_CADENCE       = 122,
    FITFILE_DEVICE_TYPE_BIKE_SPEED         = 123,
    FITFILE_DEVICE_TYPE_SDM                = 124,
};

#endif /* _FIT_TYPES_H_ */
