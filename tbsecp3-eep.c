#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>


#include "tbsecp3-eep.h"

bool tbs_write(u8 address, u8 data)
{

    return lin_tbs_eep_wr(address,data);
}

u8 tbs_read(u8 address)
{
    return lin_tbs_eep_rd(address);
}

bool lin_tbs_eep_wr(u8 address, u8 data)
{
    struct eeprom_info winfo;
    winfo.reg=address;
    winfo.data=data;
    if( ioctl(tunerFd, FE_EEPROM_WRITE, &winfo ) < 0 )
    {
        printf("write eror %s\n ",strerror(errno));
        exit(1);
    }
    return 0;
}


u8 lin_tbs_eep_rd(u8 address)
{
    struct eeprom_info rinfo;
    rinfo.reg = address;
    if( ioctl( tunerFd, FE_EEPROM_READ, &rinfo ) < 0 )
    {
        printf("write eror %s\n ",strerror(errno));
        exit(1);
    }
    return rinfo.data;
}

