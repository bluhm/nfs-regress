#include <sys/mman.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/sysctl.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/tcp_seq.h>
#include <netinet/tcp_timer.h>
#include <netinet/tcp_var.h>

#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void)
{
	char *p, file[] = "/tmp/nfsfile";
	size_t len;
	int fd, mib[] = { CTL_NET, PF_INET, IPPROTO_TCP, TCPCTL_STATS };

	if ((fd = open(file, O_RDWR|O_CREAT, 0777)) == -1)
		err(1, "open '%s'", file);

	if (lseek(fd, 4096, SEEK_SET) == (off_t)-1)
		err(1, "lseek 4096");

	p = mmap(NULL, sizeof(struct tcpstat), PROT_READ|PROT_WRITE,
	    MAP_SHARED, fd, 0);
	if (p == MAP_FAILED)
		err(1, "mmap");

	len = sizeof(struct tcpstat);
	if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), p, &len, NULL, 0) == -1)
		err(1, "sysctl tcpstat");

	return (0);
}
