        <script type="text/javascript">
          function factor_invite_observe() {
            var input_p = $('password');
            var input_i = $('invite');
            var input_l = $('login');
            var form_login = input_i.up('form');

            Event.observe( form_login, 'submit', function(e) {
                  if (input_i.value.length > 0 && !input_l.value.include("@"))
                  {
                    alert("$!_("Please input your email address into username field.")");
                    input_l.focus();
                    Event.stop(e);
                  }
                  else if (input_p.value.length == 0 || input_l.value.length == 0)
                  {
                    alert("$!_("Please enter your login and password.")");
                    input_l.focus();
                    Event.stop(e);
                  }
                  return true;
              });

            Event.observe( $('invite'), 'focus', function() {
                  try {
                    var parent = input_p.up('div')
                    if (!parent.visible())
                      new Effect.Appear(parent,{duration:0.20});
                  } catch ( e ) {;}
                  input_p.disable().clear().up('tr').hide();
              });

            Event.observe( $('password'), 'focus', function() {
                  input_i.disable().clear();
              });

            Event.observe( $('invite'), 'blur', function() {
                if (this.value.length > 0) {
                  input_p.disable().clear().up('tr').hide();
                  if (!input_l.value.include("@")) {
                    input_l.select();
                  }
                } else {
                  input_p.enable().up('tr').show();
                }
              });

            Event.observe( $('password'), 'blur', function() {
                if (this.value.length > 0)
                  input_i.disable().clear();
                else
                  input_i.enable();
              });
          }

          Event.observe(window, 'load', factor_invite_observe);
        </script>
				<div class="factorHead" style="display: none;">
					<h2 id="inviteloginTitle"><span>&nbsp;</span>$!_("Invite Code")</h2>
					<div class="authComplete" style="display: none;">$!_("Authentication Complete")</div>
				</div>
				<div class="factor" id="invitelogin" style="display: none;">
					<table>
						<tr>
							<td><label for="invite">$!_("Invite code:")</label></td>
							<td><input id="invite" name="invite" type="password" value="" size="18" tabindex="5" /></td>
						</tr>
						<tr class="capsLock" style="display: none;">
							<td>&nbsp;</td>
							<td>$!_("Caps Lock is on")</td>
						</tr>
					</table>
					<a href="#inviteinfo" id="inviteInfoShow" tabindex="10">$!_("What is invite code?")</a> </div>

