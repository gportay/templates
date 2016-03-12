/*
 * Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
 *
 * Licensed under the MIT license.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <iostream>

#include "addressbook.pb.h"

const std::string Person_PhoneType_STRINGS[] = {
	"mobile",
	"home",
	"work",
	"unknown",
};

static int DEBUG = 0;
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

struct options_t {
	int decode;
	const char *email;
	const char *phone;
};

inline const char *applet(const char *arg0)
{
	const char *s = strrchr(arg0, '/');

	if (!s)
		return arg0;

	return ++s;
}

void usage(FILE * f, char * const arg0)
{
	fprintf(f, "Usage: %s [OPTIONS] NAME ID\n"
		   "       %s --decode\n\n"
		   "Options:\n"
		   " -d or --decode        Decode from stdin.\n"
		   " --email STRING        Set e-mail.\n"
		   " --phone STRING        Set phone.\n"
		   " -D or --debug         Turn on debug messages.\n"
		   " -h or --help          Display this message.\n"
		   " -v or --version       Display the version.\n"
		   "", applet(arg0), applet(arg0));
}

inline int strtoid(char *str)
{
	char *e;
	long l = strtol(str, &e, 0);
	if (*e) {
		return -1;
	}

	return l;
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "decode",  no_argument,       NULL, 'd' },
		{ "email",   required_argument, NULL, 'e' },
		{ "phone",   required_argument, NULL, 'p' },
		{ "debug",   no_argument,       NULL, 'D' },
		{ "version", no_argument,       NULL, 'v' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	for (;;) {
		int option_index;
		int c = getopt_long(argc, argv, "de:p::Dvh", long_options, &option_index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 'd':
			opts->decode = 1;
			break;

		case 'e':
			opts->email = optarg;
			break;

		case 'p':
			opts->phone = optarg;
			break;

		case 'D':
			DEBUG = 1;
			break;

		case 'v':
			printf("%s\n", VERSION);
			exit(EXIT_SUCCESS);
			break;

		case 'h':
			usage(stdout, argv[0]);
			exit(EXIT_SUCCESS);
			break;

		case '?':
			exit(EXIT_FAILURE);
			break;

		default:
			fprintf(stderr, "Error: Illegal option code 0x%x!\n", c);
			exit(EXIT_FAILURE);
		}
	}

	return optind;
}

int main(int argc, char * const argv[])
{
	int ret = EXIT_FAILURE;
	static struct options_t options;
	tutorial::Person p;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		exit(EXIT_FAILURE);
	}

	if (!options.decode) {
		int id;

		if (argc - argi < 2) {
			usage(stdout, argv[0]);
			fprintf(stderr, "Error: Too few arguments!\n");
			exit(EXIT_FAILURE);
		}
		else if (argc - argi > 2) {
			usage(stdout, argv[0]);
			fprintf(stderr, "Error: Too many arguments!\n");
			exit(EXIT_FAILURE);
		}

		debug(">>> Person {\n");

		p.set_name(argv[argi]);
		debug(">>>   .name: \"%s\",\n", argv[argi]);
		argi++;

		id = strtoid(argv[argi]); 
		if (id < 0) {
			fprintf(stderr, "Error: Invalid id argument \"%s\"!\n", argv[argi]);
			exit(EXIT_FAILURE);
		}
		p.set_id(id);
		debug(">>>   .id: %i,\n", id);
		argi++;

		if (options.email) {
			p.set_email(options.email);
			debug(">>>   .email: \"%s\",\n", options.email);
		}

		if (options.phone) {
			const char *s = options.phone;
			int i = 0;

			while (*s) {
				tutorial::Person_PhoneNumber *pn = p.add_phone();
				std::string type, num(s);
				size_t t = num.find_first_of(',');

				if (t != std::string::npos) {
					num = std::string(s, t++);
					s += t;
				} else {
					s += num.length();
				}

				t = num.find_first_of(':');
				if (t != std::string::npos) {
					type = num.substr(0, t++);
					num = num.substr(t);

					if (!type.compare("mobile")) {
						pn->set_type(tutorial::Person_PhoneType_MOBILE);
					} else if (!type.compare("home")) {
						pn->set_type(tutorial::Person_PhoneType_HOME);
					} else if (!type.compare("work")) {
						pn->set_type(tutorial::Person_PhoneType_WORK);
					}
				}
				pn->set_number(num);
				debug(">>>   .phone[%i]: (%s) \"%s\",\n", i++, type.c_str(), num.c_str());
			}
		}

		debug(">>> }\n");

		p.SerializeToOstream(&std::cout);
	} else {
		int i;

		p.ParseFromIstream(&std::cin);

		debug("<<< Person {\n");

		if (p.has_name())
			debug("<<<   .name: \"%s\",\n", p.name().c_str());

		if (p.has_id())
			debug("<<<   .id: %i,\n", p.id());

		if (p.has_email())
			debug("<<<   .email: \"%s\",\n", p.email().c_str());

		for (i = 0; i < p.phone_size(); i++) {
			tutorial::Person_PhoneNumber pn = p.phone(i);
			debug("<<<   .phone[%i]: (%s) \"%s\",\n", i, Person_PhoneType_STRINGS[pn.type()].c_str(), pn.number().c_str());
		}

		debug("<<< }\n");
	}
exit:
	return ret;
}
