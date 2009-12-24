SOURCEDIR=cosign-ossxp

all:
	@echo "Nomal build procedures:"
	@echo "    make patch => make build"
	@echo "Tasks list:"
	@echo "    patch - export all t/* branch as debian patches."
	@echo "    clean - remove *.deb and other temporary files."
	@echo "    build - generate config files for current dists and run dpkg-buildpackage"
	@echo "    stub.build - build without update souce code."

$(SOURCEDIR):
	if [ ! -d $(SOURCEDIR) ]; then \
		 ln -s ../../cosign-ossxp $(SOURCEDIR); \
	fi

clean: $(SOURCEDIR) stub.clean
stub.clean:
	-rm stub.*
	-rm -f *.changes *.deb
	-rm -rf $(SOURCEDIR)/.pc
	(cd $(SOURCEDIR); git reset --hard; git clean -f -d; git checkout master; git fetch; )
	(cd $(SOURCEDIR); git checkout t/all; git clean -f -d; git fetch; tg summary; git checkout t/all; tg update; git status || true)
	(cd $(SOURCEDIR); git checkout master ; git clean -f -d;)
	(cd ..;  git clean -f -d; git status || true )
	touch stub.clean

dir: stub.dir
stub.dir: stub.clean
	(cd $(SOURCEDIR); [ -d debian ] || ln -s $$(dirname $(CURDIR))/debian debian; git status || true)
	sudo setfacl -Rb $(SOURCEDIR) $$(dirname $$(pwd -P))/debian
	touch stub.dir

patch: debian/patches
debian/patches: stub.dir
	(cd $(SOURCEDIR); make -f debian/rules debian/patches)
	-rm stub.*

build: stub.dir stub.build
stub.build:
	(cd $(SOURCEDIR); make -f debian/rules debian/control || true)
	(cd $(SOURCEDIR); dpkg-buildpackage -b -rfakeroot || true)
	(cd $(SOURCEDIR); if [ $$(dirname $$(pwd -P)) != $$(dirname $$(pwd)) ] ; then \
		mv $$(dirname $$(pwd -P))/*.changes $(CURDIR); mv $$(dirname $$(pwd -P))/*.deb $(CURDIR); \
		fi)
	-rm stub.clean

.PHONY: all clean build debian/patches
# vim: noet
