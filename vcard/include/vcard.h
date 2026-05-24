/**
 * @file vcard.h
 * @brief VCard class declaration
 * @author Stefan Malesevic
 */
#ifndef _VCARD_H_
#define _VCARD_H_

#include <cstdio>
#include <string>
#include <vector>

#include <list.h>

/**
 * @brief Stores all vCard fields and builds the output list
 */
class VCard {
private:
    std::string              m_surname;
    std::vector<std::string> m_firstnames;
    std::vector<std::string> m_orgs;
    std::vector<std::string> m_phones_work;
    std::vector<std::string> m_phones_home;
    std::vector<std::string> m_emails;

    static std::string join(const std::vector<std::string> &v, const std::string &sep);
    static std::string revLine();

public:
    void setSurname(const std::string &s)   { m_surname = s; }
    void addFirstname(const std::string &f) { m_firstnames.push_back(f); }
    void addOrg(const std::string &o)       { m_orgs.push_back(o); }
    void addPhoneWork(const std::string &p) { m_phones_work.push_back(p); }
    void addPhoneHome(const std::string &p) { m_phones_home.push_back(p); }
    void addEmail(const std::string &e)     { m_emails.push_back(e); }

    bool hasFirstname() const { return !m_firstnames.empty(); }
    bool hasSurname()   const { return !m_surname.empty(); }

    /**
     * @brief Builds the sorted vCard linked list
     * @return head of the list, must be freed with list_free()
     */
    VcardNode *buildList() const;
};

void print_programmer_info(FILE *out);
void print_help(const char *prog_name);

#endif /* _VCARD_H_ */
