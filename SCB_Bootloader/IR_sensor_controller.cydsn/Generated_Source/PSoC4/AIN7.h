/*******************************************************************************
* File Name: AIN7.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_AIN7_H) /* Pins AIN7_H */
#define CY_PINS_AIN7_H

#include "cytypes.h"
#include "cyfitter.h"
#include "AIN7_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    AIN7_Write(uint8 value) ;
void    AIN7_SetDriveMode(uint8 mode) ;
uint8   AIN7_ReadDataReg(void) ;
uint8   AIN7_Read(void) ;
uint8   AIN7_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define AIN7_DRIVE_MODE_BITS        (3)
#define AIN7_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - AIN7_DRIVE_MODE_BITS))

#define AIN7_DM_ALG_HIZ         (0x00u)
#define AIN7_DM_DIG_HIZ         (0x01u)
#define AIN7_DM_RES_UP          (0x02u)
#define AIN7_DM_RES_DWN         (0x03u)
#define AIN7_DM_OD_LO           (0x04u)
#define AIN7_DM_OD_HI           (0x05u)
#define AIN7_DM_STRONG          (0x06u)
#define AIN7_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define AIN7_MASK               AIN7__MASK
#define AIN7_SHIFT              AIN7__SHIFT
#define AIN7_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define AIN7_PS                     (* (reg32 *) AIN7__PS)
/* Port Configuration */
#define AIN7_PC                     (* (reg32 *) AIN7__PC)
/* Data Register */
#define AIN7_DR                     (* (reg32 *) AIN7__DR)
/* Input Buffer Disable Override */
#define AIN7_INP_DIS                (* (reg32 *) AIN7__PC2)


#if defined(AIN7__INTSTAT)  /* Interrupt Registers */

    #define AIN7_INTSTAT                (* (reg32 *) AIN7__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define AIN7_DRIVE_MODE_SHIFT       (0x00u)
#define AIN7_DRIVE_MODE_MASK        (0x07u << AIN7_DRIVE_MODE_SHIFT)


#endif /* End Pins AIN7_H */


/* [] END OF FILE */
