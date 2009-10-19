SOURCEDIR=cosign-ossxp

all:
	@echo "clean - remove *.deb and other temporary files."
	@echo "dir   - extract source package."
	@echo "build - generate config files for current dists and run dpkg-buildpackage"

clean:
	#-rm -f *changes *.deb 
	#-rm -rf cosign-2.1.1
	-rm -f ../../*changes ../../*.deb 
	-rm -rf $(SOURCEDIR)/*
	-rm -rf $(SOURCEDIR)/.pc

dir: clean
	#tar zxvf cosign-2.1.1.tar.gz
	#sudo setfacl -Rb cosign-2.1.1
	#sudo setfacl -Rb ../debian
	(cd $(SOURCEDIR)/.hg/patches; hg pull ; hg up)
	(cd $(SOURCEDIR); hg pull ; hg up ; hg qpop -a)
	(cd $(SOURCEDIR); [ -d debian ] || ln -s $$(dirname $$(dirname $$(pwd)))/debian debian; hg st)
	sudo setfacl -Rb $(SOURCEDIR) ../debian

build:
	(cd $(SOURCEDIR); make -f debian/rules debian/control)
	(cd $(SOURCEDIR); dpkg-buildpackage -b -rfakeroot)

.PHONY: all clean dir
# vim: noet
