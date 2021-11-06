if [ $# -eq 0 ]; then
  echo "Put path to the firmware as the first argument!"
  exit
fi

st-flash write $1 0x08000000
