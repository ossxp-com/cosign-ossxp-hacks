
all:
	@for file in *; do \
	    if [ ! -f $$file ]; then echo "File $$file not exist"; rm $$file; fi ; \
	done
	@for cert in /opt/ossxp/ssl/ca/* ; do \
		if [ $$cert = $${cert%.pem} ] && [ $$cert = $${cert%.crt} ] ; then \
			continue; \
		fi; \
		if [ -f `basename $$cert` ]; then continue; fi; \
		echo "link certificate $$cert here."; \
		ln -sf $$cert . ; \
	 done
	@c_rehash .

.PHONY: all

# vim: noet
