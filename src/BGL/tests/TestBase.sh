#!/bin/bash

base=`basename $0 .sh`

if [ "${base}" = "TestBase" ] ; then
    echo "This script isn't run directly. You make softlinks to it with the test names, and run it from those."
    exit 73
fi

export TIMEOUT=10

groupnum=`echo ${base} | sed 's/^.*G\([0-9]*\)[^0-9].*$/\1/'`
testnum=`echo ${base} | sed 's/^.*T\([0-9]*\)[^0-9]*$/\1/'`
subtest=`echo ${base} | sed 's/^.*T[0-9]*\([^0-9]\).*$/\1/'`

TESTROOT="G${groupnum}_T${testnum}"

SVGROOT="${TESTROOT}_*_${subtest}_"
SVGFILE="${SVGROOT}*.svg"

TESTBIN="./${TESTROOT}_*"
LOGFILE="Log-${TESTROOT}.txt"

${TESTBIN} > ${LOGFILE} 2>&1 &
PRODPID=$!

export LOGFILE
export PRODPID
export MAINPID=$$

killitnow() {
    ps -p $2 | grep -v "PID TTY" >> /dev/null 2>&1
    if [ $? == 0 ] ; then
	kill -$1 $2
	wait $2 2>/dev/null
    fi
}


exit_timeout() {
    echo "Timed Out." >> ${LOGFILE}
    killitnow SIGTERM ${PRODPID}
    exit -1
}
trap exit_timeout SIGUSR1


die_die_die() {
    echo "Killed by user." >> ${LOGFILE}
    killitnow SIGTERM ${PRODPID}
    exit -1
}
trap die_die_die SIGINT



( trap 'exit 0' TERM ; sleep ${TIMEOUT} ; killitnow SIGUSR1 ${MAINPID} ) &
TPID=$!
export TPID


wait ${PRODPID}
resval=$?

kill $TPID

if [ $resval -ne 0 ] ; then
    exit -1
fi

SVGFILE=`ls -1 $SVGFILE`
md5raw=`md5 $SVGFILE`
md5val=`echo ${md5raw} | sed 's/^.* = //'`
expected=`grep '^MD5 ('${SVGFILE} ExpectedResults.txt | tail -1 | sed 's/^.* = //'`

if [ "${md5val}x" != "${expected}x" ] ; then
    exit -1
fi
exit 0

