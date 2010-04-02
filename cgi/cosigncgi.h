struct connlist {
    struct sockaddr_in  conn_sin;
    SNET                *conn_sn;
    struct connlist     *conn_next;
};

struct subparams {
    char	*sp_ref;
    char	*sp_service;
    char	*sp_factor;
    int		sp_reauth;
    int		sp_ipchanged;
};

//int	mkcookie( int, char * );
struct connlist * connlist_setup( char *, unsigned short );

#define COSIGN_ERROR            -1
#define COSIGN_OK               0
#define COSIGN_RETRY            1
#define COSIGN_LOGGED_OUT       2

#define COSIGN_CGI_OK                 0
#define COSIGN_CGI_ERROR              1
#define COSIGN_CGI_PASSWORD_EXPIRED   2 

#define LOGIN_ERROR_HTML        "login_error.html"
#define EXPIRED_ERROR_HTML      "expired_error.html"
#define ERROR_HTML     		"error.html"
#define LOGIN_HTML      	"login.html"
#define REAUTH_HTML     	"reauth.html"
#define REDIRECT_HTML		"redirect.html"
#define VERIFY_LOGOUT		"verify-logout.html"

#define	SL_DEFAULT_ERROR_MESSAGE    \
	"<p>Web Login is unavailable due to an internal error. We apologize" \
	" for any inconvenience this may cause and are working to restore"   \
	" service as soon as possible. Please try again later.</p>"

//static html also treat as template. ( Jiang Xin <jiangxin@ossxp.com>)
#define LOOPING_HTML          "looping.html"
#define POST_ERROR_HTML       "post_error.html"
#define VALIDATION_ERROR_HTML "validation_error.html"
