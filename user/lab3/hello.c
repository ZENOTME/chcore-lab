// hello, world
#include <lib/print.h>
#include <lib/type.h>
#include <lib/syscall.h>


int bias=0;
int main(int argc, char *argv[], char *envp[])
{
	for(int i=0;i<10;i++){
		printf("hello, world %d\n",1);
		bias++;
	}
	return 0;
}
