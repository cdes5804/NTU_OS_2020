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

#define slave_IOCTL_CREATESOCK 0x12345677
#define slave_IOCTL_MMAP 0x12345678
#define slave_IOCTL_EXIT 0x12345679

#define ERR_EXIT(msg) { perror(msg); exit(-1); }

char buf[BUF_SIZE];
int main (int argc, char* argv[])
{
    int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
    size_t ret, file_size = 0, data_size = -1, tot_file_size = 0;
    char ip[20];
    struct timeval start;
    struct timeval end;
    double trans_time = 0.0; //calulate the time between the device is opened and it is closed
    void *kernel_address, *file_address;

    // read inputs
    int N;
    char file_name[MAX_FILENO][MAX_FILENAME], method[32];
    scanf("%d", &N);
    for (int file_no = 0; file_no < N; file_no++)
        scanf("%s", file_name[file_no]);
    scanf("%s", method);
    scanf("%s", ip);

    if( (dev_fd = open("/dev/slave_device", O_RDWR)) < 0)//should be O_RDWR for PROT_WRITE when mmap()
    {
        perror("failed to open /dev/slave_device\n");
        return 1;
    }

    for (int file_no = 0; file_no < N; file_no++) {
        file_size = 0;
        gettimeofday(&start ,NULL);
        if( (file_fd = open (file_name[file_no], O_RDWR | O_CREAT | O_TRUNC)) < 0)
        {
            perror("failed to open input file\n");
            return 1;
        }

        if(ioctl(dev_fd, 0x12345677, ip) == -1)    //0x12345677 : connect to master in the device
        {
            perror("ioclt create slave socket error\n");
            return 1;
        }

        //write(1, "ioctl success\n", 14);

        switch(method[0])
        {
            case 'f'://fcntl : read()/write()
                do
                {
                    ret = read(dev_fd, buf, sizeof(buf)); // read from the the device
                    write(file_fd, buf, ret); //write to the input file
                    file_size += ret;
                }while(ret > 0);
                break;
            case 'm':
                kernel_address = mmap(NULL, MMAP_SIZE, PROT_READ, MAP_SHARED, dev_fd, 0);
                if (kernel_address == MAP_FAILED)
                    ERR_EXIT("kernel mmap");
                
                for (size_t offset = 0;;) {
                    size_t len = ioctl(dev_fd, slave_IOCTL_MMAP);
                    if (len == 0)
                        break;

                    // extend the file size
                    file_size = offset + len;
                    posix_fallocate(file_fd, offset, len);
                    file_address = mmap(NULL, len, PROT_WRITE, MAP_SHARED, file_fd, offset);
                    if (file_address == MAP_FAILED)
                        ERR_EXIT("user mmap");

                    memcpy(file_address, kernel_address, len);
                    if( munmap(file_address, len) == -1 && len > 0 ){
                        fprintf(stderr, "unmapped failed\n");
                    }
                    offset += len;
                }
                ftruncate(file_fd, file_size);
                break;
        }

        if(ioctl(dev_fd, 0x12345679) == -1)// end receiving data, close the connection
        {
            perror("ioclt client exits error\n");
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
