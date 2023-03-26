#/bin/sh

# This script will run inside the VM and it's only intended
# for the new "make console" setup. It will rename
# /dev/vda to /dev/vdb and /dev/vdb to /dev/vdc
# This way we ensure backward compatibility with
# the old "make boot" setup that uses 3 disks,
# the rootfs => /dev/vda, disk1.img => /dev/vdb
# disk2.img => /dev/vdc. By contrast, the newer setup
# uses a shared samba server as the rootfs and the other
# two disks are mapped to /dev/vda and /dev/vdb.

case "$1" in
        start)
                mv /dev/vdb /dev/vdc
		mv /dev/vda /dev/vdb
                ;;
        *)
                echo "Usage: $0 start"
                exit 1
                ;;
        esac

exit 0

