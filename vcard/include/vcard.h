/**
 * @file vcard.h
 * @brief vCard construction, option handling and output functions
 * @author Stefan Malesevic
 */
#ifndef _VCARD_H_
#define _VCARD_H_

#include <cstdio>

#include <list.h>

/** @brief Maximum number of values allowed per multi-value option */
#define MAX_MULTI_VALUES 32

/**
 * @brief All command-line options parsed from argv
 *
 * Pointer members point directly into argv (valid for the
 * lifetime of main); no heap allocation is needed here.
 */
struct VcardOptions {
    const char *surname;                      /**< Last name (-s / --surname) */
    const char *firstnames[MAX_MULTI_VALUES]; /**< First name(s) (-f / --firstname) */
    int         firstname_count;              /**< Number of first names provided */
    const char *orgs[MAX_MULTI_VALUES];       /**< Organization(s) (--org) */
    int         org_count;                    /**< Number of organizations provided */
    const char *phone_work[MAX_MULTI_VALUES]; /**< Work phone(s) (--phone-work) */
    int         phone_work_count;             /**< Number of work phones provided */
    const char *phone_home[MAX_MULTI_VALUES]; /**< Home phone(s) (-p / --phone-home) */
    int         phone_home_count;             /**< Number of home phones provided */
    const char *emails[MAX_MULTI_VALUES];     /**< Email address(es) (-m / --email) */
    int         email_count;                  /**< Number of email addresses provided */
    const char *output_file;                  /**< Output filename, nullptr means stdout */
    int         show_help;                    /**< Non-zero if -h / --help was given */
    int         show_programmer_info;         /**< Non-zero if --programmer-info was given */
};

/**
 * @brief Set all fields of VcardOptions to safe initial values (zero / nullptr)
 * @param opts Pointer to the options struct to initialise
 */
void vcard_options_init(VcardOptions *opts);

/**
 * @brief Build a sorted linked list of vCard lines from parsed options
 *
 * The caller is responsible for freeing the returned list with list_free().
 *
 * @param opts Pointer to fully parsed options
 * @return Head pointer of the newly created sorted list
 */
VcardNode *build_vcard(const VcardOptions *opts);

/**
 * @brief Print the programmer identification vCard to a stream
 * @param out Target FILE stream (usually stdout)
 */
void print_programmer_info(FILE *out);

/**
 * @brief Print usage and option descriptions to stdout
 * @param prog_name Name of the executable (argv[0])
 */
void print_help(const char *prog_name);

#endif /* _VCARD_H_ */
