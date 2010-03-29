class LdapConfig:
    # LDAP parameters
    ldap_uri = 'ldap://localhost'
    ldap_binddn = ''
    ldap_bindpw = ''
    ldap_base = 'dc=foo,dc=bar'
    ldap_scope = 2 # 2 - ldap.SCOPE_SUBTREE
    ldap_filter = '(ossxpConfirmed=TRUE)'
    ldap_timeout = 10 # how long we wait for the ldap server [s]
    ldap_coding = 'utf-8' # coding used for ldap queries and result values
    ldap_start_tls = False
    ldap_email_attribute = 'mail'
    ldap_login_attribute = 'uid'

    # Debug and logging
    log_level = 'critical'
    log_file  = None

class InviteConfig:
    # "effect_user_id" :  ["invite_code1", ... ],
    maps = { }

    # Debug and logging
    log_level = 'critical'
    log_file  = None

# vim: et ts=4 sw=4
