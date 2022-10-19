# SD card using SPI in STM32

 interface a SD card with stm32 micro controller using SPI mode.
 
 CubeMx SetUp

![sd_card_spi_1](https://user-images.githubusercontent.com/89137956/196679765-dbf35cf2-b079-41eb-be44-3cf75de33314.png)

Make sure you select the fatfs and I have left everything to default. I am also selecting uart in my project so that the data can be sent to the computer.

![sd_card_spi_2 (1)](https://user-images.githubusercontent.com/89137956/196680064-bb50cc97-6d91-482c-a986-13a3f6ca3dbb.png)

Also the SPI speed is kept at 2.25 MBits/s. This is important, make sure you keep your speed around it.


>> Edit below section in intrupt.

```
volatile uint8_t FatFsCnt = 0;
volatile uint8_t Timer1, Timer2;

void SDTimer_Handler(void)
{
  if(Timer1 > 0)
    Timer1--;

  if(Timer2 > 0)
    Timer2--;
}



void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

	  FatFsCnt++;
	  if(FatFsCnt >= 10)
	  {
	    FatFsCnt = 0;
	    SDTimer_Handler();
	  }

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
```

# Some Insight in the Code

f_mount will mount the SD card. “/” is the path, that we want to mount. fs is the pointer to the file system, which we need to define as a global definition, as we are going to use it a lot.

```
fresult = f_mount(&fs, "/", 1);
if (fresult != FR_OK) send_uart ("ERROR!!! in mounting SD CARD...\n\n");
else send_uart("SD CARD mounted successfully...\n\n");
```

Once the card is mounted, we will create a file and to do that, we will use the following function

```
/* Create second file with read write access and open it */
fresult = f_open(&fil, "file2.txt", FA_CREATE_ALWAYS | FA_WRITE);
```
If the file already exists, this will override it, and if it doesn’t exist, it will create a new file and than open it.

fil is the pointer to the file, which is also global.
“file2.txt” is the name of the file, that we want to create
the third parameter is the attributes given to the file. Below is the table for that

![f_open](https://user-images.githubusercontent.com/89137956/196681034-e2b759c3-9105-411f-a2d2-a4c759fe9747.png)

once the file is opened, we can write the data using the function below

```
/* Writing text */
strcpy (buffer, "This is File2.txt, written using ...f_write... and it says Hello from Controllerstech\n");

fresult = f_write(&fil, buffer, bufsize(buffer), &bw);
```

here bw is the variable to keep the count of the number of bytes written to the file.
buffer contains the string, that we want to write into the file.

After writing, it’s time to close the file, and to do that we can use

```
/* Close file */
f_close(&fil);
```
Now it’s time to read the data, that we just wrote into the file. So, let’s open it again with read attribute

```
/* Open second file to read */
fresult = f_open(&fil, "file2.txt", FA_READ);
if (fresult == FR_OK)send_uart ("file2.txt is open and the data is shown below\n");

/* Read data from the file
 * Please see the function details for the arguments 
*/
f_read (&fil, buffer, f_size(&fil), &br);
send_uart(buffer);
send_uart("\n\n");

/* Close file */
f_close(&fil);
```
The file opened, read, and data from the file got saved into the buffer. bw is the count of the number of bytes read from the file.
Than we sent the buffer to the uart, and at last close the file.

To delete the above created file use the function below

```
fresult = f_unlink("/file2.txt");
if (fresult == FR_OK) send_uart("file2.txt removed successfully...\n");
```

so, f_unlink will delete the file.
“/file2.txt” is the path to the file that we want to delete

And finally we will unmount the CARD using the f_mount function again. But this time the file system pointer will be NULL.

```
/* Unmount SDCARD */
fresult = f_mount(NULL, "/", 1);
if (fresult == FR_OK) send_uart ("SD CARD UNMOUNTED successfully...\n");
```



sd Card SPI Connection

![connections](https://user-images.githubusercontent.com/89137956/196682431-ba0a36bf-2f36-4e17-95bd-1c7ffe338a9c.png)

