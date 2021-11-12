if [ $# -eq 0 ]; then
  echo "Puth path to the firmware as the first argument!"
  exit
fi

REPO_PATH=$(git rev-parse --show-toplevel)
TC_PATH=$REPO_PATH/toolchain/gcc-arm-none-eabi-9-2020-q2-update/bin

$TC_PATH/arm-none-eabi-gdb-py -ex 'target extended-remote localhost:3333' $1
