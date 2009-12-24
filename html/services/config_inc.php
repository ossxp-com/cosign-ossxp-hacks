<?php

$url_gosa = "https://weblogin.foo.bar/members/?login";
$url_logout = "https://weblogin.foo.bar/cgi-bin/logout";

#############################
# LDAP Settings
#############################

$g_ldap_server		= 'ldap://localhost/';
$g_ldap_port		= '389';
$g_ldap_root_dn		= 'dc=foo,dc=bar';
$g_ldap_organizatio	= '(ossxpConfirmed=TRUE)';
$g_ldap_uid_field	= 'uid';
$g_ldap_bind_dn	= 'cn=ldapadmin,dc=foo,dc=bar';
$g_ldap_bind_passwd	= 'guess';

?>
