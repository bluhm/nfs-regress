#include <err.h>
#include <fcntl.h>

int
main(void)
{
	char file[] = "/nfs/nfsfile";
	int fd;

	if ((fd = open(file, O_WRONLY|O_CREAT, 0777)) == -1)
		err(1, "open '%s'", file);

	return (0);
}
