#include <string.h>
#include <timer.h>
#include <frame_buffer.h>
#include <bios32.h>
#include <pic.h>

void vesa_set_mode(uint32_t mode) {
    register16_t reg_in = {0};
    register16_t reg_out = {0};
    reg_in.ax = 0x0013;
    bios32_service(BIOS_GRAPHICS_SERVICE, &reg_in, &reg_out);
}

//test
void gfx_test()
{
    vesa_set_mode(0x144 | 0x4000);
    pic_send_eoi(0x28);
    return;
}