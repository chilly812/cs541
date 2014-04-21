#!/bin/sh
#-*-sh-*-

#
# Copyright © 2009-2013 Inria.  All rights reserved.
# Copyright © 2009, 2011 Université Bordeaux 1
# See COPYING in top-level directory.
#

HWLOC_top_builddir="/Users/yufanxue/Documents/cs541/hwloc-1.9"
assembler="$HWLOC_top_builddir/utils/hwloc-assembler"
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
file="$tmp/test-hwloc-assembler.output"

set -e

$assembler $file \
	--name input1 $HWLOC_top_srcdir/utils/test-hwloc-assembler.input1 \
	--name input2 $HWLOC_top_srcdir/utils/test-hwloc-assembler.input2 \
	--name input1again $HWLOC_top_srcdir/utils/test-hwloc-assembler.input1
diff -u $HWLOC_top_srcdir/utils/test-hwloc-assembler.output "$file"
rm -rf "$tmp"
