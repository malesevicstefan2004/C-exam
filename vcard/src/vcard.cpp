/**
 * @file vcard.cpp
 * @brief VCard class implementation
 * @author Stefan Malesevic
 */
#include <cstdio>
#include <ctime>

#include <vcard.h>
#include <list.h>

std::string VCard::join(const std::vector<std::string> &v, const std::string &sep) {
    std::string result;
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) result += sep;
        result += v[i];
    }
    return result;
}

std::string VCard::revLine() {
    char buf[64];
    const time_t now = time(nullptr);
    const struct tm *t = gmtime(&now);
    strftime(buf, sizeof(buf), "REV:%Y%m%dT%H%M%SZ", t);
    return std::string(buf);
}

VcardNode *VCard::buildList() const {
    VcardNode *head = nullptr;

    list_insert(&head, ORDER_HEADER, "BEGIN:VCARD");
    list_insert(&head, ORDER_HEADER, "VERSION:3.0");

    /* build N field – additional names are comma separated */
    std::vector<std::string> extra(m_firstnames.begin() + 1, m_firstnames.end());
    std::string n = "N:" + m_surname + ";" + m_firstnames[0] + ";" + join(extra, ",") + ";;";
    list_insert(&head, ORDER_NAME, n.c_str());

    /* build FN field – all names with spaces, surname at end */
    std::string fn = "FN:" + join(m_firstnames, " ") + " " + m_surname;
    list_insert(&head, ORDER_NAME, fn.c_str());

    if (!m_orgs.empty()) {
        std::string org = "ORG:" + join(m_orgs, ";");
        list_insert(&head, ORDER_ORG, org.c_str());
    }

    for (const std::string &p : m_phones_work)
        list_insert(&head, ORDER_TEL, ("TEL;TYPE=WORK,VOICE:" + p).c_str());

    for (const std::string &p : m_phones_home)
        list_insert(&head, ORDER_TEL, ("TEL;TYPE=HOME,VOICE:" + p).c_str());

    for (const std::string &e : m_emails)
        list_insert(&head, ORDER_EMAIL, ("EMAIL;TYPE=PREF,INTERNET:" + e).c_str());

    list_insert(&head, ORDER_FOOTER, revLine().c_str());
    list_insert(&head, ORDER_FOOTER, "END:VCARD");

    return head;
}

void print_programmer_info(FILE *out) {
    char buf[64];
    const time_t now = time(nullptr);
    const struct tm *t = gmtime(&now);
    strftime(buf, sizeof(buf), "REV:%Y%m%dT%H%M%SZ", t);

    fprintf(out, "BEGIN:VCARD\n");
    fprintf(out, "VERSION:3.0\n");
    fprintf(out, "N:Malesevic;Stefan;;;\n");
    fprintf(out, "FN:Stefan Malesevic\n");
    fprintf(out, "ORG:TIA\n");
    fprintf(out, "TEL;TYPE=MOBIL,VOICE:+4917682301950\n");
    fprintf(out, "EMAIL;TYPE=PREF,INTERNET:malesevicstefan2004@gmail.com\n");
    fprintf(out, "%s\n", buf);
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
