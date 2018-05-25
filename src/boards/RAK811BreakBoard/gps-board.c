/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Generic low level driver for GPS receiver

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/

#include "board-config.h"
#include "gpio.h"
#include "gps.h"
#include "uart.h"
#include "rtc-board.h"
#include "gps-board.h"

/*!
 * FIFO buffers size
 */
#define GPS_FIFO_TX_SIZE                                128
#define GPS_FIFO_RX_SIZE                                128


uint8_t GpsRxBuffer[GPS_FIFO_RX_SIZE];


/*!
 * \brief Buffer holding the  raw data received from the gps
 */
uint8_t NmeaString[128];

/*!
 * \brief Maximum number of data byte that we will accept from the GPS
 */
uint8_t NmeaStringSize = 0;

Gpio_t GpsPowerEn;
Gpio_t GpsPps;

extern Uart_t Uart3;

PpsTrigger_t PpsTrigger;

void GpsMcuOnPpsSignal( void )
{
#ifdef GPS_PPS
    bool parseData = false;

    GpsPpsHandler( &parseData );

    if( parseData == true )
    {
        UartInit( &Uart3, GPS_UART, GPS_UART_TX, GPS_UART_RX );
        UartConfig( &Uart3, RX_ONLY, 9600, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL );
    }
#endif
}

void GpsMcuInvertPpsTrigger( void )
{
#if 1
    if( PpsTrigger == PpsTriggerIsRising )
    {
        PpsTrigger = PpsTriggerIsFalling;
        GpioSetInterrupt( &GpsPps, IRQ_FALLING_EDGE, IRQ_VERY_LOW_PRIORITY, &GpsMcuOnPpsSignal );
    }
    else
    {
        PpsTrigger = PpsTriggerIsRising;
        GpioSetInterrupt( &GpsPps, IRQ_RISING_EDGE, IRQ_VERY_LOW_PRIORITY, &GpsMcuOnPpsSignal );
    }
#endif
}

void GpsMcuInit( void )
{
    NmeaStringSize = 0;
    PpsTrigger = PpsTriggerIsFalling;

    GpioInit( &GpsPowerEn, GPS_POWER_ON, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );

#ifdef GPS_PPS     
    GpioInit( &GpsPps, GPS_PPS, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
    GpioSetInterrupt( &GpsPps, IRQ_FALLING_EDGE, IRQ_VERY_LOW_PRIORITY, &GpsMcuOnPpsSignal );
#endif

    FifoInit( &Uart3.FifoRx, GpsRxBuffer, GPS_FIFO_RX_SIZE );
    Uart3.IrqNotify = GpsMcuIrqNotify;

    GpsMcuStart( );

    volatile uint32_t count = 0;
    Gpio_t UartTxPin;
    GpioInit ( &UartTxPin, GPS_UART_RX, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0);
    while(!GpioRead(&UartTxPin)) count++;
    while(GpioRead(&UartTxPin)) count++;
    while(!GpioRead(&UartTxPin)) count++;

#ifndef GPS_PPS     
    UartInit( &Uart3, GPS_UART, GPS_UART_TX, GPS_UART_RX );
    UartConfig( &Uart3, RX_ONLY, 9600, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL );
#endif   
}

void GpsMcuStart( void )
{
    GpioWrite( &GpsPowerEn, 1 );    // power up the GPS
}

void GpsMcuStop( void )
{
    GpioWrite( &GpsPowerEn, 0 );    // power down the GPS
}

void GpsMcuProcess( void )
{

}

void GpsMcuIrqNotify( UartNotifyId_t id )
{
    uint8_t data;
    if( id == UART_NOTIFY_RX )
    {
        if( UartGetChar( &Uart3, &data ) == 0 )
        {
            if( ( data == '$' ) || ( NmeaStringSize >= 127 ) )
            {
                NmeaStringSize = 0;
            }

            NmeaString[NmeaStringSize++] = ( int8_t )data;

            if( data == '\n' )
            {
                NmeaString[NmeaStringSize++] = '\0';
							  //printf("%d\n",NmeaStringSize);
                GpsParseGpsData( ( int8_t* )NmeaString, NmeaStringSize );
#ifdef GPS_PPS   
                UartDeInit( &Uart3 );
#endif
                BlockLowPowerDuringTask ( false );
            }
        }
    }
}
