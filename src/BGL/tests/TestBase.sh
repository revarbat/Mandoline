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

exit_timeout() {
    echo "Timed Out." >> ${LOGFILE}
    ps -p ${PRODPID} | grep -v "PID TTY" >> /dev/null 2>&1
    if [ $? == 0 ] ; then
	kill ${PRODPID}
	wait ${PRODPID} 2>/dev/null
    fi
    exit -1
}

trap exit_timeout SIGUSR1


( trap 'exit 0' TERM ; sleep ${TIMEOUT} ; kill -SIGUSR1 ${MAINPID} ; wait ${MAINPID} 2>/dev/null) &
TPID=$!

wait ${PRODPID}
resval=$?

kill $TPID

if [ $resval -ne 0 ] ; then
    exit -1
fi

SVGFILE=`ls -1 $SVGFILE`
md5raw=`md5 $SVGFILE`
md5val=`echo ${md5raw} | sed 's/^.* = //'`
expected=`grep '^MD5 ('${SVGFILE} ExpectedResults.txt | sed 's/^.* = //'`

if [ "${md5val}x" != "${expected}x" ] ; then
    exit -1
fi
exit 0

