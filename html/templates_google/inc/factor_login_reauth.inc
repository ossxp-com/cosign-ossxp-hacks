				<div class="factorHead" style="display: none;">
					<h2 id="kerbTitle"><span>&nbsp;</span>$!_("ID &amp; Password")</h2>
					<div class="authComplete" style="display: none;">$!_("Authentication Complete")</div>
				</div>
				<div class="factor" id="kerberos" style="display: none;">
					<table>
						<tr>
							<td><label for="login">$!_("Username")</label></td>
							<td
							id="login">$l (<a href="/cgi-bin/logout" class="switchUser" id="switchUserR" tabindex="1">$!_("Switch Users")</a>)</td>
						</tr>
						<tr>
							<td><label for="password">$!_("Password")</label></td>
							<td><input id="password" name="password" type="password" value="" size="18" tabindex="2" /></td>
						</tr>
						<tr class="capsLock" style="display: none;">
							<td>&nbsp;</td>
							<td>$!_("Caps Lock is on")</td>
						</tr>
					</table>
					<a href="#kerbInfo" id="kerbInfoShow" tabindex="7">$!_("Need an account or password help?")</a> </div>
