#ifndef _HARDWARE_DHT11_HAL_H  
#define _HARDWARE_DHT11_HAL_H  
  
#include <hardware/hardware.h>  
  
__BEGIN_DECLS  
  
  
struct dht11_device_t{  
  
    struct hw_device_t common;  
  
    void (*dht11_close)(struct dht11_device_t *);  
    int (*dht11_open)(struct dht11_device_t *);  
    int (*dht11_read)(struct dht11_device_t *, unsigned char *, int);  
};  
  
  
  
__END_DECLS  




#endif

