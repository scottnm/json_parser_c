PROJ_ROOT=~/dev/json_parser_c

TFROOT=test/jsons
T1=numbers.json
T2=numbers_and_strings.json
T3=test_objs.json
T4=simple_arr.json

echo $T1
$PROJ_ROOT/main $PROJ_ROOT/$TFROOT/$T1
echo "\n"

echo $T2
$PROJ_ROOT/main $PROJ_ROOT/$TFROOT/$T2
echo "\n"

echo $T3
$PROJ_ROOT/main $PROJ_ROOT/$TFROOT/$T3
echo "\n"

echo $T4
$PROJ_ROOT/main $PROJ_ROOT/$TFROOT/$T4
echo "\n"
