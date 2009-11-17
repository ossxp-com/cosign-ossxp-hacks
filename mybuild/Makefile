SOURCEDIR=cosign-ossxp

all:
	@echo "clean - remove *.deb and other temporary files."
	@echo "dir   - extract source package."
	@echo "build - generate config files for current dists and run dpkg-buildpackage"

clean:
	-rm -f ossxp-cosign*changes ossxp-cosign*.deb 
	-rm -rf $(SOURCEDIR)/.pc
	(cd $(SOURCEDIR); hg revert -a)
	(cd $(SOURCEDIR); hg clean; hg qpop -a; hg up)
	(cd $(SOURCEDIR)/.hg/patches; hg up; hg st;)

dir: clean
	(cd $(SOURCEDIR); [ -d debian ] || ln -s $$(dirname $$(dirname $$(pwd)))/debian debian; hg st)
	sudo setfacl -Rb $(SOURCEDIR) ../debian

debian/patches:
	(cd $(SOURCEDIR); make -f debian/rules debian/patches)

build:
	(cd $(SOURCEDIR); make -f debian/rules debian/control || true)
	(cd $(SOURCEDIR); dpkg-buildpackage -b -rfakeroot || true)
	(cd $(SOURCEDIR); mv $$(dirname $$(pwd -P))/ossxp-cosign*changes $(CURDIR); mv $$(dirname $$(pwd -P))/ossxp-cosign*deb $(CURDIR);)

.PHONY: all clean dir
# vim: noet
