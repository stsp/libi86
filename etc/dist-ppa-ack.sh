#!/bin/bash
# Create Debian source package.  Public domain.

set -e
case "$DEBSIGN_KEYID" in
  '') signing=("-us" "-uc");;
  *)  signing=("-k$DEBSIGN_KEYID");;
esac
# If no target Ubuntu distribution is specified, obtain the code name for
# whatever Linux distribution we are running on, or fall back on a wild
# guess.
if test 0 = $#; then
  distro="`lsb_release -c -s || :`"
  if [ -z "$distro" ]; then
    distro="`sed -n '/^DISTRIB_CODENAME=[[:alnum:]]\+$/ { s/^.*=//; p; q; }' \
		 /etc/lsb-release || :`"
  fi
  if [ -z "$distro" ]
    then distro=xenial; fi
  set -- "$distro"
fi
trap 'rm -f debian/control debian/changelog debian/rules' ERR EXIT TERM QUIT
for distro in "$@"; do
  for f in debian/control debian/changelog; do
    sed "s|@distro@|$distro|g" $f.host-ack.in >$f
  done
  cp debian/rules.host-ack debian/rules
  debuild --no-tgz-check -i -S -rfakeroot -d ${signing[@]}
done
