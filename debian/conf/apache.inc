# General CoSign configuration:
CosignProtected         Off
CosignHostname          cosignd.local
CosignRedirect          https://weblogin.local/
CosignPostErrorRedirect https://weblogin.local/cosign/post_error.html
CosignCrypto /opt/cosign/conf/certs/mod_cosign.key /opt/cosign/conf/certs/mod_cosign.crt /opt/cosign/conf/certs/CA
