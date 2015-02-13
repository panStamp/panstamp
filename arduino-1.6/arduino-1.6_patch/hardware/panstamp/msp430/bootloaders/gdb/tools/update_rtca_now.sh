#!/bin/bash

strip_zero()
{
	cat | sed 's|^0||'
}


cat << EOF > drivers/rtca_now.h

#ifndef __RTCA_NOW_H__
#define __RTCA_NOW_H__

#define COMPILE_YEAR `date +%Y`
#define COMPILE_MON `date +%m | strip_zero`
#define COMPILE_DAY `date +%d | strip_zero`
#define COMPILE_DOW `date +%u`
#define COMPILE_HOUR `date +%H | strip_zero`
#define COMPILE_MIN `date +%M | strip_zero`

#endif
EOF
