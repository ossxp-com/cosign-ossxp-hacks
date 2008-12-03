<?php

$logouturl = "https://weblogin.foo.bar/cgi-bin/logout";
$loginurl  = "https://weblogin.foo.bar/cgi-bin/login";

function show_summary()
{
    global $loginurl, $logouturl;
    $username = $_SERVER["REMOTE_USER"];
    if ($username)
        print "<h2>Welcome ".$username . "</h2>\n";
    else
        print "<h2>Hello, Anonymous</h2>\n";

    $vars = array('REMOTE_USER',
              'AUTH_TYPE',
              'REMOTE_REALM',
              'COSIGN_FACTOR',
              'COSIGN_SERVICE',
              'HTTP_COOKIE'
              );
    print "<table>\n";
    foreach ($vars as $item)
    {
        print "<tr>";
        print "<th>$item</th>";
        print "<td>". $_SERVER[$item] . "</td>";
        print "</tr>\n";
    }
    print "</table>\n";

    print "<P>\n";
    print "| <a href=\"/\">Home - site1:on/on</a>\n";
    print "| <a href=\"/login\">login - site1:on/off</a>\n";
    print "| <a href=\"/off\">off - site1:off</a>\n";
    print "| <a href=\"/service1\">service1 - service:on/on</a>\n";
    print "| <a href=\"/service2\">service2 - service:on/off</a>\n";
    print "<br>\n";
    print "Note: &lt;pagename&gt; - &lt;cookie&gt;:&lt;CosignProtected&gt;/&lt;CosignAllowPublicAccess&gt;\n";
    print "</P>\n";

    print "<P>\n";
    print "| <a href=\"/logout\">logout</a>\n";
    print "| <a href=\"$loginurl\">$loginurl</a>\n";
    print "| <a href=\"$logouturl\">$logouturl</a>\n";
    print "</P>\n";
}

?>
