PROJ_ROOT=~/dev/json_parser

TFROOT=test/jsons
T1=numbers.json
T2=numbers_and_strings.json
T3=test_objs.json

echo $T1
$PROJ_ROOT/main $PROJ_ROOT/$TFROOT/$T1
echo "\n"

echo $T2
$PROJ_ROOT/main $PROJ_ROOT/$TFROOT/$T2
echo "\n"

echo $T3
$PROJ_ROOT/main $PROJ_ROOT/$TFROOT/$T3
echo "\n"
