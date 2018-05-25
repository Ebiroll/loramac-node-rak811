/*!
 * \file      board-config.h
 *
 * \brief     Board configuration
 *
 * \copyright See section \ref LICENSE.
 *
 * \author    Johannes Bruder
 */
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__


/*!
 * Defines the time required for the TCXO to wakeup [ms].
 */
#define BOARD_TCXO_WAKEUP_TIME                      0


/*!
 * Board IO Extender pins definitions
 */
#define LED_1                                       PA_12
#define LED_2                                       PB_4

/*!
 * Board MCU pins definitions
 */

#define RADIO_RESET                                 PB_13
#define RADIO_XTAL_EN                               PH_1

#define RADIO_MOSI                                  PA_7
#define RADIO_MISO                                  PA_6
#define RADIO_SCLK                                  PA_5
#define RADIO_NSS                                   PB_0

#define RADIO_DIO_0                                 PA_11
#define RADIO_DIO_1                                 PB_1
#define RADIO_DIO_2                                 PA_3
#define RADIO_DIO_3                                 PH_0
#define RADIO_DIO_4                                 PC_13

#define RADIO_RF_CRX_RX                             PB_6  //CRF3
#define RADIO_RF_CBT_HF                             PB_7  //CRF2 HF
#define RADIO_RF_CTX_PA                             PA_4  //CRF1 PA

#define OSC_LSE_IN                                  PC_14
#define OSC_LSE_OUT                                 PC_15

#define OSC_HSE_IN                                  PH_0
#define OSC_HSE_OUT                                 PH_1

//#define USB_DM                                      PA_11
//#define USB_DP                                      PA_12

#define I2C_SCL                                     PB_8
#define I2C_SDA                                     PB_9
#define LIS3DH_INT1_PIN                             PB_14
#define LIS3DH_INT2_PIN                             PB_15

#define UART_TX                                     PA_9
#define UART_RX                                     PA_10

#define GPS_UART                                    UART_3
#define GPS_POWER_ON                                PA_15
#define GPS_UART_TX                                 PB_10
#define GPS_UART_RX                                 PB_11
//#define GPS_PPS                                     PA_0

#define BAT_LEVEL_PIN                               PA_2
#define BAT_LEVEL_CHANNEL                           ADC_CHANNEL_2

#define SWDIO                                       PA_13
#define SWCLK                                       PA_14

#endif // __BOARD_CONFIG_H__
