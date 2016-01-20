/*******************************************************************************
* File Name: AIN0.h  
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

#if !defined(CY_PINS_AIN0_H) /* Pins AIN0_H */
#define CY_PINS_AIN0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "AIN0_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    AIN0_Write(uint8 value) ;
void    AIN0_SetDriveMode(uint8 mode) ;
uint8   AIN0_ReadDataReg(void) ;
uint8   AIN0_Read(void) ;
uint8   AIN0_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define AIN0_DRIVE_MODE_BITS        (3)
#define AIN0_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - AIN0_DRIVE_MODE_BITS))

#define AIN0_DM_ALG_HIZ         (0x00u)
#define AIN0_DM_DIG_HIZ         (0x01u)
#define AIN0_DM_RES_UP          (0x02u)
#define AIN0_DM_RES_DWN         (0x03u)
#define AIN0_DM_OD_LO           (0x04u)
#define AIN0_DM_OD_HI           (0x05u)
#define AIN0_DM_STRONG          (0x06u)
#define AIN0_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define AIN0_MASK               AIN0__MASK
#define AIN0_SHIFT              AIN0__SHIFT
#define AIN0_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define AIN0_PS                     (* (reg32 *) AIN0__PS)
/* Port Configuration */
#define AIN0_PC                     (* (reg32 *) AIN0__PC)
/* Data Register */
#define AIN0_DR                     (* (reg32 *) AIN0__DR)
/* Input Buffer Disable Override */
#define AIN0_INP_DIS                (* (reg32 *) AIN0__PC2)


#if defined(AIN0__INTSTAT)  /* Interrupt Registers */

    #define AIN0_INTSTAT                (* (reg32 *) AIN0__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define AIN0_DRIVE_MODE_SHIFT       (0x00u)
#define AIN0_DRIVE_MODE_MASK        (0x07u << AIN0_DRIVE_MODE_SHIFT)


#endif /* End Pins AIN0_H */


/* [] END OF FILE */
