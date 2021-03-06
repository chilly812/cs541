#!/bin/sh
#-*-sh-*-

#
# Copyright © 2012-2014 Inria.  All rights reserved.
# Copyright © 2010 Cisco Systems, Inc.  All rights reserved.
# Copyright © 2011 Université Bordeaux 1
# See COPYING in top-level directory.
#

HWLOC_top_builddir="/Users/yufanxue/Documents/cs541/hwloc-1.9"
lstopo="$HWLOC_top_builddir/utils/lstopo-no-graphics"
gather="$HWLOC_top_builddir/tests/linux/hwloc-gather-topology"

# make sure we use default numeric formats
LANG=C
LC_ALL=C
export LANG LC_ALL

# Ensure we're running on linux.  If not, then just exit with status
# code 77 (the GNU standard for a skipped test)

case `uname -a` in
*[Ll]inux*) good=1 ;;
*)
    echo "Not running on linux; skipped"
    exit 77 ;;
esac 

error()
{
    echo $@ 2>&1
}

if [ ! -x "$lstopo" ]
then
    error "Could not find executable file \`$lstopo'."
    exit 1
fi
if [ ! -x "$gather" ]
then
    error "Could not find executable script \`$gather'."
    exit 1
fi

tmpdir=`mktemp -d`

# make sure we compare things that are comparable
# (the tarball cannot be "this system")
export HWLOC_THISSYSTEM=0

echo "Saving current system topology to XML..."
if ! "$lstopo" --no-io "$tmpdir/save.xml" ; then
    error "Failed"
    exit 1
fi

echo "Saving current system topology to a tarball..."
if ! "$gather" "$tmpdir/save" ; then
    error "Failed"
    exit 1
fi

echo "Extracting tarball..."
if ! ( cd "$tmpdir" && tar xfj save.tar.bz2 ) ; then
     error "Failed"
     exit 1
fi
export HWLOC_FSROOT="$tmpdir/save"

rm -f "$tmpdir/save/proc/hwloc-nofile-info"

echo "Saving tarball topology to XML..."
if ! "$lstopo" --no-io "$tmpdir/save2.xml" ; then
    error "Failed"
    exit 1
fi

echo "Comparing XML outputs..."
( cd $tmpdir && diff -u save.xml save2.xml )
result=$?

rm -rf "$tmpdir"

exit $result
