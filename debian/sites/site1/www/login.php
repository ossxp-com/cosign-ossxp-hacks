<html>
<head>
<link rel="stylesheet" href="../common.css" type="text/css">
</head>
<body>

<?php
@require("include.inc.php");
if (isloggedin())
{
    show_summary();
}
else
{
    $service = "testservice";
    if (isset($_GET["service"]))
    {
        $service = $_GET["service"];
    }
    // redirect to: weblogin?cosign_$service&return_url;
    $url = $loginurl . "?cosign-" . $service . "&http://" . @$_SERVER["SERVER_NAME"] . @$_SERVER["SCRIPT_NAME"];
    header( "Location: $url" );
}
?>

<hr>

<?php
phpinfo();
?>

</body>
</html>

