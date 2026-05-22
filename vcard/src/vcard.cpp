/**
 * @file vcard.cpp
 * @brief vCard construction, output and help functions
 * @author Stefan Malesevic
 */
#include <cstdio>
#include <cstring>
#include <ctime>

#include <vcard.h>
#include <list.h>

void vcard_options_init(VcardOptions *opts) {
    opts->surname              = nullptr;
    opts->firstname_count      = 0;
    opts->org_count            = 0;
    opts->phone_work_count     = 0;
    opts->phone_home_count     = 0;
    opts->email_count          = 0;
    opts->output_file          = nullptr;
    opts->show_help            = 0;
    opts->show_programmer_info = 0;
}

/**
 * @brief Writes current UTC time as a REV: line into buf
 * @param buf  Output buffer
 * @param size Buffer size in bytes
 */
static void build_rev_line(char *buf, const size_t size) {
    const time_t    now = time(nullptr);
    const struct tm *t  = gmtime(&now);
    strftime(buf, size, "REV:%Y%m%dT%H%M%SZ", t);
}

VcardNode *build_vcard(const VcardOptions *opts) {
    VcardNode *head = nullptr;
    char        buf[1024];

    list_insert(&head, ORDER_HEADER, "BEGIN:VCARD");
    list_insert(&head, ORDER_HEADER, "VERSION:3.0");

    /* N: Surname;FirstName;AdditionalNames;; */
    snprintf(buf, sizeof(buf), "N:%s;%s;", opts->surname, opts->firstnames[0]);
    for (int i = 1; i < opts->firstname_count; i++) {
        if (i > 1) {
            strncat(buf, ",", sizeof(buf) - strlen(buf) - 1);
        }
        strncat(buf, opts->firstnames[i], sizeof(buf) - strlen(buf) - 1);
    }
    strncat(buf, ";;", sizeof(buf) - strlen(buf) - 1);
    list_insert(&head, ORDER_NAME, buf);

    /* FN: all names followed by surname */
    snprintf(buf, sizeof(buf), "FN:%s", opts->firstnames[0]);
    for (int i = 1; i < opts->firstname_count; i++) {
        strncat(buf, " ", sizeof(buf) - strlen(buf) - 1);
        strncat(buf, opts->firstnames[i], sizeof(buf) - strlen(buf) - 1);
    }
    strncat(buf, " ", sizeof(buf) - strlen(buf) - 1);
    strncat(buf, opts->surname, sizeof(buf) - strlen(buf) - 1);
    list_insert(&head, ORDER_NAME, buf);

    /* ORG is optional, multiple values are semicolon-separated */
    if (opts->org_count > 0) {
        snprintf(buf, sizeof(buf), "ORG:%s", opts->orgs[0]);
        for (int i = 1; i < opts->org_count; i++) {
            strncat(buf, ";", sizeof(buf) - strlen(buf) - 1);
            strncat(buf, opts->orgs[i], sizeof(buf) - strlen(buf) - 1);
        }
        list_insert(&head, ORDER_ORG, buf);
    }

    for (int i = 0; i < opts->phone_work_count; i++) {
        snprintf(buf, sizeof(buf), "TEL;TYPE=WORK,VOICE:%s", opts->phone_work[i]);
        list_insert(&head, ORDER_TEL, buf);
    }

    for (int i = 0; i < opts->phone_home_count; i++) {
        snprintf(buf, sizeof(buf), "TEL;TYPE=HOME,VOICE:%s", opts->phone_home[i]);
        list_insert(&head, ORDER_TEL, buf);
    }

    for (int i = 0; i < opts->email_count; i++) {
        snprintf(buf, sizeof(buf), "EMAIL;TYPE=PREF,INTERNET:%s", opts->emails[i]);
        list_insert(&head, ORDER_EMAIL, buf);
    }

    build_rev_line(buf, sizeof(buf));
    list_insert(&head, ORDER_FOOTER, buf);
    list_insert(&head, ORDER_FOOTER, "END:VCARD");

    return head;
}

void print_programmer_info(FILE *out) {
    char rev_buf[64];
    build_rev_line(rev_buf, sizeof(rev_buf));

    fprintf(out, "BEGIN:VCARD\n");
    fprintf(out, "VERSION:3.0\n");
    fprintf(out, "N:Malesevic;Stefan;;;\n");
    fprintf(out, "FN:Stefan Malesevic\n");
    fprintf(out, "ORG:TIA\n");
    fprintf(out, "EMAIL;TYPE=PREF,INTERNET:malesevicstefan2004@gmail.com\n");
    fprintf(out, "%s\n", rev_buf);
    fprintf(out, "END:VCARD\n");
}

void print_help(const char *prog_name) {
    printf("Usage: %s [OPTIONS] [OUTPUTFILE]\n", prog_name);
    printf("\n");
    printf("Create a vCard 3.0 contact file from command line options.\n");
    printf("\n");
    printf("Options:\n");
    printf("  -h, --help             Show this help text and exit\n");
    printf("  -f, --firstname NAME   First name (repeat for additional names;\n");
    printf("                         first occurrence is the primary given name)\n");
    printf("  -s, --surname  NAME    Last name (required)\n");
    printf("      --org      NAME    Organization (repeat to add multiple,\n");
    printf("                         all joined with ';' in one ORG field)\n");
    printf("      --phone-work NUM   Work phone number (repeat for multiple)\n");
    printf("  -p, --phone-home NUM   Home phone number (repeat for multiple)\n");
    printf("  -m, --email    ADDR    E-mail address (repeat for multiple)\n");
    printf("      --programmer-info  Print the author's vCard and exit\n");
    printf("\n");
    printf("Arguments:\n");
    printf("  OUTPUTFILE             Optional output file path.\n");
    printf("                         When omitted the vCard is written to stdout.\n");
    printf("\n");
    printf("Both -f/--firstname and -s/--surname are mandatory.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s -f Thomas -s Staudacher\n", prog_name);
    printf("  %s -f Erika -f Elisabeth -f Maria -s Mustermann --org DHBW\n", prog_name);
    printf("  %s -f Thomas -s Staudacher -m t@example.com contact.vcf\n", prog_name);
}
