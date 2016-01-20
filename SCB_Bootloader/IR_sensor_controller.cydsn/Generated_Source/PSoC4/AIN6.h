/*******************************************************************************
* File Name: AIN6.h  
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

#if !defined(CY_PINS_AIN6_H) /* Pins AIN6_H */
#define CY_PINS_AIN6_H

#include "cytypes.h"
#include "cyfitter.h"
#include "AIN6_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    AIN6_Write(uint8 value) ;
void    AIN6_SetDriveMode(uint8 mode) ;
uint8   AIN6_ReadDataReg(void) ;
uint8   AIN6_Read(void) ;
uint8   AIN6_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define AIN6_DRIVE_MODE_BITS        (3)
#define AIN6_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - AIN6_DRIVE_MODE_BITS))

#define AIN6_DM_ALG_HIZ         (0x00u)
#define AIN6_DM_DIG_HIZ         (0x01u)
#define AIN6_DM_RES_UP          (0x02u)
#define AIN6_DM_RES_DWN         (0x03u)
#define AIN6_DM_OD_LO           (0x04u)
#define AIN6_DM_OD_HI           (0x05u)
#define AIN6_DM_STRONG          (0x06u)
#define AIN6_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define AIN6_MASK               AIN6__MASK
#define AIN6_SHIFT              AIN6__SHIFT
#define AIN6_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define AIN6_PS                     (* (reg32 *) AIN6__PS)
/* Port Configuration */
#define AIN6_PC                     (* (reg32 *) AIN6__PC)
/* Data Register */
#define AIN6_DR                     (* (reg32 *) AIN6__DR)
/* Input Buffer Disable Override */
#define AIN6_INP_DIS                (* (reg32 *) AIN6__PC2)


#if defined(AIN6__INTSTAT)  /* Interrupt Registers */

    #define AIN6_INTSTAT                (* (reg32 *) AIN6__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define AIN6_DRIVE_MODE_SHIFT       (0x00u)
#define AIN6_DRIVE_MODE_MASK        (0x07u << AIN6_DRIVE_MODE_SHIFT)


#endif /* End Pins AIN6_H */


/* [] END OF FILE */
