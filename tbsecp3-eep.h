
#include <stdbool.h>
#include <unistd.h>
#include <linux/dvb/frontend.h>

typedef unsigned char u8;
typedef unsigned int u32; 

extern int  tunerFd;

/*typedef struct eeprom_info
{
	u8 reg;
	u8 data;
} eeprom_info;
*/

bool tbs_write(u8 address,u8 data);
u8 tbs_read(u8 address);

bool lin_tbs_eep_wr(u8 address,u8 data);
u8  lin_tbs_eep_rd(u8 address);
