POFILES = $(wildcard */LC_MESSAGES/*.po)
MOFILES = $(POFILES:po=mo)
DOMAIN = template

all: mo

po: $(POFILES)
mo: $(MOFILES)
pot: $(DOMAIN).pot

$(POFILES): $(DOMAIN).pot
	msgmerge $@ $< -o $@.new
	mv $@.new $@

$(DOMAIN).pot:
	xgettext --default-domain=$(DOMAIN) -L C -k_  \
		--no-location \
		-o $(DOMAIN).pot \
		../*.html ../inc/*.inc

%.mo: %.po
	msgfmt -o $@ $<

#.PHONY: pot po mo $(DOMAIN).pot

# vim: noet ts=4 sw=4
