/******************************************************************************
* Project Name		: Bootloadable Blinking LED
* File Name			: main.c
* Version 			: 1.0
* Device Used		: CY8C4245AXI-483
* Software Used		: PSoC Creator 3.1
* Compiler    		: ARMGCC 4.8.4, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT-049-42xx PSoC 4 Prototyping Kit 
*
********************************************************************************
* Copyright (2015), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

/******************************************************************************
*                           THEORY OF OPERATION
* This is a blinking LED project. A PWM component drives the pin to blink the 
* LED at regular intervals. This project contains a bootloadable component so 
* that it can be bootloaded into PSoC 4 which has a bootloader already programmed 
* into it.
* Default UART Port Configuration for bootloading the PSoC 4 in CY8CKIT-049-42xx
* Baud Rate : 115200 bps
* Data Bits : 8
* Stop Bits : 1
* Parity    : None
******************************************************************************/
#include <project.h>
#include <stdio.h>

uint16 result[8] = {0,0,0,0,0,0,0,0};
uint8 dataReady = 0;

int main()
{
    /* Variables for EEPROM storage */
    static const uint8 CYCODE eepromArray[]=
                                { 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00,0x08, 0x00, 0x08, 0x00,0x08, 0x00, 0x08, 0x00,0x08, 0x00, 0x08, 0x00,0x08, 0x00, 0x08, 0x00,0x08, 0x00, 0x08, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
                                 /* Setting thresholds to 2048 for first start */
    
    cystatus status;
    /* End of variables for EEPROM storage */
    
    EEPROM_Start(); /* Starting the EEPROM */
   
    CyGlobalIntEnable; // Enable global interrupts for proper operation of I2C
    
    /* Start the LCD */
    LCD_Start();
    LCD_PrintString("TEST");
    
    /* Initialize I2C component */
    /* Change SCL and SDA pins drive mode to Resistive Pull Up */
    I2C_scl_SetDriveMode(I2C_scl_DM_RES_UP);
    I2C_sda_SetDriveMode(I2C_sda_DM_RES_UP);
    
    uint8 I2C_Buffer[70]; //define the I2C Buffer
    
    /* 256 byte buffer with 34 r/w bytes (addr 0-31) */
    I2C_EzI2CSetBuffer1(70, 34, (void *) I2C_Buffer);
    
    I2C_Start();
    /* Finished initialization of I2C component*/
    
    /* Initialize ADC component */

    ADC_Start(); /* Initialize ADC */
    ADC_IRQ_Enable(); /* Enable ADC interrupts */
    ADC_StartConvert(); /* Start ADC conversions */
    
    /* Finished initialization of ADC component */
    
    int i = 0;
       
    /* Read from EEPROM to retreive previously saved thresholds*/

    for(i=0;i<70;i++)
    {
        I2C_Buffer[i] = *(volatile uint8 *) &eepromArray[i];
    }
    
    /* End of read from EEPROM */

    //uint8 count = 0;
	for(;;)
    {
        if (dataReady != 0)
        {
            /* For testing only */
            //count++;
            //if(count == 128)count = 0;
            char LCD_String[4];
            sprintf(LCD_String,"%i",(int)result[0]);
            Debug_LED_reg_Write(result[0] / 128);
            LCD_Position(0,0);
            LCD_PrintString("    ");
            LCD_Position(0,0);
            LCD_PrintString(LCD_String);
            /* End for testing only */
            dataReady = 0;
            /* Now comparing ADC readings to set thresholds */
            I2C_Buffer[36] = result[0] >> 8;
            I2C_Buffer[37] = result[0];
            I2C_Buffer[38] = result[1] >> 8;
            I2C_Buffer[39] = result[1];
            I2C_Buffer[40] = result[2] >> 8;
            I2C_Buffer[41] = result[2];
            I2C_Buffer[42] = result[3] >> 8;
            I2C_Buffer[43] = result[3];
            I2C_Buffer[44] = result[4] >> 8;
            I2C_Buffer[45] = result[4];
            I2C_Buffer[46] = result[5] >> 8;
            I2C_Buffer[47] = result[5];
            I2C_Buffer[48] = result[6] >> 8;
            I2C_Buffer[49] = result[6];
            I2C_Buffer[50] = result[7] >> 8;
            I2C_Buffer[51] = result[7];
            for(i=0; i<8;i++)
            {
                if(result[i] > (I2C_Buffer[2*i] * 256 + I2C_Buffer[2*i + 1]))I2C_Buffer[34] |= (1<<i);
            }
        }
        if(I2C_Buffer[33] != 0) /* Reset triggers */
        {
            I2C_Buffer[33] = 0;
            I2C_Buffer[34] = 0;
        }
        if(I2C_Buffer[32] != 0) /* EEPROM write */
        {
            I2C_Buffer[32] = 0;
            
            /* EEPROM doesn't work because of bootloader bug */
            
            /* Write to EEPROM */
      
            //status = EEPROM_Write(I2C_Buffer,eepromArray,70u);
              
            //if (CYRET_SUCCESS != status)
            //{
                /* There was a problem writing to EEPROM */
            //}
    
            /* End of write to EEPROM */
        }
    }
}