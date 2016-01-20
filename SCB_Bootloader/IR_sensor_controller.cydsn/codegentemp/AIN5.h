/*******************************************************************************
* File Name: AIN5.h  
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

#if !defined(CY_PINS_AIN5_H) /* Pins AIN5_H */
#define CY_PINS_AIN5_H

#include "cytypes.h"
#include "cyfitter.h"
#include "AIN5_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    AIN5_Write(uint8 value) ;
void    AIN5_SetDriveMode(uint8 mode) ;
uint8   AIN5_ReadDataReg(void) ;
uint8   AIN5_Read(void) ;
uint8   AIN5_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define AIN5_DRIVE_MODE_BITS        (3)
#define AIN5_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - AIN5_DRIVE_MODE_BITS))

#define AIN5_DM_ALG_HIZ         (0x00u)
#define AIN5_DM_DIG_HIZ         (0x01u)
#define AIN5_DM_RES_UP          (0x02u)
#define AIN5_DM_RES_DWN         (0x03u)
#define AIN5_DM_OD_LO           (0x04u)
#define AIN5_DM_OD_HI           (0x05u)
#define AIN5_DM_STRONG          (0x06u)
#define AIN5_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define AIN5_MASK               AIN5__MASK
#define AIN5_SHIFT              AIN5__SHIFT
#define AIN5_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define AIN5_PS                     (* (reg32 *) AIN5__PS)
/* Port Configuration */
#define AIN5_PC                     (* (reg32 *) AIN5__PC)
/* Data Register */
#define AIN5_DR                     (* (reg32 *) AIN5__DR)
/* Input Buffer Disable Override */
#define AIN5_INP_DIS                (* (reg32 *) AIN5__PC2)


#if defined(AIN5__INTSTAT)  /* Interrupt Registers */

    #define AIN5_INTSTAT                (* (reg32 *) AIN5__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define AIN5_DRIVE_MODE_SHIFT       (0x00u)
#define AIN5_DRIVE_MODE_MASK        (0x07u << AIN5_DRIVE_MODE_SHIFT)


#endif /* End Pins AIN5_H */


/* [] END OF FILE */
