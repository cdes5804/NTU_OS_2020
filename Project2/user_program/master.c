#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define PAGE_SIZE 4096
#define BUF_SIZE (PAGE_SIZE * 20)
#define MMAP_SIZE (PAGE_SIZE * 20)

#define MAX_FILENO 16
#define MAX_FILENAME 64

#define master_IOCTL_CREATESOCK 0x12345677
#define master_IOCTL_MMAP 0x12345678
#define master_IOCTL_EXIT 0x12345679

size_t get_filesize(const char* filename);//get the size of the input file

#define ERR_EXIT(msg) { perror(msg); exit(-1); }

char buf[BUF_SIZE];
int main (int argc, char* argv[])
{
    int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
    size_t ret, file_size, offset = 0, tmp, tot_file_size = 0;
    void *kernel_address = NULL, *file_address = NULL;
    struct timeval start;
    struct timeval end;
    double trans_time = 0.0; //calulate the time between the device is opened and it is closed

    // read inputs
    int N;
    char file_name[MAX_FILENO][MAX_FILENAME], method[32];
    scanf("%d", &N);
    for (int file_no = 0; file_no < N; file_no++)
        scanf("%s", file_name[file_no]);
    scanf("%s", method);

    if( (dev_fd = open("/dev/master_device", O_RDWR)) < 0)
    {
        perror("failed to open /dev/master_device\n");
        return 1;
    }

    for (int file_no = 0; file_no < N; file_no++) {
        if( (file_fd = open (file_name[file_no], O_RDWR)) < 0 )
        {
            perror("failed to open input file\n");
            return 1;
        }

        if( (file_size = get_filesize(file_name[file_no])) < 0)
        {
            perror("failed to get filesize\n");
            return 1;
        }


        if(ioctl(dev_fd, 0x12345677) == -1) //0x12345677 : create socket and accept the connection from the slave
        {
            perror("ioclt server create socket error\n");
            return 1;
        }

        gettimeofday(&start ,NULL);
        switch(method[0])
        {
            case 'f': //fcntl : read()/write()
                do
                {
                    ret = read(file_fd, buf, sizeof(buf)); // read from the input file
                    write(dev_fd, buf, ret);//write to the the device
                }while(ret > 0);
                break;
            case 'm':
                kernel_address = mmap(NULL, MMAP_SIZE, PROT_WRITE, MAP_SHARED, dev_fd, 0);
                if (kernel_address == MAP_FAILED)
                    ERR_EXIT("kernel mmap");
                
                for (offset = 0; offset < file_size; ) {
                    size_t len = MMAP_SIZE < file_size - offset ? MMAP_SIZE : file_size - offset;

                    file_address = mmap(NULL, len, PROT_READ, MAP_SHARED, file_fd, offset);
                    if (file_address == MAP_FAILED)
                        ERR_EXIT("user mmap");

                    memcpy(kernel_address, file_address, len);
                    ioctl(dev_fd, master_IOCTL_MMAP, len);
                    if( munmap(file_address, len) == -1 && len > 0 ){
                        fprintf(stderr, "munmap failed\n");
                    }
                    offset += len;
                }
                break;
        }

        if(ioctl(dev_fd, 0x12345679) == -1) // end sending data, close the connection
        {
            perror("ioclt server exits error");
            return 1;
        }
        gettimeofday(&end, NULL);
        trans_time += (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.001;
        tot_file_size += file_size;
    }

    printf("Transmission time: %lf ms, File size: %lu bytes\n", trans_time, tot_file_size);

    // print page descriptor of device to dmesg
    ioctl(dev_fd, 0x88888888);

    close(file_fd);
    close(dev_fd);

    return 0;
}

size_t get_filesize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}
