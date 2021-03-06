#!/bin/sh
#-*-sh-*-

#
# Copyright © 2009-2013 Inria.  All rights reserved.
# See COPYING in top-level directory.
#

HWLOC_top_builddir="/Users/yufanxue/Documents/cs541/hwloc-1.9"
annotate="$HWLOC_top_builddir/utils/hwloc-annotate"
HWLOC_top_srcdir="/Users/yufanxue/Documents/cs541/hwloc-1.9"

HWLOC_PLUGINS_PATH=${HWLOC_top_builddir}/src
export HWLOC_PLUGINS_PATH

if test x0 = x1; then
  # make sure we use default numeric formats
  LANG=C
  LC_ALL=C
  export LANG LC_ALL
fi

: ${TMPDIR=/tmp}
{
  tmp=`
    (umask 077 && mktemp -d "$TMPDIR/fooXXXXXX") 2>/dev/null
  ` &&
  test -n "$tmp" && test -d "$tmp"
} || {
  tmp=$TMPDIR/foo$$-$RANDOM
  (umask 077 && mkdir "$tmp")
} || exit $?
file="$tmp/test-hwloc-annotate.output"

set -e

$annotate $HWLOC_top_srcdir/utils/test-hwloc-annotate.input $file all info Foo Bar
$annotate --ci $file $file Core:all info Foo2 Bar2
$annotate --ci $file $file L2Cache:0 none
$annotate --ri $file $file socket:all info Foo
$annotate $file $file socket:0 info Foo2 Bar
$annotate $file $file socket:0 info Foo2 Bar2
$annotate --ri $file $file socket:0 info Foo2 Bar3

diff -u $HWLOC_top_srcdir/utils/test-hwloc-annotate.output "$file"
rm -rf "$tmp"
