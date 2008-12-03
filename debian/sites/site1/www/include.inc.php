<?php

$logouturl = "https://weblogin.foo.bar/cgi-bin/logout";
$loginurl  = "https://weblogin.foo.bar/cgi-bin/login";

function show_summary()
{
    global $loginurl, $logouturl;

    $location = trim($_SERVER["REQUEST_URI"], '/');
    if (!$location)
        $location = "Home";
    print "<h1>$location page</h1>";

    $username = $_SERVER["REMOTE_USER"];
    if ($username)
        print "<h2>Welcome ".$username . "</h2>\n";
    else
        print "<h2>Not login, Anonymous</h2>\n";

    $vars = array('REMOTE_USER',
              'AUTH_TYPE',
              'REMOTE_REALM',
              'COSIGN_FACTOR',
              'COSIGN_SERVICE',
              'HTTP_COOKIE'
              );
    print "<table>\n";
    print "<tr><th>Variable</th><th>Value</th></tr>\n";
    foreach ($vars as $item)
    {
        print "<tr>";
        print "<th>$item</th>";
        $value = $_SERVER[$item];
        $value = implode('<br>',explode(';', $value));
        print "<td>". $value . "</td>";
        print "</tr>\n";
    }
    print "</table>\n";
?>

    <p>
    <table class="borderless">
      <tr>
        <th valign="middle">
          CookieName: siteX
        </th>
        <td>
            <table border='1'>
              <tr>
                <th>
                </th>
                <th>
                  CosignAllowPublicAccess: on
                </th>
                <th>
                  CosignAllowPublicAccess: off
                </th>
              </tr>
              <tr>
                <th>
                  CosignProtected: on
                </th>
                <td>
                  <a href="/">Home - site1:on/on</a>
                </td>
                <td>
                  <a href="/login">login - site1:on/off</a>
                </td>
              </tr>
              <tr>
                <th>
                  CosignProtected: off
                </th>
                <td colspan='2'>
                  <a href="/off">off - site1:off</a>
                </td>
              </tr>
            </table>
        </td>
      </tr>
      <tr>
        <th valign="middle">
          CookieName: service
        </th>
        <td valign="middle">
            <table border='1'>
              <tr>
                <th>
                </th>
                <th>
                  CosignAllowPublicAccess: on
                </th>
                <th>
                  CosignAllowPublicAccess: off
                </th>
              </tr>
              <tr>
                <th>
                  CosignProtected: on
                </th>
                <td>
                  <a href="/service1">service1 - service:on/on</a>
                </td>
                <td>
                  <a href="/service2">service2 - service:on/off</a>
                </td>
              </tr>
              <tr>
                <th>
                  CosignProtected: off
                </th>
                <td colspan='2'>
                  <a href="/off">off - site1:off</a>
                </td>
              </tr>
            </table>
        </td>
      </tr>
    </table>
    
<?php

    print "<P>\n";
    print "| <a href=\"/logout\">logout</a>\n";
    print "| <a href=\"$loginurl\">$loginurl</a>\n";
    print "| <a href=\"$logouturl\">$logouturl</a>\n";
    print "</P>\n";
}

?>
