#!/bin/sh
#-*-sh-*-

#
# Copyright © 2009-2013 Inria.  All rights reserved.
# See COPYING in top-level directory.
#

HWLOC_top_builddir="/Users/yufanxue/Documents/cs541/hwloc-1.9"
diff="$HWLOC_top_builddir/utils/hwloc-diff"
patch="$HWLOC_top_builddir/utils/hwloc-patch"
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

cd "$tmp"
diffoutput="test-hwloc-diffpatch.diff.xml"
output1="test-hwloc-diffpatch.output1"
output2="test-hwloc-diffpatch.output2"

set -e

$diff $HWLOC_top_srcdir/utils/test-hwloc-diffpatch.input1 $HWLOC_top_srcdir/utils/test-hwloc-diffpatch.input2 > $diffoutput
cp $HWLOC_top_srcdir/utils/test-hwloc-diffpatch.input1 .
#cat $diffoutput | $patch $HWLOC_top_srcdir/utils/test-hwloc-diffpatch.input1 - $output1
cat $diffoutput | $patch refname - $output1
$patch -R $HWLOC_top_srcdir/utils/test-hwloc-diffpatch.input2 $diffoutput $output2

diff -u $HWLOC_top_srcdir/utils/test-hwloc-diffpatch.input1 "$output2"
diff -u $HWLOC_top_srcdir/utils/test-hwloc-diffpatch.input2 "$output1"

cd ..
rm -rf "$tmp"
