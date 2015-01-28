$QTDIR/bin/qmake src/Launchy.pro

$QTDIR/bin/lupdate src/Launchy.pro

$QTDIR/bin/lrelease src/common/common.pro
$QTDIR/bin/lrelease src/launchy/launchy.pro
$QTDIR/bin/lrelease src/plugins/controly/controly.pro
$QTDIR/bin/lrelease src/plugins/runner/runner.pro
$QTDIR/bin/lrelease src/plugins/verby/verby.pro
$QTDIR/bin/lrelease src/plugins/weby/weby.pro
