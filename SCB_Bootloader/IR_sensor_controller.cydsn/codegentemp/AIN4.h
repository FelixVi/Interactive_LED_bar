/*******************************************************************************
* File Name: AIN4.h  
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

#if !defined(CY_PINS_AIN4_H) /* Pins AIN4_H */
#define CY_PINS_AIN4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "AIN4_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    AIN4_Write(uint8 value) ;
void    AIN4_SetDriveMode(uint8 mode) ;
uint8   AIN4_ReadDataReg(void) ;
uint8   AIN4_Read(void) ;
uint8   AIN4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define AIN4_DRIVE_MODE_BITS        (3)
#define AIN4_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - AIN4_DRIVE_MODE_BITS))

#define AIN4_DM_ALG_HIZ         (0x00u)
#define AIN4_DM_DIG_HIZ         (0x01u)
#define AIN4_DM_RES_UP          (0x02u)
#define AIN4_DM_RES_DWN         (0x03u)
#define AIN4_DM_OD_LO           (0x04u)
#define AIN4_DM_OD_HI           (0x05u)
#define AIN4_DM_STRONG          (0x06u)
#define AIN4_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define AIN4_MASK               AIN4__MASK
#define AIN4_SHIFT              AIN4__SHIFT
#define AIN4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define AIN4_PS                     (* (reg32 *) AIN4__PS)
/* Port Configuration */
#define AIN4_PC                     (* (reg32 *) AIN4__PC)
/* Data Register */
#define AIN4_DR                     (* (reg32 *) AIN4__DR)
/* Input Buffer Disable Override */
#define AIN4_INP_DIS                (* (reg32 *) AIN4__PC2)


#if defined(AIN4__INTSTAT)  /* Interrupt Registers */

    #define AIN4_INTSTAT                (* (reg32 *) AIN4__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define AIN4_DRIVE_MODE_SHIFT       (0x00u)
#define AIN4_DRIVE_MODE_MASK        (0x07u << AIN4_DRIVE_MODE_SHIFT)


#endif /* End Pins AIN4_H */


/* [] END OF FILE */
