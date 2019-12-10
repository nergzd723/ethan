#include <smbios.h>
#include <string.h>

bool smbios_detected=false;
uint16_t cpuMHZ;
const char *machine_bios_vendor ;
const char *machine_bios_version;
const char *machine_bios_bdate  ;
struct SMBIOSEntryPoint *smb_ep;
void detect_SM_(){
   unsigned char *mem = (unsigned char *) 0xF0000;
   int length, i;
   unsigned char checksum;
	while ((unsigned int) mem < 0x100000)
	{
		if (memcmp(mem,"_SM_",4)==0)
		{
			length = mem[5];
			checksum = 0;
			for(i = 0; i < length; i++)
			{
				checksum += mem[i];
			}
			if(checksum == 0) break;
		}
		mem += 16;
	}
	if ((unsigned int) mem == 0x100000)
	{
		logf("error: SMBIOS not found!");
		
	}
	smb_ep = (struct SMBIOSEntryPoint*)mem;
}
uint16_t detectCPUSpeed()
{
    SMBIOSHeader *shdr = (SMBIOSHeader*)smb_ep->TableAddress;
    uint8_t type;
    while(1)
    {
        type=shdr->Type;
        if(type==4) break;
        uint16_t length = shdr->Length;
        shdr+=4;
        shdr+=length;
    }
    uint8_t *procstruct=(uint8_t*)shdr;
    procstruct+=0x16;
    cpuMHZ=*(uint16_t*)procstruct;
    shdr = (SMBIOSHeader*)smb_ep->TableAddress; //Reset it back.
    return cpuMHZ;
}

void *smbios_getTblPtrByID(int id)
{
    uint8_t* shdr = (uint8_t*)(((struct SMBIOSEntryPoint*)(smb_ep))->TableAddress);
    uint8_t type;
    while(1)
    {
        type=((SMBIOSHeader *)shdr)->Type;
        if(type==id) break;
        uint16_t length = ((SMBIOSHeader *)shdr)->Length;
        shdr+=1;
        shdr+=length;
    }
    return (void*)shdr;
}
const char *detectBIOSVendor()
{    
    const char *BIOSVendor=(const char*)smbios_getTblPtrByID(0);
    BIOSVendor+=(*(BIOSVendor+20)>='A'&&*(BIOSVendor+20)<='z') ? 20 : 24;
    //shdr = (SMBIOSHeader*)smb_ep->TableAddress; //Reset it back.
    return BIOSVendor;
}
const  char * detectBIOSVersion()
{
    return detectBIOSVendor() + strlen(detectBIOSVendor()) + 1;
}
const  char * detectBIOSBDate  ()
{
    return detectBIOSVendor() + strlen(detectBIOSVendor()) + 1 + strlen(detectBIOSVersion()) + 1;
}
