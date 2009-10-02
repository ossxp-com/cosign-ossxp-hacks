all:
	@echo "please run make clean!"
	@echo "or make dir!"

clean:
	-rm -f *changes *.deb 
	-rm -rf cosign-2.1.1

dir: clean
	tar zxvf cosign-2.1.1.tar.gz
	sudo setfacl -Rb cosign-2.1.1
	sudo setfacl -Rb ../debian

.PHONY: all clean dir
# vim: noet
