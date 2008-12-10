#!/bin/sh

read login
read password

case "$password" in
pass)
    echo "MAGIC"
    exit 0
    ;;

junk)
    echo "MAGIC-junk"
    exit 0
    ;;

error)
    echo "Woof!"
    exit 2
    ;;

xyzzy)
    echo "Nothing happens."
    exit 1
    ;;

fail)
    echo "Password incorrect."
    exit 1
    ;;

*)
    echo "$login typed $password.  D'oh!  Try again!"
    exit 1
    ;;
esac
