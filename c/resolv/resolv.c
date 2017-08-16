/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#if 0
/*
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

struct __res_state;
typedef struct __res_state *res_state;

int res_ninit(res_state statep);

int res_nquery(res_state statep,
          const char *dname, int class, int type,
          unsigned char *answer, int anslen);

int res_nsearch(res_state statep,
          const char *dname, int class, int type,
          unsigned char *answer, int anslen);

int res_nquerydomain(res_state statep,
          const char *name, const char *domain,
          int class, int type, unsigned char *answer,
          int anslen);

int res_nmkquery(res_state statep,
          int op, const char *dname, int class,
          int type, const unsigned char *data, int datalen,
          const unsigned char *newrr,
          unsigned char *buf, int buflen);

int res_nsend(res_state statep,
          const unsigned char *msg, int msglen,
          unsigned char *answer, int anslen);

int dn_comp(const char *exp_dn, unsigned char *comp_dn,
          int length, unsigned char **dnptrs,
          unsigned char **lastdnptr);

int dn_expand(const unsigned char *msg,
          const unsigned char *eomorig,
          const unsigned char *comp_dn, char *exp_dn,
          int length);

DESCRIPTION
       Note: This page is incomplete (various resolver functions provided by glibc are not described) and likely out of date.

       The functions described below make queries to and interpret the responses from Internet domain name servers.

       The  API  consists  of  a  set  of  more modern, reentrant functions and an older set of nonreentrant functions that have been superseded.  The traditional resolver interfaces such as res_init() and
       res_query() use some static (global) state stored in the _res structure, rendering these functions non-thread-safe.  BIND 8.2 introduced a set of new interfaces res_ninit(), res_nquery(), and so on,
       which take a res_state as their first argument, so you can use a per-thread resolver state.

       The  res_ninit()  and  res_init()  functions  read  the configuration files (see resolv.conf(5)) to get the default domain name and name server address(es).  If no server is given, the local host is
       tried.  If no domain is given, that associated with the local host is used.  It can be overridden with the environment variable LOCALDOMAIN.  res_ninit() or res_init() is normally  executed  by  the
       first call to one of the other functions.

       The  res_nquery()  and res_query() functions query the name server for the fully qualified domain name name of specified type and class.  The reply is left in the buffer answer of length anslen sup‐
       plied by the caller.

       The res_nsearch() and res_search() functions make a query and waits for the response like res_nquery() and res_query(), but in addition they implement the default  and  search  rules  controlled  by
       RES_DEFNAMES and RES_DNSRCH (see description of _res options below).

       The res_nquerydomain() and res_querydomain() functions make a query using res_nquery()/res_query() on the concatenation of name and domain.

       The following functions are lower-level routines used by res_query()/res_query().

       The  res_nmkquery()  and res_mkquery() functions construct a query message in buf of length buflen for the domain name dname.  The query type op is usually QUERY, but can be any of the types defined
       in <arpa/nameser.h>.  newrr is currently unused.

       The res_nsend() and res_send() function send a preformatted query given in msg of length msglen and returns the answer in answer which is of length anslen.  They will call res_ninit()/res_init()  if
       it has not already been called.

       The dn_comp() function compresses the domain name exp_dn and stores it in the buffer comp_dn of length length.  The compression uses an array of pointers dnptrs to previously compressed names in the
       current message.  The first pointer points to the beginning of the message and the list ends with NULL.  The limit of the array is specified by lastdnptr.  If dnptr is NULL,  domain  names  are  not
       compressed.  If lastdnptr is NULL, the list of labels is not updated.

       The  dn_expand() function expands the compressed domain name comp_dn to a full domain name, which is placed in the buffer exp_dn of size length.  The compressed name is contained in a query or reply
       message, and msg points to the beginning of the message.

       The resolver routines use configuration and state information contained in a __res_state structure (either passed as the statep argument, or in the global variable _res, in the  case  of  the  older
       nonreentrant functions).  The only field of this structure that is normally manipulated by the user is the options field.  This field can contain the bitwise "OR" of the following options:

       RES_INIT
              True if res_ninit() or res_init() has been called.

       RES_DEBUG
              Print debugging messages.  This option is available only if glibc was built with debugging enabled, which is not the default.

       RES_AAONLY (unimplemented; deprecated in glibc 2.25)
              Accept  authoritative  answers  only.  res_send() continues until it finds an authoritative answer or returns an error.  This option was present but unimplemented in glibc until version 2.24;
              since glibc 2.25, it is deprecated, and its usage produces a warning.

       RES_USEVC
              Use TCP connections for queries rather than UDP datagrams.

       RES_PRIMARY (unimplemented; deprecated in glibc 2.25)
              Query primary domain name server only.  This option was present but unimplemented in glibc until version 2.24; since glibc 2.25, it is deprecated, and its usage produces a warning.

       RES_IGNTC
              Ignore truncation errors.  Don't retry with TCP.

       RES_RECURSE
              Set the recursion desired bit in queries.  Recursion is carried out by the domain name server, not by res_send().  [Enabled by default].

       RES_DEFNAMES
              If set, res_search() will append the default domain name to single component names—that is, those that do not contain a dot.  [Enabled by default].

       RES_STAYOPEN
              Used with RES_USEVC to keep the TCP connection open between queries.

       RES_DNSRCH
              If set, res_search() will search for hostnames in the current domain and in parent domains.  This option is used by gethostbyname(3).  [Enabled by default].

       RES_INSECURE1
              Accept a response from a wrong server.  This can be used to detect potential security hazards, but you need to compile glibc with debugging enabled and use RES_DEBUG option (for debug purpose
              only).

       RES_INSECURE2
              Accept  a  response  which  contains  a wrong query.  This can be used to detect potential security hazards, but you need to compile glibc with debugging enabled and use RES_DEBUG option (for
              debug purpose only).

       RES_NOALIASES
              Disable usage of HOSTALIASES environment variable.

       RES_USE_INET6
              Try an AAAA query before an A query inside the gethostbyname(3) function, and map IPv4 responses in IPv6 "tunneled form" if no AAAA records are found but an A record set exists.  Since  glibc
              2.25, this option is deprecated, and its usage produces a warning; applications should use getaddrinfo(3), rather than gethostbyname(3).

       RES_ROTATE
              Causes  round-robin  selection of name servers from among those listed.  This has the effect of spreading the query load among all listed servers, rather than having all clients try the first
              listed server first every time.

       RES_NOCHECKNAME (unimplemented; deprecated in glibc 2.25)
              Disable the modern BIND checking of incoming hostnames and mail names for invalid characters such as underscore (_), non-ASCII, or control characters.  This option was present in glibc  until
              version 2.24; since glibc 2.25, it is deprecated, and its usage produces a warning.

       RES_KEEPTSIG (unimplemented; deprecated in glibc 2.25)
              Do not strip TSIG records.  This option was present but unimplemented in glibc until version 2.24; since glibc 2.25, it is deprecated, and its usage produces a warning.

       RES_BLAST  (unimplemented; deprecated in glibc 2.25)
              Send  each  query simultaneously and recursively to all servers.  This option was present but unimplemented in glibc until version 2.24; since glibc 2.25, it is deprecated, and its usage pro‐
              duces a warning.

       RES_USEBSTRING (glibc 2.3.4 to 2.24)
              Make reverse IPv6 lookups using the bit-label format described in RFC 2673; if this option is not set (which is the default), then nibble format is used.  This option  was  removed  in  glibc
              2.25, since it relied on a backward-incompatible DNS extension that was never deployed on the Internet.

       RES_NOIP6DOTINT (glibc 2.24 and earlier)
              Use  ip6.arpa  zone in IPv6 reverse lookup instead of ip6.int, which is deprecated since glibc 2.3.4.  This option is present in glibc up to and including version 2.24, where it is enabled by
              default.  In glibc 2.25, this option was removed.

       RES_USE_EDNS0 (since glibc 2.6)
              Enables support for the DNS extensions (EDNS0) described in RFC 2671.

       RES_SNGLKUP (since glibc 2.10)
              By default, glibc performs IPv4 and IPv6 lookups in parallel since version 2.9.  Some appliance DNS servers cannot handle these queries properly and make the requests time out.   This  option
              disables the behavior and makes glibc perform the IPv6 and IPv4 requests sequentially (at the cost of some slowdown of the resolving process).

       RES_SNGLKUPREOP
              When RES_SNGLKUP option is enabled, opens a new socket for the each request.

       RES_USE_DNSSEC
              Use DNSSEC with OK bit in OPT record.  This option implies RES_USE_EDNS0.

       RES_NOTLDQUERY
              Do not look up unqualified name as a top-level domain (TLD).

       RES_DEFAULT
              Default option which implies: RES_RECURSE, RES_DEFNAMES, RES_DNSRCH and RES_NOIP6DOTINT.

RETURN VALUE
       The res_ninit() and res_init() functions return 0 on success, or -1 if an error occurs.

       The  res_nquery(),  res_query(),  res_nsearch(),  res_search(),  res_nquerydomain(),  res_querydomain(), res_nmkquery(), res_mkquery(), res_nsend(), and res_send() functions return the length of the
       response, or -1 if an error occurs.

       The dn_comp() and dn_expand() functions return the length of the compressed name, or -1 if an error occurs.

FILES
       /etc/resolv.conf          resolver configuration file
       /etc/host.conf            resolver configuration file

ATTRIBUTES
       For an explanation of the terms used in this section, see attributes(7).

       ┌───────────────────────────────────┬───────────────┬────────────────┐
       │Interface                          │ Attribute     │ Value          │
       ├───────────────────────────────────┼───────────────┼────────────────┤
       │res_ninit(), res_nquery(),         │ Thread safety │ MT-Safe locale │
       │res_nsearch(), res_nquerydomain(), │               │                │
       │res_nsend()                        │               │                │
       ├───────────────────────────────────┼───────────────┼────────────────┤
       │res_nmkquery(), dn_comp(),         │ Thread safety │ MT-Safe        │
       │dn_expand()                        │               │                │
       └───────────────────────────────────┴───────────────┴────────────────┘

CONFORMING TO
       4.3BSD.

SEE ALSO
       gethostbyname(3), resolv.conf(5), resolver(5), hostname(7), named(8)

       The GNU C library source file resolv/README.
*/
#endif

#if 0


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <resolv.h>
#include <netdb.h>

#define N 4096

/* --------------------------------- */
/* CODE BEGINS                       */
/* --------------------------------- */

int main (int argc, char *argv[]) {
    u_char nsbuf[N];
    char dispbuf[N];
    ns_msg msg;
    ns_rr rr;
    int i, l;

    if (argc < 2) {
        printf ("Usage: %s <domain>\n", argv[0]);
        exit (1);
    }

/* Initialize to use the Google nameservers */
     res_init();
     _res.nscount = 2;
     _res.nsaddr_list[0].sin_family = AF_INET;
     _res.nsaddr_list[0].sin_addr.s_addr = inet_addr ("8.8.8.8");
     _res.nsaddr_list[0].sin_port = htons(53);
     _res.nsaddr_list[1].sin_family = AF_INET;
     _res.nsaddr_list[1].sin_addr.s_addr = inet_addr ("8.8.4.4");
     _res.nsaddr_list[1].sin_port = htons(53);

    printf("Domain : %s\n", argv[1]);

 /* Print the A record return or the error code */
    printf("A records : \n");
    l = res_query(argv[1], ns_c_any, ns_t_a, nsbuf, sizeof(nsbuf));
    if (l < 0) perror(argv[1]);
    ns_initparse(nsbuf, l, &msg);
    l = ns_msg_count(msg, ns_s_an);
    for (i = 0; i < l; i++)
    {
      ns_parserr(&msg, ns_s_an, i, &rr);
      ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
      printf("\t%s \n", dispbuf);
    }


    // NS RECORD
    printf("NS records : \n");
    l = res_query(argv[1], ns_c_any, ns_t_ns, nsbuf, sizeof(nsbuf));
    if (l < 0)
    {
      perror(argv[1]);
    }
    ns_initparse(nsbuf, l, &msg);
    l = ns_msg_count(msg, ns_s_an);
    for (i = 0; i < l; i++)
    {
      ns_parserr(&msg, ns_s_an, 0, &rr);
      ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
      printf("\t%s \n", dispbuf);
    }
    // ---------
    return 0;
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include "hexdump.h"

static int DEBUG = 0;
static int VERBOSE = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stdout, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)
#define hexdebug(addr, buf, size) if (DEBUG) fhexdump(stderr, addr, buf, size)

struct resolv_type {
	const char *name;
	int type;
};

#define resolv_type(t) #t, ns_t_##t
struct resolv_type RESOLV_TYPES[] = {
	{ resolv_type(invalid)    },
	{ resolv_type(a)          },
	{ resolv_type(ns)         },
	{ resolv_type(md)         },
	{ resolv_type(mf)         },
	{ resolv_type(cname)      },
	{ resolv_type(soa)        },
	{ resolv_type(mb)         },
	{ resolv_type(mg)         },
	{ resolv_type(mr)         },
	{ resolv_type(null)       },
	{ resolv_type(wks)        },
	{ resolv_type(ptr)        },
	{ resolv_type(hinfo)      },
	{ resolv_type(minfo)      },
	{ resolv_type(mx)         },
	{ resolv_type(txt)        },
	{ resolv_type(rp)         },
	{ resolv_type(afsdb)      },
	{ resolv_type(x25)        },
	{ resolv_type(isdn)       },
	{ resolv_type(rt)         },
	{ resolv_type(nsap)       },
	{ resolv_type(nsap_ptr)   },
	{ resolv_type(sig)        },
	{ resolv_type(key)        },
	{ resolv_type(px)         },
	{ resolv_type(gpos)       },
	{ resolv_type(aaaa)       },
	{ resolv_type(loc)        },
	{ resolv_type(nxt)        },
	{ resolv_type(eid)        },
	{ resolv_type(nimloc)     },
	{ resolv_type(srv)        },
	{ resolv_type(atma)       },
	{ resolv_type(naptr)      },
	{ resolv_type(kx)         },
	{ resolv_type(cert)       },
	{ resolv_type(a6)         },
	{ resolv_type(dname)      },
	{ resolv_type(sink)       },
	{ resolv_type(opt)        },
	{ resolv_type(apl)        },
	{ resolv_type(ds)         },
	{ resolv_type(sshfp)      },
	{ resolv_type(ipseckey)   },
	{ resolv_type(rrsig)      },
	{ resolv_type(nsec)       },
	{ resolv_type(dnskey)     },
	{ resolv_type(dhcid)      },
	{ resolv_type(nsec3)      },
	{ resolv_type(nsec3param) },
	{ resolv_type(tlsa)       },
	{ resolv_type(smimea)     },
	{ resolv_type(hip)        },
	{ resolv_type(ninfo)      },
	{ resolv_type(rkey)       },
	{ resolv_type(talink)     },
	{ resolv_type(cds)        },
	{ resolv_type(cdnskey)    },
	{ resolv_type(openpgpkey) },
	{ resolv_type(csync)      },
	{ resolv_type(spf)        },
	{ resolv_type(uinfo)      },
	{ resolv_type(uid)        } ,
	{ resolv_type(gid)        },
	{ resolv_type(unspec)     },
	{ resolv_type(nid)        },
	{ resolv_type(l32)        },
	{ resolv_type(l64)        },
	{ resolv_type(lp)         },
	{ resolv_type(eui48)      },
	{ resolv_type(eui64)      },
	{ resolv_type(tkey)       },
	{ resolv_type(tsig)       },
	{ resolv_type(ixfr)       },
	{ resolv_type(axfr)       },
	{ resolv_type(mailb)      },
	{ resolv_type(maila)      },
	{ resolv_type(any)        },
	{ resolv_type(uri)        },
	{ resolv_type(caa)        },
	{ resolv_type(avc)        },
	{ resolv_type(ta)         },
	{ resolv_type(dlv)        },
	{ resolv_type(max)        },
	{ NULL, 0                 }
};

struct resolv_class {
	const char *name;
	int class;
};

#define resolv_class(t) #t, ns_c_##t
struct resolv_class RESOLV_CLASSES[] = {
	{ resolv_class(invalid) },
	{ resolv_class(in)      },
	{ resolv_class(2)       },
	{ resolv_class(chaos)   },
	{ resolv_class(hs)      },
	{ resolv_class(none)    },
	{ resolv_class(any)     },
	{ resolv_class(max)     },
	{ NULL, 0               }
};

int strtotype(const char *nptr, char **endptr, int base)
{
	struct resolv_type *t;
	long l;

	for (t = RESOLV_TYPES; t->name; t++)
		if (strcmp(t->name, nptr) == 0)
			return t->type;

	l = strtol(nptr, endptr, base);
	if ((endptr && *endptr) || (errno == ERANGE && l == LONG_MAX))
		return -1;

	for (t = RESOLV_TYPES; t->name; t++)
		if (t->type == l)
			return t->type;

	return -1;
}

const char *typetostr(int type)
{
	struct resolv_type *t;
	for (t = RESOLV_TYPES; t->name; t++) {
		if (t->type == type)
			return t->name;
	}

	return NULL;
}

int strtoclass(const char *nptr, char **endptr, int base)
{
	struct resolv_class *c;
	long l;

	for (c = RESOLV_CLASSES; c->name; c++)
		if (strcmp(c->name, nptr) == 0)
			return c->class;
printf("nptr:     %s\n", nptr);
	l = strtol(nptr, endptr, base);
	if ((endptr && *endptr) || (errno == ERANGE && l == LONG_MAX))
		return -1;

	for (c = RESOLV_CLASSES; c->name; c++)
		if (c->class == l)
			return c->class;

	return -1;
}

const char *classtostr(int class)
{
	struct resolv_class *c;
	for (c = RESOLV_CLASSES; c->name; c++) {
		if (c->class == class)
			return c->name;
	}

	return NULL;
}

struct options_t {
	struct __res_state state;
	int class;
	int type;
};

static inline const char *applet(const char *arg0)
{
	char *s = strrchr(arg0, '/');
	if (!s)
		return arg0;

	return s+1;
}

void usage(FILE * f, char * const arg0)
{
	struct resolv_class *c;
	struct resolv_type *t;
	fprintf(f, "Usage: %s [OPTIONS] ENDPOINT TOPIC MESSAGE\n\n"
		   "Options:\n"
		   " -c or --class CLASS Set class [default=C_IN].\n"
		   " -t or --type TYPE   Set type [default=T_ANY].\n"
		   " -v or --verbose     Turn on verbose messages.\n"
		   " -D or --debug       Turn on debug messages.\n"
		   " -V or --version     Display the version.\n"
		   " -h or --help        Display this message.\n"
		   "", applet(arg0));

	fprintf(f, "Class:\n");
	for (c = RESOLV_CLASSES; c->name; c++)
		fprintf(f, " %s", c->name);
	fprintf(f, "\n");

	fprintf(f, "Type:\n");
	for (t = RESOLV_TYPES; t->name; t++)
		fprintf(f, " %s", t->name);
	fprintf(f, "\n");
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "class",   required_argument, NULL, 'c' },
		{ "type",    required_argument, NULL, 't' },
		{ "verbose", no_argument,       NULL, 'v' },
		{ "debug",   no_argument,       NULL, 'D' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opterr = 1;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "c:t:vDVh", long_options, &index);
		if (c == -1)
			break;

		switch (c) {
		case 'c': {
			char *e;
			int c = strtoclass(optarg, &e, 0);
			if (c == -1) {
				fprintf(stderr, "Error: "
					"Invalid timeout argument \"%s\"!\n",
					*e ? e : optarg);
				exit(EXIT_FAILURE);
			}
			opts->class = c;
			break;
		}

		case 't': {
			char *e;
			int t = strtotype(optarg, &e, 0);
			if (t == -1) {
				fprintf(stderr, "Error: "
					"Invalid type argument \"%s\"!\n",
					*e ? e : optarg);
				exit(EXIT_FAILURE);
			}
			opts->type = t;
			break;
		}

		case 'v':
			VERBOSE++;
			break;

		case 'D':
			DEBUG++;
			break;

		case 'V':
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
		}
	}

	return optind;
}

int main(int argc, char * const argv[])
{
	int i, len, ret = EXIT_SUCCESS;
	struct options_t options = {
		.class   = T_ANY,
		.type    = C_IN,
	};
	res_state state = &options.state;
	unsigned char answer[BUFSIZ];

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	}

	debug("class: %i, %s\n", options.class, classtostr(options.class));
	debug("type:  %i, %s\n", options.type, typetostr(options.type));

	for (i = argi; i < argc; i++) {
		if (res_ninit(state) == -1) {
			fprintf(stderr, "%s: res_init: %s\n", argv[i], strerror(state->res_h_errno));
			continue;
		}

#if 0
		state->nscount = 2;
		state->nsaddr_list[0].sin_family = AF_INET;
		state->nsaddr_list[0].sin_addr.s_addr = inet_addr ("8.8.8.8");
		state->nsaddr_list[0].sin_port = htons(53);
		state->nsaddr_list[1].sin_family = AF_INET;
		state->nsaddr_list[1].sin_addr.s_addr = inet_addr ("8.8.4.4");
		state->nsaddr_list[1].sin_port = htons(53);
#endif

		len = res_nquery(state, argv[i], options.type, options.class,
				 answer, sizeof(answer));
		if (len == -1) {
			fprintf(stderr, "%s: res_nquery: %s\n", argv[i],
					strerror(state->res_h_errno));
			ret = EXIT_FAILURE;
			goto close;
		}

		hexdebug(0, answer, len);
		close:
		res_nclose(state);
	}

#if RES_STATE_MALLOC
	free(state);
	state = NULL;
#endif

	return ret;
}
