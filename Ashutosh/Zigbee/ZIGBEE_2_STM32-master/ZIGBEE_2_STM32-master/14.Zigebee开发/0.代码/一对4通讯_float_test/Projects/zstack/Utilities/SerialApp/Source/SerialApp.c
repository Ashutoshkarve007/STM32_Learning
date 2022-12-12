/*********************************************************************
* INCLUDES
*/
#include <stdio.h>
#include <string.h>

#include "AF.h"
#include "OnBoard.h"
#include "OSAL_Tasks.h"
#include "SerialApp.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "hal_drivers.h"
#include "hal_key.h"
#if defined ( LCD_SUPPORTED )
#include "hal_lcd.h"
#endif
#include "hal_led.h"
#include "hal_uart.h"

#include "DHT11.h"
#include "nwk_globals.h"
/*********************************************************************
* MACROS
*/
#define COORD_ADDR   0x00
#define ED_ADDR      0x01
#define UART0        0x00
#define MAX_NODE     0x04
#define SUM_NUM      0x01
#define UART_DEBUG   0x00        //调试宏,通过串口输出协调器和终端的IEEE、短地址

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr)[0])



/*********************************************************************
* CONSTANTS
*/

#if !defined( SERIAL_APP_PORT )
#define SERIAL_APP_PORT  0
#endif

#if !defined( SERIAL_APP_BAUD )
#define SERIAL_APP_BAUD  HAL_UART_BR_115200
#endif

// When the Rx buf space is less than this threshold, invoke the Rx callback.
#if !defined( SERIAL_APP_THRESH )
#define SERIAL_APP_THRESH  64
#endif

#if !defined( SERIAL_APP_RX_SZ )
#define SERIAL_APP_RX_SZ  128
#endif

#if !defined( SERIAL_APP_TX_SZ )
#define SERIAL_APP_TX_SZ  128
#endif

// Millisecs of idle time after a byte is received before invoking Rx callback.
#if !defined( SERIAL_APP_IDLE )
#define SERIAL_APP_IDLE  6
#endif

// Loopback Rx bytes to Tx for throughput testing.
#if !defined( SERIAL_APP_LOOPBACK )
#define SERIAL_APP_LOOPBACK  FALSE
#endif

// This is the max byte count per OTA message.
#if !defined( SERIAL_APP_TX_MAX )
#define SERIAL_APP_TX_MAX  20
#endif

#define SERIAL_APP_RSP_CNT  4

// This list should be filled with Application specific Cluster IDs.
const cId_t SerialApp_ClusterList[SERIALAPP_MAX_CLUSTERS] =
{
	SERIALAPP_CLUSTERID
};

const SimpleDescriptionFormat_t SerialApp_SimpleDesc =
{
	SERIALAPP_ENDPOINT,              //  int   Endpoint;
    SERIALAPP_PROFID,                //  uint16 AppProfId[2];
    SERIALAPP_DEVICEID,              //  uint16 AppDeviceId[2];
    SERIALAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
    SERIALAPP_FLAGS,                 //  int   AppFlags:4;
    SERIALAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
    (cId_t *)SerialApp_ClusterList,  //  byte *pAppInClusterList;
    SERIALAPP_MAX_CLUSTERS,          //  byte  AppNumOutClusters;
    (cId_t *)SerialApp_ClusterList   //  byte *pAppOutClusterList;
};

const endPointDesc_t SerialApp_epDesc =
{
	SERIALAPP_ENDPOINT,
    &SerialApp_TaskID,
    (SimpleDescriptionFormat_t *)&SerialApp_SimpleDesc,
    noLatencyReqs
};

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/

uint8 SerialApp_TaskID;    // Task ID for internal task/event processing.

/*********************************************************************
* EXTERNAL VARIABLES
*/

/*********************************************************************
* EXTERNAL FUNCTIONS
*/

/*********************************************************************
* LOCAL VARIABLES
*/
static bool SendFlag = 0;

static uint8 SerialApp_MsgID;

static afAddrType_t SerialApp_TxAddr;
static afAddrType_t Broadcast_DstAddr;

static uint8 SerialApp_TxSeq;
static uint8 SerialApp_TxBuf[SERIAL_APP_TX_MAX+1];
static uint8 SerialApp_TxLen;

static afAddrType_t SerialApp_RxAddr;
static uint8 SerialApp_RspBuf[SERIAL_APP_RSP_CNT];

static devStates_t SerialApp_NwkState;
static afAddrType_t SerialApp_TxAddr;
static uint8 SerialApp_MsgID;
//---------------------------------------------------------------------

float NodesData[MAX_NODE][SUM_NUM]={{725.432},{2527.222},{125.115},{1.989}};  
uint16 EndDeviceID[MAX_NODE]={0x0001,0x0002,0x0003,0x0004};
uint16 EndDeviceID_current=0x0002;


char position1_data[4]={0};//终端1
char position2_data[4]={0};//终端2
char position3_data[4]={0};//终端3
char position4_data[4]={0};//终端4



union position1
{
  float position1_float;
  unsigned char position1_char[4];
}x1_data,y1_data,z1_data,theta1_data,phi1_data,gamma1_data;


union position2
{
float position2_float;
unsigned char position2_char[4];
}x2_data,y2_data,z2_data,theta2_data,phi2_data,gamma2_data;

union position3
{
float position3_float;
unsigned char position3_char[4];
}x3_data,y3_data,z3_data,theta3_data,phi3_data,gamma3_data;

union position4
{
float position4_float;
unsigned char position4_char[4];
}x4_data,y4_data,z4_data,theta4_data,phi4_data,gamma4_data;



//---------------------------------------------------------------------
/*********************************************************************
* LOCAL FUNCTIONS
*/

static void SerialApp_HandleKeys( uint8 shift, uint8 keys );
static void SerialApp_ProcessMSGCmd( afIncomingMSGPacket_t *pkt );
static void SerialApp_Resp(void);
static void SerialApp_CallBack(uint8 port, uint8 event);


static void AfSendAddrInfo(void);
  
static void SerialApp_SendPeriodicMessage( void );

static uint8 XorCheckSum(uint8 * pBuf, uint8 len);

/*********************************************************************
* @fn      SerialApp_Init
*
* @brief   This is called during OSAL tasks' initialization.
*
* @param   task_id - the Task ID assigned by OSAL.
*
* @return  none
*/
void SerialApp_Init( uint8 task_id )
{
    halUARTCfg_t uartConfig;
    
  
	
	SerialApp_TaskID = task_id;
	
	
	afRegister( (endPointDesc_t *)&SerialApp_epDesc );
	
	RegisterForKeys( task_id );
	
	uartConfig.configured           = TRUE;              // 2x30 don't care - see uart driver.
	uartConfig.baudRate             = SERIAL_APP_BAUD;
	uartConfig.flowControl          = FALSE;
	uartConfig.flowControlThreshold = SERIAL_APP_THRESH; // 2x30 don't care - see uart driver.
	uartConfig.rx.maxBufSize        = SERIAL_APP_RX_SZ;  // 2x30 don't care - see uart driver.
	uartConfig.tx.maxBufSize        = SERIAL_APP_TX_SZ;  // 2x30 don't care - see uart driver.
	uartConfig.idleTimeout          = SERIAL_APP_IDLE;   // 2x30 don't care - see uart driver.
	uartConfig.intEnable            = TRUE;              // 2x30 don't care - see uart driver.
	uartConfig.callBackFunc         = SerialApp_CallBack;
	HalUARTOpen (UART0, &uartConfig);
	
#if defined ( LCD_SUPPORTED )
	HalLcdWriteString( "SerialApp", HAL_LCD_LINE_2 );
#endif
	
}

/*********************************************************************
* @fn      SerialApp_ProcessEvent
*
* @brief   Generic Application Task event processor.
*
* @param   task_id  - The OSAL assigned task ID.
* @param   events   - Bit map of events to process.
*
* @return  Event flags of all unprocessed events.
*/
UINT16 SerialApp_ProcessEvent( uint8 task_id, UINT16 events )
{
	(void)task_id;  // Intentionally unreferenced parameter
	
	if ( events & SYS_EVENT_MSG )//输入消息处理
	{
		afIncomingMSGPacket_t *MSGpkt;
		
		while ( (MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SerialApp_TaskID )) )
		{
			switch ( MSGpkt->hdr.event )//输入
			{
		
				
			case KEY_CHANGE:
				SerialApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
				break;
				
			case AF_INCOMING_MSG_CMD://从其他节点应用层发给自己的数据包，可以改
				SerialApp_ProcessMSGCmd( MSGpkt );
				break;
                
            case ZDO_STATE_CHANGE://设备网络状态，协调器、路由器和
              SerialApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
              if ( (SerialApp_NwkState == DEV_ZB_COORD)
                  || (SerialApp_NwkState == DEV_ROUTER)
                  || (SerialApp_NwkState == DEV_END_DEVICE) )
              {
                #if defined(ZDO_COORDINATOR) //协调器通过串口输出自身短地址、IEEE  
                    Broadcast_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;
                    Broadcast_DstAddr.endPoint = SERIALAPP_ENDPOINT;
                    Broadcast_DstAddr.addr.shortAddr = 0xFFFF;
                
                 
                #else                        //终端无线发送短地址、IEEE   
                    AfSendAddrInfo();
                #endif
                
              }
              break;				
			default:
				break;
			}
			
			osal_msg_deallocate( (uint8 *)MSGpkt );
		}
		
		return ( events ^ SYS_EVENT_MSG );
	}
    
    //在此事件中可以定时向协调器发送节点传感器参数信息
        
     if ( events & SERIALAPP_SEND_PERIODIC_EVT )
    {
        SerialApp_SendPeriodicMessage();
        
        osal_start_timerEx( SerialApp_TaskID, SERIALAPP_SEND_PERIODIC_EVT,
            (SERIALAPP_SEND_PERIODIC_TIMEOUT + (osal_rand() & 0x00FF)) );
        
        return (events ^ SERIALAPP_SEND_PERIODIC_EVT);
    }

	
    if ( events & SERIALAPP_RESP_EVT )
	{
		SerialApp_Resp();
		return ( events ^ SERIALAPP_RESP_EVT );
	}
	
	return ( 0 ); 
}

/*********************************************************************
* @fn      SerialApp_HandleKeys
*
* @brief   Handles all key events for this device.
*
* @param   shift - true if in shift/alt.
* @param   keys  - bit field for key events.
*
* @return  none
*/
void SerialApp_HandleKeys( uint8 shift, uint8 keys )
{
#if defined(ZDO_COORDINATOR)//协调器
	
    if ( keys & HAL_KEY_SW_6 ) //按S1键启动或停止终端定时上报数据 
    {
      if(SendFlag == 0)
        {
        SendFlag = 1;
      
        osal_start_timerEx( SerialApp_TaskID,
                            SERIALAPP_SEND_PERIODIC_EVT,
                            SERIALAPP_SEND_PERIODIC_TIMEOUT );
        }
        else
        {      
            SendFlag = 0;
           
            osal_stop_timerEx(SerialApp_TaskID, SERIALAPP_SEND_PERIODIC_EVT);
        }
    }
    
#endif
}

void SerialApp_ProcessMSGCmd( afIncomingMSGPacket_t *pkt )
{

   
    uint8 afRxData[26]={0};
    uint8 getData[4]={0};
    uint8 i;
    
	switch ( pkt->clusterId )
	{

	case SERIALAPP_CLUSTERID:
        osal_memcpy(afRxData, pkt->cmd.Data, pkt->cmd.DataLength);
		switch(afRxData[0]) 
		{
#if defined(ZDO_COORDINATOR)//协调器，接收数据
		
         
                 
#else  //终端，接收到数据,打印协调器数据
        case 0x3A:  //开始符，终端接收到的数据	      
          
              if(EndDeviceID_current == afRxData[2])//验证终端设备地址,0x01
		    {
                      for (i=0;i<4;i++)
                      {
                        getData[i]=afRxData[3+i];
                      }

                           HalUARTWrite(UART0, getData,4);                 
                    }  
              else if(EndDeviceID_current == afRxData[8])//验证终端设备地址,0x01
		    {
		            for (i=0;i<4;i++)
                      {
                        getData[i]=afRxData[9+i];
                      }
                           HalUARTWrite(UART0, getData,4);                 
                    }
             else if(EndDeviceID_current == afRxData[14])//验证终端设备地址,0x01
		    {
		            for (i=0;i<4;i++)
                      {
                        getData[i]=afRxData[15+i];
                      }
                           HalUARTWrite(UART0, getData,4);                 
                    }
              else if(EndDeviceID_current == afRxData[20])//验证终端设备地址,0x01
		    {
		            for (i=0;i<4;i++)
                      {
                        getData[i]=afRxData[21+i];
                      }
                           HalUARTWrite(UART0, getData,4);                 
                    }
              
          	
        break;
        	
#endif
       
        }
        break;
			
		
	}
}

uint8 TxBuffer[26];


void SerialApp_SendPeriodicMessage( void )//终端定时发给协调器
{ 
   
     
     TxBuffer[0] = 0x3A;   
     
     TxBuffer[1] = 0x00;
     TxBuffer[2] = EndDeviceID[0];
   
     
     TxBuffer[7] = 0x00;
     TxBuffer[8] = EndDeviceID[1];
    
     
     TxBuffer[13] = 0x00;
     TxBuffer[14] = EndDeviceID[2];
     
     TxBuffer[19] = 0x00;
     TxBuffer[20] = EndDeviceID[3];
     

          x1_data.position1_float=NodesData[0][0];
        
         
          x2_data.position2_float=NodesData[1][0];
         
          
          x3_data.position3_float=NodesData[2][0];
         
        
          x4_data.position4_float=NodesData[3][0];
        
 
        for (uint8 i=0;i<4;i++)
    
        {
               TxBuffer[3+i]=x1_data.position1_char[i];
	      
               
               TxBuffer[9+i]=x2_data.position2_char[i];
	      
               TxBuffer[15+i]=x3_data.position3_char[i];
	   
               
               TxBuffer[21+i]=x4_data.position4_char[i];
	

       
             } 
       
         
	TxBuffer[25] =0x23;

       
        
        HalUARTWrite(UART0, TxBuffer,26);		
                
        AF_DataRequest(&Broadcast_DstAddr,
                       (endPointDesc_t *)&SerialApp_epDesc,
                        SERIALAPP_CLUSTERID,
                        26, 
                        TxBuffer,
                        &SerialApp_MsgID, 
                        0, 
                        AF_DEFAULT_RADIUS);
           
		 
      
}



/*********************************************************************
* @fn      SerialApp_Resp
*
* @brief   Send data OTA.
*
* @param   none
*
* @return  none
*/
static void SerialApp_Resp(void)
{
	if (afStatus_SUCCESS != AF_DataRequest(&SerialApp_RxAddr,
		(endPointDesc_t *)&SerialApp_epDesc,
		SERIALAPP_CLUSTERID2,
		SERIAL_APP_RSP_CNT, SerialApp_RspBuf,
		&SerialApp_MsgID, 0, AF_DEFAULT_RADIUS))
	{
		osal_set_event(SerialApp_TaskID, SERIALAPP_RESP_EVT);
	}
}

/*********************************************************************
* @fn      SerialApp_CallBack
*
* @brief   Send data OTA.
*
* @param   port - UART port.
* @param   event - the UART port event flag.
*
* @return  none
*/
static void SerialApp_CallBack(uint8 port, uint8 event)
{
	(void)port;
	
	if ((event & (HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL | HAL_UART_RX_TIMEOUT)) &&
#if SERIAL_APP_LOOPBACK
		(SerialApp_TxLen < SERIAL_APP_TX_MAX))
#else
		!SerialApp_TxLen)
#endif
	{
		SerialApp_SendPeriodicMessage();
	}
}






void AfSendAddrInfo(void)
{
    uint16 shortAddr;
    uint8 strBuf[11]={0};  
    
    SerialApp_TxAddr.addrMode = (afAddrMode_t)Addr16Bit;
    SerialApp_TxAddr.endPoint = SERIALAPP_ENDPOINT;
    SerialApp_TxAddr.addr.shortAddr = 0x00;   
    
    shortAddr=NLME_GetShortAddr();
    
    strBuf[0] = 0x3B;                          //发送地址给协调器 可用于点播
    strBuf[1] = HI_UINT16( shortAddr );        //存放短地址高8位
    strBuf[2] = LO_UINT16( shortAddr );        //存放短地址低8位
    
    osal_memcpy(&strBuf[3], NLME_GetExtAddr(), 8);
        
   if ( AF_DataRequest( &SerialApp_TxAddr, (endPointDesc_t *)&SerialApp_epDesc,
                       SERIALAPP_CLUSTERID,
                       11,
                       strBuf,
                       &SerialApp_MsgID, 
                       0, 
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }   
}











/*********************************************************************
*********************************************************************/
