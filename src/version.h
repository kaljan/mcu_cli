/**
 * @file    version.h
 * @author  Mikalai Naurotski (kaljan.nothern@gmail.com)
 * @version 0.1.4_spi
 * @date    2024-08-20
 *
 * @brief   This header contains project version information
 */
#ifndef VERSION_H
#define VERSION_H


#define VERSION_MAJOR     0
#define VERSION_MINOR     1
#define VERSION_BUILD     4
#define VERSION_NAME      "dev"

/** date */
#define DATE_DAY          20
#define DATE_MONTH        8
#define DATE_YEAR         2024

/** date/time text */
#define DATE_STR        "2024-08-20"
#define TIME_STR        "23:50:02"

#define PROJECT_NAME    "mcu_cli"

#if defined DEBUG || defined RELEASE
# if DEBUG == 1
# define BUILD_MODE "debug"
# elif RELEASE == 1
# define BUILD_MODE "release"
# else
# define BUILD_MODE "build"
# endif
#else
#define BUILD_MODE "build"
#endif

#endif /* VERSION_H */
