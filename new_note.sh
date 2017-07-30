DATE="========== $(date +"%b %d, %Y") =========="
NOTE=$1

echo "$DATE\n$NOTE\n\n$(cat notes.txt)" > notes.txt
