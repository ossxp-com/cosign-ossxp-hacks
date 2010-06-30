/* common/config.h.  Generated from config.h.in by configure.  */

/* openssl is required */
#define HAVE_LIBSSL 1

/* kerberos and gssapi support */
/* #undef KRB */
/* #undef GSS */

/* friend accounts */
/* #undef HAVE_MYSQL */
/* #undef SQL_FRIEND */

/* Apache 1.3.x */
/* #undef APACHE1 */

/* Apache 2.x */
#define APACHE2 1
#define HAVE_MOD_AUTHZ_HOST 1
#define HAVE_AP_REGEX_H 1

/* lighttpd */
/* #undef LIGHTTPD */
