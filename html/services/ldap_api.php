<?php

###########################################################################
# LDAP API
###########################################################################

# force config variable from a global to avoid recursion
function config_get( $p_option, $p_default = null ) {

    if ( isset( $GLOBALS['g_' . $p_option] ) ) {
        $t_value = $GLOBALS['g_' . $p_option];
        if ( $t_value !== $GLOBALS['g_' . $p_option] ) {
            $GLOBALS['g_' . $p_option] = $t_value;
        }
        return $t_value;
    } else {
        # unless we were allowing for the option not to exist by passing
        #  a default, trigger a WARNING
        return $p_default;
    }
}

function is_blank( $p_var ) {
    $p_var = trim( $p_var );
    $str_len = strlen( $p_var );
    if ( 0 == $str_len ) {
        return true;
    }
    return false;
}

# --------------------
# Connect and bind to the LDAP directory
function ldap_connect_bind( $p_binddn = '', $p_password = '' ) {
    $t_ldap_server	= config_get( 'ldap_server' );
    $t_ldap_port	= config_get( 'ldap_port' );
    if ( is_blank( $t_ldap_port ) ) {
        $t_ldap_port = 389;
    }
    $t_ds = @ldap_connect ( $t_ldap_server, $t_ldap_port );
    if ( $t_ds > 0 ) {
        $t_protocol_version = config_get( 'ldap_protocol_version' );

        if ( $t_protocol_version > 0 ) {
            ldap_set_option( $t_ds, LDAP_OPT_PROTOCOL_VERSION, $t_protocol_version );
        }

        # If no Bind DN and Password is set, attempt to login as the configured
        #  Bind DN.
        if ( is_blank( $p_binddn ) && is_blank( $p_password ) ) {
            $p_binddn	= config_get( 'ldap_bind_dn', '' );
            $p_password	= config_get( 'ldap_bind_passwd', '' );
        }

        if ( !is_blank( $p_binddn ) && !is_blank( $p_password ) ) {
            $t_br = @ldap_bind( $t_ds, $p_binddn, $p_password );
        } else {
            # Either the Bind DN or the Password are empty, so attempt an anonymous bind.
            $t_br = @ldap_bind( $t_ds );
        }
    } 

    return $t_ds;
}

# --------------------
# Return an email address from LDAP, given a username
function ldap_brief_info( $p_username ) {
    $t_ldap_organization	= config_get( 'ldap_organization' );
    $t_ldap_root_dn	    	= config_get( 'ldap_root_dn' );

    $t_ldap_uid_field = config_get( 'ldap_uid_field', 'uid' ) ;
    $t_search_filter	= "(&$t_ldap_organization($t_ldap_uid_field=$p_username))";
    $t_search_attrs		= array( $t_ldap_uid_field, 'dn', 'givenName', 'sn', 'cn', 'mail' );
    $t_ds           	= ldap_connect_bind();

    $t_sr	= ldap_search( $t_ds, $t_ldap_root_dn, $t_search_filter, $t_search_attrs );
    $t_info	= ldap_get_entries( $t_ds, $t_sr );
    ldap_free_result( $t_sr );
    ldap_unbind( $t_ds );

    $result = array();
    $result['dn'] = $t_info[0]['dn'];
    $result['givenname'] = $t_info[0]['givenname'][0];
    $result['sn'] = $t_info[0]['sn'][0];
    $result['cn'] = $t_info[0]['cn'][0];
    $result['mail'] = $t_info[0]['mail'][0];

    return $result;
}

?>
