typedef struct {
    char                *host;
    char                *service;
    char		*siteentry;
    char		**reqfv;
    int			reqfc;
    char		*suffix;
    int			fake;
    int			public;
    char                *redirect;
    char                *posterror;
    unsigned short      port;
    int                 protect;
    int                 configured;
    int			checkip;
    struct connlist     *cl;
    SSL_CTX		*ctx;
    char		*cert;
    char		*key;
    char		*cadir;
    char		*filterdb;
    int			hashlen;
    char		*proxydb;
    char		*tkt_prefix;
    int                 http;
    int                 noappendport;
    int			proxy;
    int			expiretime;
#ifdef KRB
#ifdef GSS
    int			gss;
#endif /* GSS */
    int			krbtkt;
#endif /* KRB */
} cosign_host_config;

struct connlist {
    struct sockaddr_in  conn_sin;
    SNET                *conn_sn;
    struct connlist     *conn_next;
};

#define COSIGN_ERROR		-1
#define COSIGN_OK		0
#define COSIGN_RETRY		1
#define COSIGN_LOGGED_OUT	2

#define IPCHECK_NEVER		0
#define IPCHECK_INITIAL		1
#define IPCHECK_ALWAYS		2

int cosign_cookie_valid( cosign_host_config *, char *, struct sinfo *, char *,
	server_rec * );
int cosign_check_cookie( char *, struct sinfo *, cosign_host_config *, int,
	server_rec * );
int teardown_conn( struct connlist *, server_rec * );
