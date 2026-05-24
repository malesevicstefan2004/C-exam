/**
 * @file main.cpp
 * @brief vCard generator – entry point and option parsing
 * @author Stefan Malesevic
 */
#include <cstdio>
#include <cstdlib>
#include <getopt.h>

#include <list.h>
#include <vcard.h>

static const struct option long_options[] = {
    { "help",            no_argument,       nullptr, 'h' },
    { "firstname",       required_argument, nullptr, 'f' },
    { "surname",         required_argument, nullptr, 's' },
    { "org",             required_argument, nullptr, 'o' },
    { "phone-work",      required_argument, nullptr, 'w' },
    { "phone-home",      required_argument, nullptr, 'p' },
    { "email",           required_argument, nullptr, 'm' },
    { "programmer-info", no_argument,       nullptr, 'P' },
    { nullptr,           0,                 nullptr,  0  }
};

/**
 * @brief Program entry point
 * @param argc argument count
 * @param argv argument vector
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[]) {
    VCard card;
    const char *output_file = nullptr;
    int show_programmer_info = 0;

    int opt;
    while ((opt = getopt_long(argc, argv, "hf:s:p:m:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'h': print_help(argv[0]); return EXIT_SUCCESS;
            case 'f': card.addFirstname(optarg); break;
            case 's': card.setSurname(optarg);   break;
            case 'o': card.addOrg(optarg);       break;
            case 'w': card.addPhoneWork(optarg); break;
            case 'p': card.addPhoneHome(optarg); break;
            case 'm': card.addEmail(optarg);     break;
            case 'P': show_programmer_info = 1;  break;
            default:
                fprintf(stderr, "Unknown option. Use -h or --help.\n");
                return EXIT_FAILURE;
        }
    }

    if (show_programmer_info) {
        print_programmer_info(stdout);
        return EXIT_SUCCESS;
    }

    if (optind < argc)
        output_file = argv[optind];

    if (!card.hasFirstname()) {
        fprintf(stderr, "Error: a first name is required (-f / --firstname).\n");
        return EXIT_FAILURE;
    }
    if (!card.hasSurname()) {
        fprintf(stderr, "Error: a surname is required (-s / --surname).\n");
        return EXIT_FAILURE;
    }

    VcardNode *head = card.buildList();

    if (output_file != nullptr) {
        FILE *f = fopen(output_file, "w");
        if (f == nullptr) {
            fprintf(stderr, "Error: cannot open file '%s'.\n", output_file);
            list_free(&head);
            return EXIT_FAILURE;
        }
        list_print(head, f);
        fclose(f);
    } else {
        list_print(head, stdout);
    }

    list_free(&head);
    return EXIT_SUCCESS;
}
