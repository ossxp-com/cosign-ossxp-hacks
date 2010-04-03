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
