#!/bin/sh

do_preinst()
{
    exit 0
}

do_postinst()
{
	DEMO_FILE="/data/demo.tar.gz"
	if [ -f "${DEMO_FILE}" ] ; then
		PROJECT_PATH="/data/factory/projects"	
		if [ -d "${PROJECT_PATH}/DEMO" ] ; then
			rm -rf "${PROJECT_PATH}/DEMO"
		fi
		mkdir -p "${PROJECT_PATH}"
		tar xvf "${DEMO_FILE}" -C "${PROJECT_PATH}"
		sync
		rm -f "${DEMO_FILE}"
	fi
	exit 0
}

case "$1" in
preinst)
    do_preinst
    ;;
postinst)
    do_postinst
    ;;
*)
    echo "default"
    exit 1
    ;;
esac
