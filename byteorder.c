#include <stdio.h>
#include <string.h>
#include <sys/sysctl.h>


void deviceInfo();

int main(int argc, char const *argv[])
{
	union {
		short s;
		char c[sizeof(short)];
	} un;
	un.s = 0x0102;

	deviceInfo();	// CPU, VENDOR, OS
	printf("Byteorder:\t");
	if (sizeof(short) == 2) {
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	}
	else
		printf("sizeof(short) = %d\n", sizeof(short));

	return 0;
}

void deviceInfo()
{
	int ret;
	char str[256];
	size_t size_str;

	printf("CPU:\t\t");
	size_str = sizeof(str);
	ret = sysctlbyname("machdep.cpu.brand_string", str, &size_str, NULL, 0);
	printf("%s\n", str);

	printf("VENDOR:\t\t");
#if defined(__APPLE__)
	printf("Apple\n");
#elif defined(__linux__)
	printf("Linux\n");
#elif defined(WIN32) || defined(_WIN32)
	printf("Windows\n");
#else
	printf("Unknown\n");
#endif

	printf("OS:\t\t");
	size_str = sizeof(str);
	ret = sysctlbyname("kern.ostype", str, &size_str, NULL, 0);
	printf("%s ", str);
	size_str = sizeof(str);
	ret = sysctlbyname("kern.osrelease", str, &size_str, NULL, 0);
	printf("Kernel Version %s\n", str);
}