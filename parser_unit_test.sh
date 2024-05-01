clear

mkdir -p outputs/

TERM_WITDH=$(tput columns)

for ((i = 1; i <= 20; i++)); do
	L_DIVIDER=""
	R_DIVIDER=""
	TEST_TITLE="[samples/sample""$i"".puk]"
	TITLE_LENGTH=${#TEST_TITLE}
	LEFT=$(((($TERM_WITDH - $TITLE_LENGTH)) / 2))

	for ((j = 0; j < $LEFT; j++)); do
		L_DIVIDER=$L_DIVIDER"="
	done

	R_DIVIDER=$L_DIVIDER

	if ((${#i} % 2 == 1)); then
		R_DIVIDER=$R_DIVIDER"="
	fi

	echo $L_DIVIDER$TEST_TITLE$R_DIVIDER

	./run.sh < "samples/sample""$i"".puk" > "outputs/out""$i"".txt"
	cat "outputs/out""$i"".txt"

done

DONE="[Done]"
DONE_LEFT=$(((($TERM_WITDH - ${#DONE})) / 2))
for ((j = 0; j < $DONE_LEFT; j++)); do
	DONE_L_DIVIDER=$DONE_L_DIVIDER"="
done

echo $DONE_L_DIVIDER$DONE$DONE_L_DIVIDER
