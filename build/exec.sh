# This points to a file, which should contains hostnames (one per line).
# E.g.,
#
# worker1
# worker2
# worker3
#
MACHINE_CFG=/data/opt/tmp/1155107959/husky-sql/build/slaves

# This point to the directory where Husky binaries live.
# If Husky is running in a cluster, this directory should be available
# to all machines.
BIN_DIR=/data/opt/tmp/tati/husky/build
time pssh -t 0 -P -h ${MACHINE_CFG} \
        -x "-t -t" "cd $BIN_DIR && ls $BIN_DIR > /dev/null && ./$@"
