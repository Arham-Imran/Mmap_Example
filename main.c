#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    printf("Hello, from Mmap!\n");
    size_t mapSize = sysconf(_SC_PAGESIZE);
    size_t mapOffset = sysconf(_SC_PAGESIZE);

    int fd = open("../adc_data.txt", O_RDWR);
    if(fd == -1)
    {
        return -1;
    }

    void *mappedMem = mmap(NULL, mapSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mapOffset);
    if(mappedMem == MAP_FAILED)
    {
        return -1;
    }

    pid_t pid = fork();

    if(pid == 0)
    {
        ((char*)mappedMem)[mapSize - 2] = 36;
    }
    else if(pid > 0)
    {
        wait(0);
        ((char*)mappedMem)[mapSize - 1] = 37;
        printf("%s\n", (char*)mappedMem);
    }

    int err = munmap(mappedMem, mapSize);
    close(fd);
    if(err == -1)
    {
        return -1;
    }
}
