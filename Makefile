#	$OpenBSD$

PROG=		mmap-sysctl
CLEANFILES=	diskimage

.PHONY: disk nfs mount unconfig clean

disk: unconfig
	dd if=/dev/zero of=diskimage bs=512 count=4k
	vnconfig vnd0 diskimage
	newfs vnd0c

nfs:
	grep '/mnt/nfs-server\>' /etc/exports || \
	    echo /mnt/nfs-server -maproot=0:0 127.0.0.1 >>/etc/exports
	rcctl -f start portmap
	rcctl -f start nfsd
	rcctl -f start mountd

mount: disk nfs
	mkdir -p /mnt/nfs-server
	mount /dev/vnd0c /mnt/nfs-server
	mkdir -p /mnt/nfs-client
	mount -t nfs 127.0.0.1:/mnt/nfs-server /mnt/nfs-client

unconfig:
	-umount -f -t nfs -h 127.0.0.1 -a
	-rmdir /mnt/nfs-client
	-pkill mountd || true
	-rcctl -f stop nfsd
	-rcctl -f stop portmap
	-umount -f /dev/vnd0c 2>/dev/null || true
	-rmdir /mnt/nfs-server
	-vnconfig -u vnd0 2>/dev/null || true

clean: _SUBDIRUSE unconfig
	rm -f a.out [Ee]rrs mklog *.core y.tab.h \
	    ${PROG} ${OBJS} ${_LEXINTM} ${_YACCINTM} ${CLEANFILES}

.include <bsd.regress.mk>
