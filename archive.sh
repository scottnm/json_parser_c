ARCHIVEDIR=".archive"
FILENAME="$ARCHIVEDIR/$(date +%F).txt"

if [ -f $FILENAME ]; then
    echo "file with that name already exists. archive manually"
else
    mv main.c $FILENAME
    echo "archived! $FILENAME"
fi
