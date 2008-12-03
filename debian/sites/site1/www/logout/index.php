<?php

require("../include.inc.php");

/* 
 * cleanup cookies.
 * make any local additions here (e.g. expiring local sessions, etc.),
 * but it's important that there be no output on this page.
 */
#setcookie( "cosign-site1", "null", time()-1, '/', "", 1 );
#setcookie( "cosign-service", "null", time()-1, '/', "", 1 );
setcookie( $_SERVER['COSIGN_SERVICE'], "null", time()-1, '/', "", 1 );

/*
 * Redirect to SSO's logout
 */
header( "Location: $logouturl" );
exit;

?>
