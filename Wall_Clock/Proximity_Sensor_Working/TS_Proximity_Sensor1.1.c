/*******************************************************************************
 * TS_Proximity_Sensor.c
 *
 *  Created on: March 3, 2014
 *      Author: libin.eb
 *******************************************************************************/
//******************************************************************************
//                              /|\ /|\
//                   TMD2721    1.5k 1.5k     CC430F5137
//                   slave       |   |        	master
//           -----------------   |   |	 ----------------- 		----------------
//         -|		  	   |<-|------+->|P1.3/UCB0SDA     |		|				|
//          |                 |  |   	|     P2.3/UCB0SDA|<--->|				|
//         -|	              |  |   	|           	  |		|				|
//          |     		      |<-+----->|P1.2/UCB0SCL     |		|				|
//          |                 |         |             	  |		|				|
//          |                 |         |        P2.6/SCL |<--->|				|
//          |                 |<-+----->|P1.4/INT1        |     |				|
//          |                 |         |       P2.4/INT1 |     |				|
//          |                 |         |             	  |		|				|
//*********************************************************************************
/* Header files */
#include "TS_Proximity_Sensor.h"
#include "TS_ProcessHandler.h"

/* Commands for setting the registers in the sensor */
const unsigned char MINUTE_SENSOR_CONFIG[TOTAL_COMMANDS_MINUTE][TOTAL_BYTES_SEND]=
	{
		{COMMAND_PULSE_COUNT_REG,PULSE_COUNT_MINUTE},
		/*{COMMAND_CONFIG_REG,CONFIG_VALUE},
		{COMMAND_WAIT_TIME_REG,WAIT_TIME_VALUE},*/
		{COMMAND_CONTROL_REG,DIODE_GAIN_MINUTE},
		{COMMAND_PERSISTENCE_REG,PERSISTANCE_VALUE},
		{COMMAND_HIGH_LOW_BYTE_REG,INT_HIGH_LOW_MINUTE},
		{COMMAND_HIGH_HIGH_BYTE_REG,INT_HIGH_HIGH_MINUTE}
	};
const unsigned char HOUR_SENSOR_CONFIG[TOTAL_COMMANDS_HOUR][TOTAL_BYTES_SEND]=
	{
		{COMMAND_PULSE_COUNT_REG,PULSE_COUNT_HOUR},
		/*{COMMAND_CONFIG_REG,CONFIG_VALUE},
		{COMMAND_WAIT_TIME_REG,WAIT_TIME_VALUE},*/
		{COMMAND_CONTROL_REG,DIODE_GAIN_HOUR},
		{COMMAND_PERSISTENCE_REG,PERSISTANCE_VALUE},
		{COMMAND_HIGH_LOW_BYTE_REG,INT_HIGH_LOW_HOUR},
		{COMMAND_HIGH_HIGH_BYTE_REG,INT_HIGH_HIGH_HOUR}
	};
const unsigned char SECOND_SENSOR_CONFIG[TOTAL_COMMANDS_SECOND][TOTAL_BYTES_SEND]=
	{
		{COMMAND_PULSE_COUNT_REG,PULSE_COUNT_SECOND},
		/*{COMMAND_CONFIG_REG,CONFIG_VALUE},
		{COMMAND_WAIT_TIME_REG,WAIT_TIME_SECOND},*/
		{COMMAND_CONTROL_REG,DIODE_GAIN_SECOND},
		{COMMAND_PERSISTENCE_REG,PERSISTANCE_VALUE},
		{COMMAND_HIGH_LOW_BYTE_REG,INT_HIGH_LOW_SECOND},
		{COMMAND_HIGH_HIGH_BYTE_REG,INT_HIGH_HIGH_SECOND}
	};
/*constant for clearing the interrupt*/
const unsigned char INTERRUPT_CLEAR = 0xE5;
/* command for starting the proximity with interrupt enabled */
const unsigned char BUFFER_PROXI_INT_START[TOTAL_BYTES_SEND] = {COMMAND_ENABLE_REG,PON_PROX_INT_ENABLE};
/* command for starting the proximity with interrupt enabled */
const unsigned char BUFFER_PROXI_START[TOTAL_BYTES_SEND] = {COMMAND_ENABLE_REG,PON_PROX_ENABLE};
const unsigned char BUFFER_PON_START[TOTAL_BYTES_SEND] = {COMMAND_ENABLE_REG,PON_ENABLE};
/*constant for clearing the enable register*/
const unsigned char BUFFER_PROXI_DISABLE[TOTAL_BYTES_SEND] = {COMMAND_ENABLE_REG,0x00};
/*Register command for reading proximity data value*/
const unsigned char PROXI_READ = 0xB8;
const unsigned char *PTxData;               // Pointer to TX data
const uint16 PROXI_READ_MASK = 0x0003;		// Mask for the proximity read data, last byte omitted

/* macros for the file */
//uint16 usHourHandReadValue = 0;//	0x0280 Proto clock values 0x03F4  0x0244 usHourHandReadValue


/* Variables for the file */
static uint8 TXByteCtr;
/*variables for receive*/
static uint8 *PRxData;                     	// Pointer to RX data
static uint8 RXByteCtr;

uint16 usProxiReadData = 0;					// for check made global

/* local functions */
static void TS_MinuteSensorIntInit (void);
static void TS_HourSecondSensorIntInit (void);
static void TS_SensorIIC_Start (void);
static void TS_MinuteSensorInit (void);
static void TS_HourSecondSensorInit (void);
static void TS_Config_MinuteSensor (void);
static void TS_ConfigHourSecondSensor (uint8 ucSelectHand);
/**************************************************************************//**
* TS_MinuteSensorIntInit
* @brief         - First Interrupt pin initialization(P1.4).
* @param         -
* @return        -
******************************************************************************/
static void TS_MinuteSensorIntInit (void)
{
 /*Disabling the hour interrupt*/
  HOUR_INT_DIR |= HOUR_INT;
  HOUR_INT_OUT |= HOUR_INT;

  MINUTE_INT_DIR &= ~MINUTE_INT;	// as input
  MINUTE_INT_REN |= MINUTE_INT;		// Pull up or pull down enabled.
  MINUTE_INT_IES |= MINUTE_INT;		// P1IFG flag is set with a high-to-low transition.
  MINUTE_INT_OUT |= MINUTE_INT;		// Pull up selected
  MINUTE_INT_IFG  = 0;				// No interrupt is pending
  MINUTE_INT_IE  |= MINUTE_INT;		// interrupt enabled
}
/**************************************************************************//**
* TS_SecondSensorInterrupt_Init
* @brief         - Second Interrupt pin initialization(P2.4).
* @param         -
* @return        -
******************************************************************************/
static void TS_HourSecondSensorIntInit (void)
{
 /*Disabling the minute interrupt*/
  MINUTE_INT_DIR |= MINUTE_INT;
  MINUTE_INT_OUT |= MINUTE_INT;

  HOUR_INT_DIR &= ~HOUR_INT;
  HOUR_INT_REN |= HOUR_INT;
  HOUR_INT_IES |= HOUR_INT;
  HOUR_INT_OUT |= HOUR_INT;		    		// 10K Ohm pull up resistor used.
  HOUR_INT_IFG  = 0;
  HOUR_INT_IE  |= HOUR_INT;
}
/**************************************************************************//**
* TS_SensorIIC_Start
* @brief         - All IIC register initializations.
* @param         -
* @return        -
******************************************************************************/
static void TS_SensorIIC_Start (void)
{
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset

  UCB0CTL0 = (UCMST + UCMODE_3 + UCSYNC);   // I2C mode, Master, synchronous mode
  UCB0CTL1 = (UCSSEL__ACLK + UCSWRST);      // Use ACLK=32Khz, keep SW reset
  UCB0BR0  = 12;                            // fSCL = ACLK/12 = ~2.73kHz  //183.1uSec period
  UCB0BR1  = 0;								// pre-scaler value = (UCxxBR0 + UCxxBR1 × 256)
  UCB0I2CSA = SLAVE_ADDRESS;                // Slave Address for sensor.

  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

  UCB0IE |=  UCALIE;   						// Enable TX interrupt, RX interrupt
}
/******************************************************************************
* Outline 		: TS_MinuteSensorInit
* Description 	: IIC hardware Initialization for first sensor.
* Argument  	: none
* Return value  : none
******************************************************************************/
static void TS_MinuteSensorInit (void)
{
  TS_MinuteSensorIntInit();

  PMAPPWD = PMAPKEY;                        // Get write-access to port mapping registers.
  P1MAP3  = PM_UCB0SDA;                     // P1.3 // I2C data
  P1MAP2  = PM_UCB0SCL;                     // P1.2 // I2C clock
  P2MAP6  = PM_NONE;                     	// PM_NONE output to P2.6
  P2MAP3  = PM_NONE;                     	// PM_NONE output to P2.3
  PMAPPWD = 0;                              // Lock port mapping registers

  MINUTE_IIC_SEL |= MINUTE_SCL + MINUTE_SDA;
  HOUR_IIC_SEL   &= ~(HOUR_SDA + HOUR_SCL);

  TS_SensorIIC_Start ();
}
/******************************************************************************
* Outline 		: TS_HourSecondSensorInit
* Description 	: IIC hardware Initialization for second sensor.
* Argument  	: none
* Return value  : none
******************************************************************************/
static void TS_HourSecondSensorInit (void)
{
  TS_HourSecondSensorIntInit ();

  PMAPPWD = PMAPKEY;                        // Get write-access to port mapping regs
  P1MAP3  = PM_NONE;                     	// PM_NONE output to P1.3
  P1MAP2  = PM_NONE;                     	// PM_NONE output to P1.2
  P2MAP3  = PM_UCB0SDA;                     // P2.3 // I2C data
  P2MAP6  = PM_UCB0SCL;                     // P2.6 // I2C clock
  PMAPPWD = 0;                              // Lock port mapping registers

  MINUTE_IIC_SEL &= ~(MINUTE_SCL + MINUTE_SDA);
  HOUR_IIC_SEL   |= HOUR_SDA + HOUR_SCL;    //  P2.6=SCL, P2.3=SDA

  TS_SensorIIC_Start ();
}
/**************************************************************************//**
* TS_SensorIntClear
* @brief         - Clearing the interrupt status for further sensing.
* @param         -
* @return        -
******************************************************************************/
void TS_SensorIntClear (void)
{
   /* Command for clearing the interrupt in the sensor */
   TS_Sensor_IIC_Write (&INTERRUPT_CLEAR,CLEAR_COMMAND_BYTE);
}
/**************************************************************************//**
* TS_Config_MinuteSensor
* @brief         - Program for configuring the sensor registers for Proximity read.
* @param         -
* @return        -
******************************************************************************/
static void TS_Config_MinuteSensor (void)
{
  uint8 ucTotalCommand=0;

  TS_SensorIntClear ();

  /*Sending the commands for the setting of the sensor registers*/
  for(ucTotalCommand=0;ucTotalCommand < TOTAL_COMMANDS_MINUTE;ucTotalCommand++)
  {
	  TS_Sensor_IIC_Write (MINUTE_SENSOR_CONFIG[ucTotalCommand],TOTAL_BYTES_SEND);
  }
}
/**************************************************************************//**
* TS_ConfigHourSecondSensor
* @brief         - Program for configuring the sensor registers for Proximity read.
* @param         -
* @return        -
******************************************************************************/
static void TS_ConfigHourSecondSensor (uint8 ucSelectHand)
{
  uint8 ucTotalCommand = 0;

  TS_SensorIntClear ();

  /*Sending the commands for the setting of the sensor registers*/
  if(HOUR_HAND == ucSelectHand)
  {
	  for(ucTotalCommand=0;ucTotalCommand < TOTAL_COMMANDS_HOUR;ucTotalCommand++)
	  {
		  TS_Sensor_IIC_Write (HOUR_SENSOR_CONFIG[ucTotalCommand],TOTAL_BYTES_SEND);
	  }
  }
  if(SECOND_HAND == ucSelectHand)
  {
	  for(ucTotalCommand=0;ucTotalCommand < TOTAL_COMMANDS_SECOND;ucTotalCommand++)
	  {
		  TS_Sensor_IIC_Write (SECOND_SENSOR_CONFIG[ucTotalCommand],TOTAL_BYTES_SEND);
	  }
  }
}
/**************************************************************************//**
* TS_MinuteSensorStart
* @brief         - Program for starting the first sensor.
* @param         -
* @return        -
******************************************************************************/
void TS_MinuteSensorStart (uint8 ucInterrupt)
{
	TS_MinuteSensorInit ();

	TS_Sensor_IIC_Write (BUFFER_PON_START,TOTAL_BYTES_SEND);
	TS_Config_MinuteSensor ();

	if(INT_ENABLE == ucInterrupt)
	{
		TS_Sensor_IIC_Write (BUFFER_PROXI_INT_START,TOTAL_BYTES_SEND);
	}
	else
	{
		TS_Sensor_IIC_Write (BUFFER_PROXI_START,TOTAL_BYTES_SEND);
	}
}
/**************************************************************************//**
* TS_HourSecondSensorStart
* @brief         - Program for starting the second sensor.
* @param         -
* @return        -
******************************************************************************/
void TS_HourSecondSensorStart (uint8 ucSelectHand,uint8 ucInterrupt)
{
   TS_HourSecondSensorInit ();

   TS_Sensor_IIC_Write (BUFFER_PON_START,TOTAL_BYTES_SEND);
   TS_ConfigHourSecondSensor (ucSelectHand);

	if(INT_ENABLE == ucInterrupt)
	{
		TS_Sensor_IIC_Write (BUFFER_PROXI_INT_START,TOTAL_BYTES_SEND);
	}
	else
	{
		TS_Sensor_IIC_Write (BUFFER_PROXI_START,TOTAL_BYTES_SEND);
	}
}
/**************************************************************************//**
* TS_SensorStop
* @brief         - Function to disable the sensor and clearing the interrupts.
* @param         -
* @return        -
******************************************************************************/
void TS_SensorStop (void)
{
  /* clearing the enable register. */
  TS_Sensor_IIC_Write (BUFFER_PROXI_DISABLE,TOTAL_BYTES_SEND);
  /* clearing the interrupts. */
  TS_Sensor_IIC_Write (&INTERRUPT_CLEAR,CLEAR_COMMAND_BYTE);
  /* Enable SW reset, disabling the IIC activity */
  UCB0CTL1 |= UCSWRST;
}
/**************************************************************************//**
* TS_ReturnProximityReadData
* @brief         - Program for returning total GPS seconds from GPS Buffer.
* @param         -
* @return        -
******************************************************************************/
uint16 TS_ReturnProximityReadData (void)
{
	uint8 ucProxiBufferRead[2] = {0};
//	uint16 usProxiReadData = 0;						// for check disabled

	TS_Sensor_IIC_Read (&PROXI_READ,ucProxiBufferRead);
	/* For combining the byte values of the proximity data */
	usProxiReadData = ((uint16)((uint16)ucProxiBufferRead[1] << SHIFT)\
									  + (uint16)ucProxiBufferRead[0]);
	/* for masking the last byte of the value */
	usProxiReadData &= ~PROXI_READ_MASK;

	return usProxiReadData;
}
/**************************************************************************//**
* TS_CheckMinuteHourHand
* @brief         - Program for checking the position of minute and hour hand.
* @param         -
* @return        - Return the SUCCESS when both hands are detected else FAILURE
******************************************************************************/
uint8 TS_CheckMinuteHourHand (void)
{
	uint16 usProxiReadCheck = RESET;		// variable for value read
	uint8  ucCheckResult = RESET;			// Check result return variable
	uint16 usActualSec = 0;

	if(SET == TS_ReturnFirstSensorFlag ())
	{
	  /* Clear the sensor interrupt and interrupt flag */
	  TS_SensorIntClear();
	  /* Stop the minute sensor */
	  TS_SensorStop ();
	  TS_ClearFirstSensorFlag ();
	  TS_Clock_Pwm_Stop();
#ifdef TEST_SENSOR_FUNCTIONALITY
	  TS_BlinkPowerLEDOnce();
#endif
	  /* Start the hour sensor*/
	  TS_HourSecondSensorStart (HOUR_HAND,INT_DISABLE);
	  /* Read the hour sensor proximity value */
	  usProxiReadCheck = TS_ReturnProximityReadData ();

	  if(usProxiReadCheck)
	  {
#ifdef TEST_SENSOR_FUNCTIONALITY
		  TS_BlinkLinkLossLEDOnce();
#else
		  usActualSec = TS_RunningDelayCalc (CONFIRM_HOUR_HAND_IN_SEC);
		  TS_SetActualSeconds(CONFIRM_HOUR_HAND_IN_SEC + usActualSec);
		  TS_Clock_Pwm_Mode (TS_FAST_MODE);
		  TS_ClearActualSeconds();
		  /* Start the hour sensor*/
		  TS_HourSecondSensorStart (HOUR_HAND,INT_DISABLE);
		  /* Read the hour sensor proximity value */
		  usProxiReadCheck = TS_ReturnProximityReadData ();
#endif
		  if(usProxiReadCheck)
		  {
			  TS_Clock_Pwm_Stop();
			  TS_SensorStop ();
			  TS_ClearSecondSensorFlag ();	 	// Clear the second sensor flag
			  TS_ClearFirstSensorFlag ();	 	// Clear the first sensor flag

			  usProxiReadCheck = RESET;			// clear the Proximity sensor read variable
			  ucCheckResult = SET;
			  return ucCheckResult;
		  }
		  else
		  {

		  }
	  }
	}

	return RESET;
}
/**************************************************************************//**
* TS_Sensor_IIC_Write
* @brief         - Function for IIC sensor write.
* @param         - pointer to which the register to be write,
* 				   total number of bytes to write
* @return        - uint8 if returned 0 IIC is free.
******************************************************************************/
uint8 TS_Sensor_IIC_Write (const unsigned char *ucWriteBuffer, uint8 ucTotalBytes)
{
  uint8 ucIICstatusByte = 0;

  __delay_cycles(47);				// Delay between byte transactions

  PTxData   = ucWriteBuffer; 		// TX array start address
  TXByteCtr = ucTotalBytes;

  UCB0IE   &= ~(UCRXIE);
  UCB0IE   |= (UCTXIE);
  UCB0CTL1 |= (UCTR + UCTXSTT);      // I2C TX, start condition
  __bis_SR_register(LPM3_bits);    // Enter LPM0, enable interrupts

  while (UCB0CTL1 & UCTXSTP);      // Ensure stop condition got sent, receiver mode,UCTXSTP is
								   // automatically cleared after STOP is generated.
   __no_operation();

  ucIICstatusByte = (UCB0STAT & UCBUSY);

  return ucIICstatusByte;			// returning the Status register
									// If returned 0 IIC is free after operation
}
/**************************************************************************//**
* TS_Sensor_IIC_Read
* @brief         - Function for IIC sensor read.
* @param         - pointer to the register to be read,
* 				   pointer to which the data is to be read.
* @return        - uint8 if returned 0 IIC is free.
******************************************************************************/
uint8 TS_Sensor_IIC_Read (const unsigned char *ucWriteCommand, uint8 *ucReadBuffer)
{
  uint8 ucIICstatusByte = 0;

  PTxData = ucWriteCommand; 						// TX array start address
  PRxData = ucReadBuffer;    						// Start of RX buffer
  TXByteCtr = 1;									// total number of commands
  RXByteCtr = sizeof ucReadBuffer;                  // Load RX byte counter

  UCB0IE |= UCTXIE + UCSTTIE + UCSTPIE;				// enabling the TX
  UCB0CTL1 |= UCTR + UCTXSTT;             	 		// I2C start and as master
   __bis_SR_register(LPM3_bits);     				// Enter LPM0, enable interrupts
   	   	   	   	   	   	   	   	   	   	   	   	   	// Remain in LPM0 until all data is TX'd

   while (UCB0CTL1 & UCTXSTP);
   UCB0CTL1 &= ~(UCTR);								// for selecting the receive operation Libin
   UCB0IE   |= UCRXIE;
   UCB0CTL1 |= UCTXSTT;                    			// I2C start and receiver
   __bis_SR_register(LPM3_bits);    				// Enter LPM0, enable interrupts

   while (UCB0CTL1 & UCTXSTP);

   ucIICstatusByte =(UCB0STAT & UCBUSY);
   return ucIICstatusByte;							// returning the Status register
													// If returned 0 IIC is free after operation
}

//------------------------------------------------------------------------------
// The USCIAB0TX_ISR
// Any number of data bytes can be send through the IIC bus and any number of
// data bytes can be received through the IIC.
//------------------------------------------------------------------------------
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch (__even_in_range(UCB0IV,12))
  {
	  case  0: break;                          		// Vector  0: No interrupts
	  case  2: break;                           	// Vector  2: ALIFG
	  case  4: break;                           	// Vector  4: NACKIFG
	  case  6: break;                           	// Vector  6: STTIFG
	  case  8: break;                           	// Vector  8: STPIFG
	  case 10:										// Vector 10: RXIFG
			RXByteCtr--;                            // Decrement RX byte counter.
			if (RXByteCtr)
			{
			  *PRxData++ = UCB0RXBUF;               // Move RX data to address PRxData
			  if (1 == RXByteCtr)                   // Only one byte left?
			  {
				UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
			  }
			}
			else
			{
			  *PRxData = UCB0RXBUF;                 // Move final RX data to PRxData
			  UCB0IFG = 0;
			  UCB0IE &= ~(UCRXIE);  				// clearing the RX interrupt
			  __bic_SR_register_on_exit(LPM3_bits);
			}
		   break;
  case 12:                                   		// Vector 12: TXIFG
			if (TXByteCtr)                          // Check TX byte counter
			{
			  UCB0TXBUF = *PTxData++;               // Load TX buffer
			  TXByteCtr--;                          // Decrement TX byte counter
			}
			else
			{
			  UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
			  UCB0IFG = 0;
			  UCB0IE &= ~(UCTXIE);  				// clearing the transmit interrupt
			  __bic_SR_register_on_exit(LPM3_bits);
			}
			break;
  default:  break;
  }
}


