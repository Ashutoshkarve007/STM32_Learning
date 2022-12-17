#define TRUE   1 
#define FALSE  0 
#define bool BYTE 

#include "stm32f1xx_hal.h" 

#include "diskio.h" 
#include "fatfs_sd.h" 

#define SD_CS_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_5


extern SPI_HandleTypeDef hspi1 ;  
extern volatile uint8_t Timer1 , Timer2 ;                     /* timer that decrements every 10 ms */ 

static volatile DSTATUS Stat =  STA_NOINIT ;               /* Disk Status Flag*/ 
static uint8_t CardType ;                                 /* SD type 0:MMC, 1:SDC, 2:Block addressing */ 
static uint8_t PowerFlag =  0 ;                           /* Power Status Flag */ 

/* SPI Chip Select */ 
static  void  SELECT ( void ) 
{ 
  HAL_GPIO_WritePin ( SD_CS_GPIO_Port , SD_CS_Pin ,  GPIO_PIN_RESET ) ; 
} 

/* SPI Chip Deselect */ 
static  void  DESELECT ( void ) 
{ 
  HAL_GPIO_WritePin ( SD_CS_GPIO_Port , SD_CS_Pin ,  GPIO_PIN_SET ) ; 
} 

/* Send SPI data */ 
static  void  SPI_TxByte ( BYTE data )
{ 
  while  ( HAL_SPI_GetState ( & hspi1 )  !=  HAL_SPI_STATE_READY ) ; 
  HAL_SPI_Transmit ( & hspi1 ,  & data ,  1 ,  SPI_TIMEOUT ) ; 
} 

/* Return type function for sending and receiving SPI data */ 
static uint8_t SPI_RxByte ( void ) 
{ 
  uint8_t dummy , data ; 
  dummy =  0xFF ; 
  data =  0 ; 
  
  while  ( (HAL_SPI_GetState ( & hspi1 )  !=  HAL_SPI_STATE_READY ) ) ; 
  HAL_SPI_TransmitReceive ( & hspi1 ,  & dummy ,  & data ,  1 ,  SPI_TIMEOUT ) ; 
  
  return data ; 
} 

/* Pointer function for sending and receiving SPI data */ 
static  void  SPI_RxBytePtr ( uint8_t *buff )  
{ 
  * buff =  SPI_RxByte ( ) ; 
} 

/* Wait for SD card to be ready */ 
static uint8_t SD_ReadyWait ( void )  
{ 
  uint8_t res ; 
  
  /* Prepare 500 ms counter */ 
  Timer2 =  50 ; 
  SPI_RxByte ( ) ; 
  
  do 
  { 
    /* SPI communication until 0xFF value is received */ 
    res =  SPI_RxByte ( ) ; 
  }  while  ( ( res !=  0xFF )  && Timer2 ) ; 
  
  return res ; 
} 

/* Power On */ 
static  void  SD_PowerOn ( void ) 
{ 
  uint8_t cmd_arg [ 6 ] ; 
  uint32_t Count =  0x1FFF ; 
  
  /* In the deselect state, send an SPI message to make it stand-by. */ 
  DESELECT ( ) ; 
  
  for ( int i =  0 ; i <  10 ; i ++ ) 
  { 
    SPI_TxByte ( 0xFF ) ; 
  } 
  
  /* SPI Chips Select */ 
  SELECT ( ) ; 
  
  /* Initial GO_IDLE_STATE state transition */ 
  cmd_arg [ 0 ]  =  ( CMD0 |  0x40 ) ; 
  cmd_arg [ 1 ]  =  0 ; 
  cmd_arg [ 2 ]  =  0 ; 
  cmd_arg [ 3 ]  =  0 ; 
  cmd_arg [ 4 ]  =  0 ; 
  cmd_arg [ 5 ]  =  0x95 ; 
  
  /* Send command */ 
  for  ( int i =  0 ; i <  6 ; i ++ ) 
  { 
    SPI_TxByte( cmd_arg [ i ] ) ; 
  } 
  
  /* Wait for response */ 
  while  ( ( SPI_RxByte ( )  !=  0x01 )  && Count ) 
  { 
    Count -- ; 
  } 
  
  DESELECT ( ) ; 
  SPI_TxByte ( 0XFF ) ; 
  
  PowerFlag =  1 ; 
} 

/* Power off */ 
static  void  SD_PowerOff ( void )  
{ 
  PowerFlag =  0 ;
} 

/* Check power state */ 
static uint8_t SD_CheckPower ( void )  
{ 
  /* 0=off, 1=on */ 
  return PowerFlag ; 
} 

/* Receive data packet */ 
static bool SD_RxDataBlock ( BYTE *buff, UINT btr )  
{ 
  uint8_t token ; 
  
  /* 100 ms timer */ 
  Timer1 =  10 ; 
  
  /* Wait for response */ 		
  do  
  {     
    token =  SPI_RxByte ( ) ; 
  }  while ( ( token ==  0xFF ) && Timer1 ) ; 
  
  /* Error handling when receiving tokens other than 0xFE */ 
  if ( token !=  0xFE ) 
    return  FALSE ; 
  
  /* Receive data in buffer */ 
  do  
  {      
    SPI_RxBytePtr ( buff ++ ) ; 
    SPI_RxBytePtr ( buff ++ ) ; 
  }  while ( btr -=  2 ) ; 
  
  SPI_RxByte ( ) ;  /* Ignore CRC */ 
  SPI_RxByte ( ) ; 
  
  return  TRUE ; 
}

/* data transfer packet */ 
# if _READONLY ==  0 
static bool SD_TxDataBlock ( const BYTE *buff, BYTE token ) 
{ 
  uint8_t resp , wc ; 
  uint8_t i =  0 ; 
    
  /* Wait for SD card ready */ 
  if  ( SD_ReadyWait ( )  !=  0xFF ) 
    return  FALSE ; 
  
  /* Token transfer */ 
  SPI_TxByte ( token ) ;       
  
  /* if it is a data token */ 
  if  ( token !=  0xFD )  
  { 
    wc =  0 ; 
    
    /* Send 512 bytes of data */ 
    do  
    {  
      SPI_TxByte ( * buff ++ ) ; 
      SPI_TxByte ( * buff ++ ) ; 
    }  while  ( -- wc ) ; 
    
    SPI_RxByte ( ) ;        /* Ignore CRC */ 
    SPI_RxByte ( ) ; 
    
    /* Receive data response */         
    while  ( i <=  64 )  
    { 			
      resp =  SPI_RxByte ( ) ;
      
      /* handle error response */ 
      if  ( ( resp &  0x1F )  ==  0x05 )  
        break ; 
      
      i ++ ; 
    } 
    
    /* Clear the SPI receive buffer */ 
    while  ( SPI_RxByte ( )  ==  0 ) ; 
  } 
  
  if  ( ( resp &  0x1F )  ==  0x05 ) 
    return  TRUE ; 
  else 
    return  FALSE ; 
} 
#endif /* _READONLY */
