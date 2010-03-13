<?php

require("../include.inc.php");

/* 
 * cleanup cookies.
 * make any local additions here (e.g. expiring local sessions, etc.),
 * but it's important that there be no output on this page.
 */
setcookie( "cosign-site1.test1", "null", time()-1, '/', "", 1 );
setcookie( "cosign-site1.testservice", "null", time()-1, '/', "", 1 );
setcookie( $_SERVER['COSIGN_SERVICE'], "null", time()-1, '/', "", 1 );

/*
 * Redirect to SSO's logout
 */
header( "Location: $logouturl?http://" . $_SERVER["SERVER_NAME"] );
exit;

?>
