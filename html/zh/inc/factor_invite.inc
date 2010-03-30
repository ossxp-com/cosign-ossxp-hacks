        <script type="text/javascript">
          function factor_invite_observe() {
            var input_p = $('password');
            var input_i = $('invite');
            var input_l = $('login');
            var form_login = input_i.up('form');

            Event.observe( form_login, 'submit', function(e) {
                  if (input_p.textLength > 0 && input_l.value.length == 0)
                  {
                    alert("Please enter your login and password."._());
                    input_l.focus();
                    Event.stop(e);
                  }
                  if (input_i.textLength > 0 && !input_l.value.include("@"))
                  {
                    alert("Please input your email address into username field."._());
                    input_l.focus();
                    Event.stop(e);
                  }
                  return true;
              });

            Event.observe( $('invite'), 'focus', function() {
                  try {
                    parent = input_p.up('div')
                    if (!parent.visible())
                      new Effect.Appear(parent,{duration:0.20});
                  } catch ( e ) {;}
                  input_p.disable().clear().up('tr').hide();
              });

            Event.observe( $('password'), 'focus', function() {
                  input_i.disable().clear();
              });

            Event.observe( $('invite'), 'blur', function() {
                if (this.textLength > 0) {
                  input_p.disable().clear().up('tr').hide();
                  if (!input_l.value.include("@")) {
                    input_l.select();
                  }
                } else {
                  input_p.enable().up('tr').show();
                }
              });

            Event.observe( $('password'), 'blur', function() {
                if (this.textLength > 0)
                  input_i.disable().clear();
                else
                  input_i.enable();
              });
          }

          Event.observe(window, 'load', factor_invite_observe);
        </script>
				<div class="factorHead" style="display: none;">
					<h2 id="inviteloginTitle"><span>&nbsp;</span>邀请码认证</h2>
					<div class="authComplete" style="display: none;">认证完成</div>
				</div>
				<div class="factor" id="invitelogin" style="display: none;">
					<table>
						<tr>
							<td><label for="invite">邀请码</label></td>
							<td><input id="invite" name="invite" type="password" value="" size="18" tabindex="5" /></td>
						</tr>
						<tr class="capsLock" style="display: none;">
							<td>&nbsp;</td>
							<td>大小写锁定键被打开</td>
						</tr>
					</table>
					<a href="#inviteinfo" id="inviteInfoShow" tabindex="10">什么是邀请码？</a> </div>

