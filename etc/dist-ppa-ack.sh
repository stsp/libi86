#!/bin/bash
# Create Debian source package for libi86 for use with the Amsterdam Compiler
# Kit (https://github.com/davidgiven/ack).  Public domain.

set -e
case "$DEBSIGN_KEYID" in
  '') signing=("-us" "-uc");;
  *)  signing=("-k$DEBSIGN_KEYID");;
esac
# If the first argument is a directory path, treat it as a destination
# directory to place the output source package files in.
dest=.
case "$1" in
  */*)
    dest="$1"
    shift;;
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
# Make sure we are in the top-level libi86 source directory.
if test \! -f README.asciidoc; then
  echo 'no README.asciidoc --- are you in libi86 top directory?' >&2
  exit 1
fi
# Make a copy of the source tree.  If our source is under Git, use the Git
# metadata to decide which files to copy.
tmp=
trap 'rm -rf "$tmp"' ERR EXIT TERM QUIT
tmp="`mktemp -d`"
if test -z "$tmp"; then
  echo 'could not create temporary directory!' >&2
  exit 1
fi
if test -f .git/config; then
  git ls-files -z \
   | tar cf - --no-recursion --null -T - --transform 's,^,libi86/,' \
   | tar xf - -C "$tmp"
else
  cp -a . "$tmp"/libi86
fi
# Create the source packages.
pushd "$tmp"/libi86
li_uver="`cat configure \
	  | sed -n "/^PACKAGE_VERSION='/ { s/^.*='*//; s/'*$//; p; q; }" || :`"
test -n "$li_uver"
curr_tm="`date -R`"
ppa_no="`date -d "$curr_tm" +%y%m%d%H%M`"
ppa_no="${ppa_no%?}"
for distro in "$@"; do
  for f in debian/control; do
    sed "s|@distro@|$distro|g" $f.host-ack.in >$f
  done
  cp debian/rules.host-ack debian/rules
  li_pver="$li_uver-ppa$ppa_no~$distro"
  (
    echo "libi86-ack ($li_pver) $distro; urgency=medium"
    echo
    echo '  * Release.'
    echo
    echo " -- user <user@localhost.localdomain>  $curr_tm"
  ) >debian/changelog
  debuild --no-tgz-check -i -S -rfakeroot -d ${signing[@]}
done
popd
# Copy out the source packages to the destination directory.
cp -a "$tmp"/libi86-ack_* "$dest"
# The temporary directory will now be deleted.
