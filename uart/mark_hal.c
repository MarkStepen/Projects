#define LOG_TAG "markHal"  
  
#include <hardware/mark_hal.h>  
#include <hardware/hardware.h>  
  
#include <cutils/log.h>  
  
#include <stdio.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <math.h>  
  
#include <utils/Log.h>  
  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
  
#include <sys/ioctl.h>  
#include <unistd.h>  
  
static int fd;  
  
static void mark_close(struct mark_device_t *dev)  
  
{  
    close(fd);  
    ALOGI("mark_close fd : %d", fd);  
    return;  
}  
  
static int mark_open(struct mark_device_t *dev)  
{  
    fd = open("/dev/ttySAC2", O_RDWR);  
  
    ALOGI("mark_open fd : %d", fd);  
  
    return 0;  
}  
  
  
static int mark_read(struct mark_device_t *dev, unsigned char *buffer, int count)  
{  
    int ret = read(fd, buffer, count);  
    ALOGI("mark_read");  
    return ret;  
}  
  
  
  
struct mark_device_t mark_dev = {  
    .common     = {  
        .tag = HARDWARE_DEVICE_TAG,   
    },  
    .mark_close    = mark_close,  
    .mark_open     = mark_open,  
    .mark_read     = mark_read,  
};  
  
  
static int mark_device_open(const struct hw_module_t* module, const char* id, struct hw_device_t** device)  
{  
    *device = &mark_dev;  
    return 0;  
}  
  
static struct hw_module_methods_t mark_module_methods = {  
    .open = mark_device_open,  
};  
  
struct hw_module_t HAL_MODULE_INFO_SYM = {  
    .id      = "mark",  
    .tag     = HARDWARE_MODULE_TAG,       
    .methods = &mark_module_methods,  
};  