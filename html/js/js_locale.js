var Localization = {
	"9999": {
		"en": "Cannot open logfile.",
		"zh": "无法打开日志文件。" },

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
