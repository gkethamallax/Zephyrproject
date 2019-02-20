/* This file is a temporary workaround for mapping of the generated information
 * to the current driver definitions.  This will be removed when the drivers
 * are modified to handle the generated information, or the mapping of
 * generated data matches the driver definitions.
 */

/* SoC level DTS fixup file */

#define DT_NUM_IRQ_PRIO_BITS	DT_ARM_V7M_NVIC_E000E100_ARM_NUM_IRQ_PRIORITY_BITS

#define DT_FLASH_DEV_BASE_ADDRESS		DT_SILABS_GECKO_FLASH_CONTROLLER_400E0000_BASE_ADDRESS
#define DT_FLASH_DEV_NAME			DT_SILABS_GECKO_FLASH_CONTROLLER_400E0000_LABEL
#define DT_FLASH_PARENT_NAME			DT_SILABS_GECKO_FLASH_CONTROLLER_400E0000_PARENT_LABEL

#define DT_GPIO_GECKO_COMMON_NAME		DT_SILABS_EFR32MG_GPIO_4000A400_LABEL
#define DT_GPIO_GECKO_COMMON_PARENT_NAME		DT_SILABS_EFR32MG_GPIO_4000A400_PARENT_LABEL
#define DT_GPIO_GECKO_COMMON_EVEN_IRQ	DT_SILABS_EFR32MG_GPIO_4000A400_IRQ_GPIO_EVEN
#define DT_GPIO_GECKO_COMMON_EVEN_PRI	DT_SILABS_EFR32MG_GPIO_4000A400_IRQ_GPIO_EVEN_PRIORITY
#define DT_GPIO_GECKO_COMMON_ODD_IRQ	DT_SILABS_EFR32MG_GPIO_4000A400_IRQ_GPIO_ODD
#define DT_GPIO_GECKO_COMMON_ODD_PRI	DT_SILABS_EFR32MG_GPIO_4000A400_IRQ_GPIO_ODD_PRIORITY
#define DT_GPIO_GECKO_SWO_LOCATION	DT_SILABS_EFR32MG_GPIO_4000A400_LOCATION_SWO

#define DT_GPIO_GECKO_PORTA_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A000_LABEL
#define DT_GPIO_GECKO_PORTA_PARENT_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A000_PARENT_LABEL
#define DT_GPIO_GECKO_PORTB_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A030_LABEL
#define DT_GPIO_GECKO_PORTB_PARENT_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A030_PARENT_LABEL
#define DT_GPIO_GECKO_PORTC_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A060_LABEL
#define DT_GPIO_GECKO_PORTC_PARENT_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A060_PARENT_LABEL
#define DT_GPIO_GECKO_PORTD_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A090_LABEL
#define DT_GPIO_GECKO_PORTD_PARENT_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A090_PARENT_LABEL
#define DT_GPIO_GECKO_PORTE_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A0C0_LABEL
#define DT_GPIO_GECKO_PORTE_PARENT_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A0C0_PARENT_LABEL
#define DT_GPIO_GECKO_PORTF_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A0F0_LABEL
#define DT_GPIO_GECKO_PORTF_PARENT_NAME	DT_SILABS_EFR32MG_GPIO_PORT_4000A0F0_PARENT_LABEL

/* End of SoC Level DTS fixup file */
