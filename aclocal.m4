AC_DEFUN([CHECK_SSL],
[
    AC_MSG_CHECKING(for ssl)
    ssldirs="/usr/local/openssl /usr/lib/openssl /usr/openssl \
        /usr/local/ssl /usr/lib/ssl /usr/ssl \
        /usr/pkg /usr/local /usr"
    AC_ARG_WITH(ssl,
        AC_HELP_STRING([--with-ssl=DIR], [path to ssl]),
        ssldirs="$withval")
    AC_CACHE_VAL(ac_cv_path_ssl,[
        for ssldir in $ssldirs; do
            if test -f "$ssldir/include/openssl/ssl.h"; then
                ac_cv_path_ssl=$ssldir;
                break;
            fi
            if test -f "$ssldir/include/ssl.h"; then
                ac_cv_path_ssl=$ssldir;
                break
            fi
        done
    ])
    if test ! -e "$ac_cv_path_ssl" ; then
        AC_MSG_ERROR(cannot find ssl libraries)
    fi
    case "X$host_os" in 
    Xaix*)
	ADDLIBS="-lssl -lcrypto";
	AC_SUBST(ADDLIBS)
	ADDLDFLAGS="-L$ac_cv_path_ssl/lib";
	AC_SUBST(ADDLDFLAGS)
    ;;
    esac

    CPPFLAGS="$CPPFLAGS -I$ac_cv_path_ssl/include";
    TLSDEFS=-DHAVE_LIBSSL;
    AC_SUBST(TLSDEFS)
    LIBS="$LIBS -lssl -lcrypto";
    LDFLAGS="$LDFLAGS -L$ac_cv_path_ssl/lib";
    HAVE_SSL=yes
    AC_SUBST(HAVE_SSL)
    AC_MSG_RESULT($ac_cv_path_ssl)
])

AC_DEFUN([CHECK_LIBKRB],
[
    AC_MSG_CHECKING(for krb)

    if test -d $enableval; then
	echo "using krb as '$enableval'";
	if test -f "$enableval/include/krb5.h"; then
	    ac_cv_path_krb=$enableval
	fi
    else
	krbdirs="/usr/local/kerberos /usr/lib/kerberos /usr/kerberos \
		/usr/local/krb5 /usr/lib/krb /usr/krb \
		/usr/pkg /usr/local /usr"
	AC_CACHE_VAL(ac_cv_path_krb,[
	    for krbdir in $krbdirs; do
		if test -f "$krbdir/include/krb5.h"; then
		    ac_cv_path_krb=$krbdir
		    break;
		fi
	    done
	])
    fi
    if test ! -e "$ac_cv_path_krb" ; then
        AC_MSG_ERROR(cannot find krb libraries)
    fi
    KRBCGI="cosign.cgi"
    AC_SUBST(KRBCGI)
    KRBDEFS=-DKRB;
    AC_SUBST(KRBDEFS)
    KINC="-I$ac_cv_path_krb/include";
    AC_SUBST(KINC)
    KLIBS="-lkrb5 -lk5crypto -lcom_err";
    AC_SUBST(KLIBS)
    KLDFLAGS="-L$ac_cv_path_krb/lib";
    AC_SUBST(KLDFLAGS)
    HAVE_KRB=yes
    AC_SUBST(HAVE_KRB)
    AC_MSG_RESULT(Kerberos found at $ac_cv_path_krb)
])

AC_DEFUN([CHECK_APACHE2],
[
    AC_MSG_CHECKING(for apache 2)
 
    if test -f "$enableval"; then
	echo "using apxs2 as '$enableval'"
	ac_cv_path_apxs2="$enableval"
    fi

    APXS2="$ac_cv_path_apxs2"

    if test ! -e "$ac_cv_path_apxs2" ; then
        AC_MSG_ERROR(cannot find apache 2)
    fi

    FILTERS="$FILTERS filters/apache2"
    APXS2_TARGET="`$APXS2 -q TARGET`"
    if test x_$APXS2_TARGET = x_httpd ; then
	APACHECTL2="${APXS2_SBINDIR}/apachectl"
    else
	APACHECTL2="${APXS2_SBINDIR}/${APXS2_TARGET}ctl"
    fi
    AC_SUBST(APXS2)
    AC_SUBST(APACHECTL2)
    HAVE_APACHE2=yes
    AC_SUBST(HAVE_APACHE2)
    AC_MSG_RESULT(apache 2 filter will be built)

])

AC_DEFUN([CHECK_GSS],
[
    AC_MSG_CHECKING(for gss)
    if test ! -e "$ac_cv_path_krb" ; then
        AC_MSG_ERROR(gss require krb5 libraries)
    fi
    GSSDEFS=-DGSS;
    AC_SUBST(GSSDEFS)
    GSSINC="-I$ac_cv_path_krb/include";
    AC_SUBST(GSSINC)
    GSSLIBS="-lgssapi_krb5 -lkrb5 -lk5crypto -lcom_err";
    AC_SUBST(GSSLIBS)
    GSSLDFLAGS="-L$ac_cv_path_krb/lib";
    AC_SUBST(GSSLDFLAGS)
    HAVE_GSS=yes
    AC_SUBST(HAVE_GSS)
    AC_MSG_RESULT($ac_cv_path_krb)
])

AC_DEFUN([CHECK_APACHE_1],
[

    AC_MSG_CHECKING([for apache 1.3])

    if test -f "$enableval"; then
        echo "using apxs as '$enableval'"
        ac_cv_path_apxs="$enableval"
    fi

    APXS="$ac_cv_path_apxs"

    if test ! -e "$ac_cv_path_apxs" ; then
        AC_MSG_ERROR(cannot find apache 1.3)
    fi

    HAVE_APACHE=yes
    FILTERS="$FILTERS filters/apache"
    APXS_TARGET="`$APXS -q TARGET`"
    if test x_$APXS_TARGET = x_httpd ; then
	APACHECTL="${APXS_SBINDIR}/apachectl"
    else
	APACHECTL="${APXS_SBINDIR}/${APXS_TARGET}ctl"
    fi
    AC_SUBST(APXS)
    AC_SUBST(APACHECTL)
    AC_MSG_RESULT(apache 1.3 filter will be built)

])

AC_DEFUN([CHECK_LIBMYSQL],
[
    AC_MSG_CHECKING(for mysql)
    mysqldirs="/usr /usr/local/mysql /usr/lib/mysql /usr/mysql \
            /usr/pkg /usr/local /usr"
    AC_CACHE_VAL(ac_cv_path_mysql,[
        for mysqldir in $mysqldirs; do
            if test -f "$mysqldir/include/mysql/mysql.h"; then
                ac_cv_path_mysql=$mysqldir
                break;
            fi
        done
    ])
    if test ! -e "$ac_cv_path_mysql" ; then
        AC_MSG_ERROR(cannot find mysql libraries)
    fi
    MYSQLINC="-I$ac_cv_path_mysql/include/mysql";
    AC_SUBST(MYSQLINC)
    MYSQLLIBS="-lmysqlclient";
    AC_SUBST(MYSQLLIBS)
    MYSQLLDFLAGS="-L$ac_cv_path_mysql/lib/mysql -R$ac_cv_path_mysql/lib/mysql";
    AC_SUBST(MYSQLLDFLAGS)
    SQLDEFS=-DSQL_FRIEND
    AC_SUBST(SQLDEFS)
    HAVE_MYSQL=yes
    AC_SUBST(HAVE_MYSQL)
    AC_MSG_RESULT($ac_cv_path_mysql)
])

AC_DEFUN([SET_NO_SASL],
[
    ac_configure_args="$ac_configure_args --with-sasl=no";
    AC_MSG_RESULT(Disabled SASL)
])

AC_DEFUN([SET_NO_ZLIB],
[
    ac_configure_args="$ac_configure_args --with-zlib=no";
    AC_MSG_RESULT(Disabled ZLIB)
])

AC_DEFUN([SET_ENABLE_SHARED],
[
    ac_configure_args="$ac_configure_args --enable-shared";
    AC_MSG_RESULT(Enabled Shared)
])
