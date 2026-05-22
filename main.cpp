/**
 * @file main.cpp
 * @brief vCard generator – main entry point and option parsing
 * @author Stefan Malesevic
 */
#include <cstdio>
#include <cstdlib>
#include <getopt.h>

#include <list.h>
#include <vcard.h>

/**
 * @brief Long option definitions for getopt_long
 *
 * Options without a short form ('o', 'w', 'P') use an internal
 * character value not present in the short-options string.
 */
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
 * @param argc Argument count
 * @param argv Argument vector
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char *argv[]) {
    VcardOptions opts;
    vcard_options_init(&opts);

    int opt;
    while ((opt = getopt_long(argc, argv, "hf:s:p:m:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;

            case 'f':
                if (opts.firstname_count < MAX_MULTI_VALUES) {
                    opts.firstnames[opts.firstname_count++] = optarg;
                }
                break;

            case 's':
                opts.surname = optarg;
                break;

            case 'o':
                if (opts.org_count < MAX_MULTI_VALUES) {
                    opts.orgs[opts.org_count++] = optarg;
                }
                break;

            case 'w':
                if (opts.phone_work_count < MAX_MULTI_VALUES) {
                    opts.phone_work[opts.phone_work_count++] = optarg;
                }
                break;

            case 'p':
                if (opts.phone_home_count < MAX_MULTI_VALUES) {
                    opts.phone_home[opts.phone_home_count++] = optarg;
                }
                break;

            case 'm':
                if (opts.email_count < MAX_MULTI_VALUES) {
                    opts.emails[opts.email_count++] = optarg;
                }
                break;

            case 'P':
                opts.show_programmer_info = 1;
                break;

            default:
                fprintf(stderr, "Unknown option. Use -h or --help for usage information.\n");
                return EXIT_FAILURE;
        }
    }

    if (opts.show_programmer_info) {
        print_programmer_info(stdout);
        return EXIT_SUCCESS;
    }

    /* first non-option argument is the output filename */
    if (optind < argc) {
        opts.output_file = argv[optind];
    }

    /* firstname and surname are both required */
    int error = 0;
    if (opts.firstname_count == 0) {
        fprintf(stderr, "Error: a first name is required (-f / --firstname).\n");
        error = 1;
    }
    if (opts.surname == nullptr) {
        fprintf(stderr, "Error: a surname is required (-s / --surname).\n");
        error = 1;
    }
    if (error) {
        return EXIT_FAILURE;
    }

    VcardNode *head = build_vcard(&opts);

    if (opts.output_file != nullptr) {
        FILE *f = fopen(opts.output_file, "w");
        if (f == nullptr) {
            fprintf(stderr, "Error: cannot open output file '%s'.\n", opts.output_file);
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
