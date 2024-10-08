#include <stdint.h>
#include "stdio.h"
#include "x86.h"
#include "disk.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

void __attribute__((cdecl)) start(uint16_t bootDrive)
{
    clrscr();

    printf("MinimOS Bootloader Stage 2\r\n");

    printf("Initializing disk...\r\n");
    
    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    }

    printf("Initializing FAT...\r\n");

    if (!FAT_Initialize(&disk))
    {
        printf("FAT init error\r\n");
        goto end;
    }

    // FAT_File* root = FAT_Open(&disk, "/");

    // FAT_DirectoryEntry entry;
    // while (FAT_ReadEntry(&disk, root, &entry))
    // {
    //     printf("  ");
    //     for (int i = 0; i < 11; i++)
    //         putc(entry.Name[i]);

    //     printf("\r\n");
    // }
    // FAT_Close(root);


    printf("Loading kernel...\r\n");

    // load kernel
    FAT_File* fd = FAT_Open(&disk, "/kernel.bin");
    uint32_t read;
    uint8_t* kernelBuffer = Kernel;
    while ((read = FAT_Read(&disk, fd, MEMORY_LOAD_SIZE, KernelLoadBuffer)))
    {
        memcpy(kernelBuffer, KernelLoadBuffer, read);
        kernelBuffer += read;
    }
    FAT_Close(fd);

    printf("Bootloader complete!\r\n");
    printf("Executing kernel...\r\n");

    // execute kernel
    KernelStart kernelStart = (KernelStart)Kernel;
    kernelStart(bootDrive);

end:
    printf("Halted.\r\n");
    for (;;);
}
