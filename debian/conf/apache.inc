# General CoSign configuration:
CosignProtected         Off
CosignHostname          cosignd.foo.bar
CosignRedirect          https://weblogin.foo.bar/cgi-bin/login
CosignPostErrorRedirect https://weblogin.foo.bar/cosign/post_error.html
CosignCrypto /opt/cosign/conf/certs/foo.bar/filter-1.key /opt/cosign/conf/certs/foo.bar/filter-1.pem /opt/cosign/conf/certs/CA
