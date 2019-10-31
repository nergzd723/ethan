//Digital Sound Processor ports
#define DSP_RESET 0x226
#define DSP_READ 0x22A
#define DSP_WRITE 0x22C
#define DSP_BUFFER 0x22A
#define DSP_STATUS 0x22E
#define DSP_INTERRUPT 0x22F
 
//Digital Sound Processor commands
#define DSP_CMD_OUTPUT_RATE 0x41
#define DSP_CMD_TRANSFER_MODE 0xB6
#define DSP_CMD_STOP 0xD5
#define DSP_CMD_VERSION 0xE1
 
//DMA ports
#define DMA_ADDRES 0xC4
#define DMA_COUNT 0xC6
#define DMA_PAGE 0x8B
#define DMA_SINGLE_MASK 0xD4
#define DMA_TRANSFER_MODE 0xD6
#define DMA_CLEAR_POINTER 0xD8
void sb16_init(void);