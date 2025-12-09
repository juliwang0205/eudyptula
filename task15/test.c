#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#define EUDYPTULA_SYSCALL 548

int main(int argc, char *argv[])
{
	int rc = 0;

	printf(">> sys_eudyptula(0x01, 0x01)\n");
	rc = syscall(EUDYPTULA_SYSCALL, 0x01, 0x01);
	printf("Return code: %d\n", rc);
	assert(rc != 0);

	printf(">> sys_eudyptula(0x7c1c,0xaf2f50d1)\n");
	rc = syscall(EUDYPTULA_SYSCALL, 0x7c1c,0xaf2f50d1);
	printf("Return code: %d\n", rc);
	assert(rc == 0);

	return 0;
}
