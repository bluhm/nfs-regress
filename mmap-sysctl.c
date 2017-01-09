#include <machine/param.h>

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
#include <string.h>
#include <unistd.h>

int
main(void)
{
	char *p, file[] = "/tmp/nfsfile", page[PAGE_SIZE];
	int fd;
	int mib[] = { CTL_NET, PF_INET, IPPROTO_TCP, TCPCTL_ALWAYS_KEEPALIVE };

	if ((fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0777)) == -1)
		err(1, "open write '%s'", file);
	memset(page, 0, sizeof(page));
	if (write(fd, page, sizeof(page)) == -1)
		err(1, "write");
	if (close(fd) == -1)
		err(1, "close write");

	if ((fd = open(file, O_RDWR)) == -1)
		err(1, "open mmap '%s'", file);
	p = mmap(NULL, sizeof(struct tcpstat), PROT_READ|PROT_WRITE,
	    MAP_SHARED, fd, 0);
	if (p == MAP_FAILED)
		err(1, "mmap");
	if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), NULL, 0,
	    p, sizeof(int)) == -1)
		err(1, "sysctl keepalive");
	if (close(fd) == -1)
		err(1, "close mmap");

	return (0);
}
