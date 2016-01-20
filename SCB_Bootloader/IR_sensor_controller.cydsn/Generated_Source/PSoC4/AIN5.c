/*******************************************************************************
* File Name: AIN5.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "AIN5.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        AIN5_PC =   (AIN5_PC & \
                                (uint32)(~(uint32)(AIN5_DRIVE_MODE_IND_MASK << (AIN5_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (AIN5_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: AIN5_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void AIN5_Write(uint8 value) 
{
    uint8 drVal = (uint8)(AIN5_DR & (uint8)(~AIN5_MASK));
    drVal = (drVal | ((uint8)(value << AIN5_SHIFT) & AIN5_MASK));
    AIN5_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: AIN5_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  AIN5_DM_STRONG     Strong Drive 
*  AIN5_DM_OD_HI      Open Drain, Drives High 
*  AIN5_DM_OD_LO      Open Drain, Drives Low 
*  AIN5_DM_RES_UP     Resistive Pull Up 
*  AIN5_DM_RES_DWN    Resistive Pull Down 
*  AIN5_DM_RES_UPDWN  Resistive Pull Up/Down 
*  AIN5_DM_DIG_HIZ    High Impedance Digital 
*  AIN5_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void AIN5_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(AIN5__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: AIN5_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro AIN5_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 AIN5_Read(void) 
{
    return (uint8)((AIN5_PS & AIN5_MASK) >> AIN5_SHIFT);
}


/*******************************************************************************
* Function Name: AIN5_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 AIN5_ReadDataReg(void) 
{
    return (uint8)((AIN5_DR & AIN5_MASK) >> AIN5_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(AIN5_INTSTAT) 

    /*******************************************************************************
    * Function Name: AIN5_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 AIN5_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(AIN5_INTSTAT & AIN5_MASK);
		AIN5_INTSTAT = maskedStatus;
        return maskedStatus >> AIN5_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
