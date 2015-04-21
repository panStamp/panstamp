#! /bin/sh
#
# panStamp init.d script for Raspbian
#

case "$1" in
  start)
  echo "Starting lagarto-max"
  python /home/pi/python/lagarto/lagarto-max/lagarto-max.py &
  sleep 5
  echo "Starting lagarto-swap"
  python /home/pi/python/lagarto/lagarto-swap/lagarto-swap.py &
  ;;
  stop)
  echo "Killing all python processes"
  killall python
  ;;
esac

exit 0

  
