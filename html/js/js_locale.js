var Localization = {
	"0001": {
		"en": "Login failed: user does not exist or wrong password.",
		"zh": "登录失败: 用户不存在或错误的口令。" },

	"0002": {
		"en": "Cannot connect to LDAP server.",
		"zh": "无法连接到 LDAP 服务器。" },

	"0101": {
		"en": "Login failed: email or invite_code not provided.",
		"zh": "登录失败: 请输入邮件地址以及邀请码。" },

	"0102": {
		"en": "Login failed: invite factor has not setup yet.",
		"zh": "登录失败: 邀请码尚未设置。" },

	"0103": {
		"en": "Login failed: provide your email address as login name.",
		"zh": "登录失败: 请在用户名输入框输入您的邮件地址。" },

	"0104": {
		"en": "Login failed: wrong invite code.",
		"zh": "登录失败: 错误的邀请码。" },

	"0105": {
		"en": "Please enter your login and password.",
		"zh": "请输入您的用户名和口令。" },

	"0106": {
		"en": "Please input your email address into username field.",
		"zh": "请在用户名输入框输入您的邮件地址。" },

	"default": "en"
};

function setLocale(lang) {
	if (!lang) lang = "en";
	Localization.default = lang;
}

(function() {
	function extend(destination, source) {
		for (var property in source)
			destination[property] = source[property];
			return destination;
	}

	extend(String.prototype, (function() {
		function localize(lang) {
			var code;
			var s;
			if (code = /^([0-9]+)\s*:/.exec(this)) code = code[1];
			if (Localization) {
				s= Localization[code] ? Localization[code] : Localization[this];
				if (!s) return this;
				if (s instanceof Object) {
					if (!lang) lang = Localization.default;
					return s[lang] ? s[lang] : s["en"];
				}
				return s;
			}
			return this;
		}

		return {
			l: localize,
			_: localize,
			localize: localize
		};
	})());
})();

// vim: noet ts=4 sw=4
