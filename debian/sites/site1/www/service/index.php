<html>
<body>
<h1>Service Page</h1>

<?php

$username = $_SERVER["REMOTE_USER"];
if ($username)
    print "Welcome ".$username;
else
    print "Hello, Anonymous";

?>
<hr>

<ul>
<li> <a href="https://weblogin.local/cosign-bin/logout">logout</a></li> <br />
	    When you are finished using all authenticated web resources,
	    be sure to logout.
<li> cookie: <?php echo $_SERVER["HTTP_COOKIE"]; ?>
</ul>

<?php
phpinfo();
?>

</body>
</html>
