<?php

require_once("config_inc.php");
require_once("ldap_api.php");

if (array_key_exists("logout", $_REQUEST))
{
  if (@$_SERVER['COSIGN_SERVICE'] || @$_SERVER['REDIRECT_COSIGN_SERVICE'])
  {
    $cookie_name = @$_SERVER["COSIGN_SERVICE"] ? @$_SERVER["COSIGN_SERVICE"] : @$_SERVER["REDIRECT_COSIGN_SERVICE"];
    setcookie( $cookie_name, "null", time()-1, '/', "", 0 );
    setcookie( $cookie_name, "null", time()-1 );
  }
  Header( "Location: $url_logout" );
  exit(0);
}

$uid = @$_SERVER["REMOTE_USER"]? @$_SERVER["REMOTE_USER"] : @$_SERVER["REDIRECT_REMOTE_USER"];
if (!$uid)
{
  die("Not login yet.");
}

$info=ldap_brief_info($uid);
if ($info['givenname'] == $info['sn'] && $info['sn']!='')
{
  $username = $info['sn'];
}
else
{
  $username = $info['sn'] . " " .  $info['givenname'];
}
if (!$username)
  $username = $uid;

if ($info['cn'])
  $aliasname = $info['cn'];
else
  $aliasname = $username;

$mail = $info['mail'];

?>
<html><head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="Monday, 16-Apr-73 13:10:00 GMT" />
<title>欢迎, <?php echo $username; ?></title>
<script><!--
function gaia_setFocus() {
  var f = null;
  if (document.getElementById) { 
    f = document.getElementById("gaia_loginform");
  } else if (window.gaia_loginform) { 
    f = window.gaia_loginform;
  } 
  if (f) {
    if (f.login && (f.login.value == null || f.login.value == "")) {
      f.login.focus();
    } else if (f.password) {
      f.password.focus();
    } 
  }
}
--></script>
<style type="text/css">
  <!--
  body { font-family: arial,sans-serif; background-color: #fff; margin-top: 2; }
  .c { width: 4; height: 4; } 
  a:link { color: #00c; } 
  a:visited { color: #551a8b; }
  a:active { color: #f00; }
  .form-noindent { background-color: #fff; border: 1px solid #c3d9ff; }
  -->
</style>
<style type="text/css"><!--
.gaia.le.lbl { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }
.gaia.le.fpwd { font-family: Arial, Helvetica, sans-serif; font-size: 70%; }
.gaia.le.chusr { font-family: Arial, Helvetica, sans-serif; font-size: 70%; }
.gaia.le.val { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }
.gaia.le.button { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }
.gaia.le.rem { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }

.gaia.captchahtml.desc { font-family: arial, sans-serif; font-size: smaller; } 
.gaia.captchahtml.cmt { font-family: arial, sans-serif; font-size: smaller; font-style: italic; }
  
--></style>
<style type="text/css"><!--
  div.errormsg { color: red; font-size: smaller; font-family:arial,sans-serif; }
  font.errormsg { color: red; font-size: smaller; font-family:arial,sans-serif; }  
--></style>
<style type="text/css"><!--
.gaia.le.lbl { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }
.gaia.le.fpwd { font-family: Arial, Helvetica, sans-serif; font-size: 70%; }
.gaia.le.chusr { font-family: Arial, Helvetica, sans-serif; font-size: 70%; }
.gaia.le.val { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }
.gaia.le.button { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }
.gaia.le.rem { font-family: Arial, Helvetica, sans-serif; font-size: smaller; }

.gaia.captchahtml.desc { font-family: arial, sans-serif; font-size: smaller; } 
.gaia.captchahtml.cmt { font-family: arial, sans-serif; font-size: smaller; font-style: italic; }
  
--></style>
 
<style type="text/css"><!--
    .body { margin-left: 3em;
            margin-right: 5em;
            font-family: arial,sans-serif; }

    div.errorbox-good {}

    div.errorbox-bad {} 

    div.errormsg { color: red; font-size: smaller; font-family: arial,sans-serif;}
    font.errormsg { color: red; font-size: smaller; font-family: arial,sans-serif;}
    
    hr {
      border: 0;
      background-color:#DDDDDD;
      height: 1px;
      width: 100%;
      text-align: left;
      margin: 5px;
    }
    
--></style>
<style type="text/css"><!--
#error {
	border: 1px dotted #cc9999;
	margin-bottom: 10px;
	padding: 6px 8px 6px 28px;
	background: url(images/icon_alert.gif) left center no-repeat;
	background-color: #fcfcfc;
	color: #cc0000;
}
#foot {
	width: 760px;
	margin: 20px auto;
	text-align: center;
	padding: 5px;
	font-size: 108%;
	border-top: 1px solid #ccc;
}

#foot,
#foot a {
	background-color: inherit;
	color: #999;
}

#foot a:hover {
	background-color: inherit;
	color: #70a8f1;
}
--></style>

</head>
<body dir="ltr" onload="gaia_setFocus();">
<div id="main">
  <table border="0" cellpadding="2" cellspacing="0" width="100%">
    <tbody>
      <tr>
        <td colspan="2"><img alt="" height="2" width="1"></td>
      </tr>
      <tr>
        <td valign="top" width="1%">
          <img src="/cosign/images/services/Cosign_3d.gif" alt="CoSign" align="left" border="0">
        </td>
        <td valign="bottom">
          <table border="0" cellpadding="0" cellspacing="0" width="100%">
            <tbody>
              <tr>
                <td colspan="2"><img alt="" height="15" width="1"></td>
              </tr>
              <tr bgcolor="#3366cc">
                <td colspan="2"><img alt="" height="1" width="1"></td>
              </tr>
              <tr bgcolor="#e5ecf9">
                <td style="padding-left: 4px; padding-bottom: 3px; padding-top: 2px; font-family: arial,sans-serif;">
                  <b><?php echo $username; ?> 的帐户</b>
                </td>
                <td style="padding-left: 4px; padding-bottom: 3px; padding-top: 2px; font-family: arial,sans-serif;" align="right">
                  <a href="?logout">退出</a>&nbsp;&nbsp;&nbsp;&nbsp;
                </td>
              </tr>
              <tr>
                <td colspan="2"><img alt="" height="5" width="1"></td>
              </tr>
            </tbody>
          </table>
        </td>
      </tr>
    </tbody>
  </table>
  <br>
  <table align="center" border="0" cellpadding="5" cellspacing="1" width="90%">
    <tbody>
      <tr>
        <td style="padding-left: 10px;" align="left" valign="top">
          <b>个人信息</b>
          <hr color="#dddddd" noshade="noshade" size="1">
          <br>
          <ul>
            <li><?php echo $username; ?></li>
            <li><?php echo "$uid ($aliasname)"; ?></li>
            <li><?php echo "$mail"; ?></li>
            <li><a href="<?php echo $url_gosa; ?>">修改口令</a></li>
          </ul>
        </td>
        <td valign="top" width="55%">
          <b>Web 服务</b>
          <hr color="#dddddd" noshade="noshade" size="1">
          <div style="font-size: 83%;">
            <br><br>
<?php
$has_services = false;
$srvlist = dirname( dirname(__FILE__) ) . "/templates-local/zh/inc/*.html";
foreach (glob($srvlist) as $filename) {
  if (file_exists($filename))
  {
    $contents = file_get_contents($filename);
    if ($contents)
    {
      $has_services = true;
      echo ($contents);
    }
  }
}
if (! $has_services)
{
  echo "于下面的位置定义服务列表:<br>\n";
  echo "<ul><li>$srvlist</li></ul>\n";
}
?>
          </div>
        </td>
      </tr>
    </tbody>
  </table>
  <br>
  <br>
<?php
  $footer = dirname( dirname(__FILE__) ) . "/templates-local/zh/inc/footer.inc";
  if (file_exists($footer))
  {
    $contents = file_get_contents($footer);
    echo $contents;
  }
  else
  {
?>
  <div id="foot">&copy; 2006-2008 <a href="http://www.umich.edu/~regents/" title="密歇根大学">密歇根大学</a>, 
       <a href="http://www.ossxp.com/">群英汇技术支持</a></div>
<?php
  }
?>
</body>
</html>
<!-- vim: set ts=2 sw=2 et : -->
