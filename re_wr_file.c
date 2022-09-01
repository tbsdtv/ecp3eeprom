#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <linux/fs.h>

#include "tbsecp3-eep.h"

const char *R_FILE_NAME = NULL;   
const char *W_FILE_NAME = NULL;  
unsigned char file_buffer[256]; 
int i_adapter = -1;
int i_fenum =0;
int tunerFd =-1;
int i_read = 0;
int i_write = 0;
static const struct option long_options[] =
{
{ "adapter",         required_argument, NULL, 'a' },
{ "frontend-number", required_argument, NULL, 'n' },
{ "read", 					 required_argument, NULL, 'r' },
{ "write", 					 required_argument, NULL, 'w' },
{ "help",            no_argument,       NULL, 'h' },
{ 0, 0, 0, 0 }
};


void usage()
{
    printf("Usage: ecp3_eeprom -a 0 -n 0 -w yyy.bin\n");
    printf("Usage: ecp3_eeprom -a 0 -n 0 -w xxx.bin -r yyy.bin\n");
    printf("\n");
    exit(1);
}     

static void Read_Eeprom()
{
    u32 addr;
	u8 val;
    FILE * r_file_fd;
    r_file_fd = fopen(R_FILE_NAME,"wb");
    fseek(r_file_fd,0,SEEK_SET);
    if(r_file_fd == NULL)
    {
        printf("errno open file!!!\n");
        exit(1);
    }
    else
    {
        printf("File Open successed.\n");
    }
    
    for(addr=0;addr<256;addr++)
    {
		val = tbs_read(addr);
        fwrite(&val,1, 1, r_file_fd);
    }
    printf("read flash successed.\n");
    fclose(r_file_fd);
}


static void Write_Eeprom()
{
    int m_eraseNum = 0;
    FILE *w_file_fd;
    u8 data[256];
    u32 addr;
    int read_len;
    int ret;

    w_file_fd = fopen(W_FILE_NAME,"rb+");
    if(w_file_fd == NULL)
    {
        printf("errno open file !!!\n");
        exit(1);
    }
    else
        printf("File Open successed.\n");
	
	read_len = fread(data, 1, 256, w_file_fd);
	if(read_len == -1)
	{
		printf("File read error!!!\n");
		fclose(w_file_fd);
		exit(0);
	}
	else if(read_len < 256)
	{
		printf("File read Over!!!\n");
		fclose(w_file_fd);
		exit(0);
	}

	fclose(w_file_fd);
   
    for(addr=0;addr<256;addr++)
	{
		tbs_write(addr, data[addr]);
		usleep(5000);
	}

	printf("write flash successed.\n");
}  


int main( int i_argc, char **pp_argv )
{  
    int c;
    char psz_tmp[128];
    FILE * r_file_fd;
    FILE * w_file_fd;
    int read_len ,i;

    if(i_argc == 1)
        usage();
    while ( (c = getopt_long(i_argc, pp_argv, "a:n:r:w:", long_options, NULL)) != -1 )
    {
        switch ( c )
        {
        case 'a':
            i_adapter = strtol( optarg, NULL, 0 );
            break;
        case 'n':
            i_fenum = strtol( optarg, NULL, 0 );
            break;
        case 'r':
            i_read=1;
            R_FILE_NAME = optarg;
            break;
        case 'w':
            i_write=1;
            W_FILE_NAME = optarg;
            break;
        case 'h':
        default:
            usage();
        }
    }

    if(i_adapter < 0 )
        usage();

#if 0
	printf("i_adapter=%d,i_fenum=%d\n",i_adapter,i_fenum);
	printf("i_read=%d, R_FILE_NAME=%s\n",i_read,R_FILE_NAME);
	printf("i_write=%d, W_FILE_NAME=%s\n",i_write,W_FILE_NAME);
#endif

    sprintf( psz_tmp, "/dev/dvb/adapter%d/frontend%d", i_adapter, i_fenum );
    if( (tunerFd = open(psz_tmp, O_RDWR | O_NONBLOCK)) < 0 )
    {
        printf( "opening device %s failed (%s)", psz_tmp,strerror(errno) );
        exit(1);
    }

    if(i_write == 1)
        Write_Eeprom();

    if(i_read == 1)
        Read_Eeprom();

    if(i_write == 0 && i_read == 0)
        usage();

    return 0;
}   
