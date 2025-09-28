#!/usr/bin/env bash

# This is a wrapper script to conveniently run datacollector and statistics tool.
# This is designed to be symlinked to users PATH, so that the script itself can
# live in this directory.
#
# However, because of symlinking, we cannot get the script location in the filesystem
# and hence the script dir must be hardcoded:

SCRIPT_DIR="/home/jaba/OmatProjektit/training-diary-v2"

# Show last training date and type
# Not very elegant solution but works for now
echo_last_training () {
    line=$(tail -1 "${SCRIPT_DIR}/data/2025.csv")
    echo "Edellinen treenikerta ${line::10}."

    type=${line:11:1}
    case ${type} in
        "y")
            echo "Edellinen treeni oli ylävartalo."
            ;;
        "j")
            echo "Edellinen treeni oli juoksu."
            ;;
        "h")
            echo "Edellinen treeni oli hiihto."
            ;;
        "c")
            echo "Edellinen treeni oli crossfit."
            ;;
        "k")
            echo "Edellinen treeni oli kävely."
            ;;
        "a")
            echo "Edellinen treeni oli jalkatreeni."
            ;;
        *)
            echo "Tuntematon"
            ;;
    esac
}

collect() {
	FILE="${SCRIPT_DIR}/data_collector.out"
	if test -f "$FILE"; then
		${SCRIPT_DIR}/data_collector.out $1
	else
		make -C ${SCRIPT_DIR}
		${SCRIPT_DIR}/data_collector.out $1
	fi
}

statistics()
{
	FILE="${SCRIPT_DIR}/statistics_tool.out"
	if test -f "$FILE"; then
		${SCRIPT_DIR}/statistics_tool.out $1
	else
		make -C ${SCRIPT_DIR}
		${SCRIPT_DIR}/statistics_tool.out $1
	fi
}

case $1 in
	stat)
		statistics $2
		;;
	save)
		echo_last_training
		collect $1
		;;
	help)
		cat <<EOF
Usage:

	Default behavior (no arguments): runs data collector without arguments,
	defaults saving to ./data/<current year>.csv

	help: print this message
	stat <optional; file to read>: run statistics tool; searches for ./data/<current year>.csv by default
	save <optional; file to write>: run data collector, saves into ./data/<current year>.csv by default
EOF
		;;
	*)
		echo_last_training
		collect $1
		;;
esac
