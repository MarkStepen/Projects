#define LOG_TAG "MarkService"  
  
#include "jni.h"  
#include "JNIHelp.h"  
#include "android_runtime/AndroidRuntime.h"  
  
#include <utils/misc.h>  
#include <utils/Log.h>  
  
#include <stdio.h>  
  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
  
#include <sys/ioctl.h>  
  
#include <hardware/mark_hal.h>  
  
  
struct mark_device_t* mark_device;  
  
  
namespace android  
{  
    JNIEXPORT jbyteArray JNICALL markGetDatas(JNIEnv *, jclass);  
    JNIEXPORT jint JNICALL markOpen(JNIEnv *, jclass);  
    JNIEXPORT void JNICALL markClose(JNIEnv *, jclass);  
  
    /* Define a array to implement a map for Java methods and C functions */  
    static const JNINativeMethod methods[] = {  
        {"native_markGetDatas", "()[B", (void *)markGetDatas},  
        {"native_markOpen", "()I", (void *)markOpen},  
        {"native_markClose", "()V", (void *)markClose},  
    };  
  
    /* Open mark device */  
    JNIEXPORT jint JNICALL markOpen(JNIEnv *env, jclass cls)  
    {  
        jint err;  
        hw_module_t* module;  
        hw_device_t* device;  
  
        /* get HAL hw_module_t object */  
        err = hw_get_module("mark", (hw_module_t const**)&module);  
  
        if(err == 0)  
        {  
            /* call open from hw_module_t object */  
            err = module->methods->open(module, NULL, &device);  
            if(err == 0)  
            {  
                mark_device = (struct mark_device_t*)device;  
                mark_device->mark_open(mark_device);      // open mark device  
                return 0;  
            }  
            else  
            {  
                return -1;  
            }  
        }  
          
        return -1;  
    }  
  
    /* close mark device */  
    JNIEXPORT void JNICALL markClose(JNIEnv *env, jclass cls)  
    {  
        mark_device->mark_close(mark_device);  
        return;  
    }  
  
    /* get mark datas */  
    JNIEXPORT jbyteArray JNICALL markGetDatas(JNIEnv *env, jclass cls)  
    {  
        unsigned char buffer[5];  
        jbyteArray bytes = 0;  
  
        mark_device->mark_read(mark_device, buffer, 5);  
  
        bytes = env->NewByteArray(5);  
  
        if (bytes != NULL) {  
            env->SetByteArrayRegion(bytes, 0, 5, (jbyte *)buffer);  
        }  
        else  
        {  
            return NULL;  
        }  
        return bytes;  
    } 
	    /* register JNI methods to Android System */  
    int register_android_server_MarkService(JNIEnv *env)  
    {  
        return jniRegisterNativeMethods(env, "com/android/server/MarkService",  
                methods, NELEM(methods));  
    }  
  
}; 

