1,实现读卡器功能，用cubemx生成USB DEVICE MMC代码后，在usbd_xxxxx_if.c添加SD卡读写函数，并且修改usbd_conf.h	
		usbd_conf.h修改 
		
		#define MSC_MEDIA_PACKET     32768
		
		#define USBD_malloc(x)               mymalloc(SRAMIN,x)

		#define USBD_free(x)                 myfree(SRAMIN,x)

2,有FATFS文件操作系统，可用来读写内存卡以及spi flash
