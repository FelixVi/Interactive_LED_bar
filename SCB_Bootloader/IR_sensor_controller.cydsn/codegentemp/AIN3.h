/*******************************************************************************
* File Name: AIN3.h  
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

#if !defined(CY_PINS_AIN3_H) /* Pins AIN3_H */
#define CY_PINS_AIN3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "AIN3_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    AIN3_Write(uint8 value) ;
void    AIN3_SetDriveMode(uint8 mode) ;
uint8   AIN3_ReadDataReg(void) ;
uint8   AIN3_Read(void) ;
uint8   AIN3_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define AIN3_DRIVE_MODE_BITS        (3)
#define AIN3_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - AIN3_DRIVE_MODE_BITS))

#define AIN3_DM_ALG_HIZ         (0x00u)
#define AIN3_DM_DIG_HIZ         (0x01u)
#define AIN3_DM_RES_UP          (0x02u)
#define AIN3_DM_RES_DWN         (0x03u)
#define AIN3_DM_OD_LO           (0x04u)
#define AIN3_DM_OD_HI           (0x05u)
#define AIN3_DM_STRONG          (0x06u)
#define AIN3_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define AIN3_MASK               AIN3__MASK
#define AIN3_SHIFT              AIN3__SHIFT
#define AIN3_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define AIN3_PS                     (* (reg32 *) AIN3__PS)
/* Port Configuration */
#define AIN3_PC                     (* (reg32 *) AIN3__PC)
/* Data Register */
#define AIN3_DR                     (* (reg32 *) AIN3__DR)
/* Input Buffer Disable Override */
#define AIN3_INP_DIS                (* (reg32 *) AIN3__PC2)


#if defined(AIN3__INTSTAT)  /* Interrupt Registers */

    #define AIN3_INTSTAT                (* (reg32 *) AIN3__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define AIN3_DRIVE_MODE_SHIFT       (0x00u)
#define AIN3_DRIVE_MODE_MASK        (0x07u << AIN3_DRIVE_MODE_SHIFT)


#endif /* End Pins AIN3_H */


/* [] END OF FILE */
