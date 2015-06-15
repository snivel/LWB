#ifndef __PORTS_H__
#define __PORTS_H__

// use the following pin definitions for all _DIRECT macros
#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7

/*#define PIN0BIT 0x01
#define PIN1BIT 0x02
#define PIN2BIT 0x04
#define PIN3BIT 0x08
#define PIN4BIT 0x10
#define PIN5BIT 0x20
#define PIN6BIT 0x40
#define PIN7BIT 0x80*/

#define PORT1   1
#define PORT2   2
#define PORT3   3
#define PORT4   4
#define PORT5   5

#define PIN_TO_BIT(pin)                     (1 << pin)

/** 
 * @brief access a 16-bit register value (read or write)
 */
#define REGVAL16(x)                         (*((volatile uint16_t*)((uint16_t)x))) 
/** 
 * @brief access an 8-bit register value (read or write)
 */
#define REGVAL8(x)                          (*((volatile uint8_t*)((uint16_t)x)))

// Note: all following macros ending with '_DIRECT' can only be used when passing numbers directly (no defines or variables)
#define PIN_TOGGLE_DIRECT(port, pin)        P##port##OUT ^=  BIT##pin
#define PIN_SET_DIRECT(port, pin)           P##port##OUT |=  BIT##pin
#define PIN_CLEAR_DIRECT(port, pin)         P##port##OUT &= ~BIT##pin
#define PIN_SELECT_DIRECT(port, pin)        P##port##SEL |=  BIT##pin
#define PIN_UNSELECT_DIRECT(port, pin)      P##port##SEL &= ~BIT##pin
#define PIN_SET_AS_OUTPUT_DIRECT(port, pin) P##port##DIR |=  BIT##pin
#define PIN_SET_AS_INPUT_DIRECT(port, pin)  P##port##DIR &= ~BIT##pin
#define PIN_MAP_DIRECT(port, pin, map)      P##port##MAP##pin = map
#define PIN_CLEAR_IFG_DIRECT(port, pin)     P##port##IFG &= ~BIT##pin
#define PIN_RESISTOR_EN_DIRECT(port, pin)   P##port##REN |=  BIT##pin       // only has an effect if port is configured as input
#define PIN_IES_RISING_DIRECT(port, pin)    P##port##IES &= ~BIT##pin
#define PIN_IES_FALLING_DIRECT(port, pin)   P##port##IES |=  BIT##pin
#define PIN_IES_TOGGLE_DIRECT(port, pin)    P##port##IES ^=  BIT##pin
#define PIN_EINT_DIRECT(port, pin)          P##port##IE  |=  BIT##pin
#define PIN_CFG_PORT_INT_DIRECT(port, pin)  { PIN_IES_RISING_DIRECT(port, pin); PIN_CLEAR_IFG_DIRECT(port, pin); PIN_EINT_DIRECT(port, pin); }
#define PIN_IFG_DIRECT(port, pin)           ( P##port##IFG & (uint8_t)(BIT##pin) )
#define PIN_GET_INPUT_BIT_DIRECT(port, pin) ( P##port##IN & (uint8_t)(BIT##pin) )
#define PORT_TOGGLE_DIRECT(port)            P##port##OUT ^= 0xff
#define PORT_SET_DIRECT(port)               P##port##OUT = 0xff
#define PORT_CLEAR_DIRECT(port)             P##port##OUT = 0x00
#define PORT_SELECT_DIRECT(port)            P##port##SEL = 0xff
#define PORT_UNSELECT_DIRECT(port)          P##port##SEL = 0x00
#define PORT_RESISTOR_EN_DIRECT(port)       P##port##REN = 0xff
#define PORT_CLEAR_IFG_DIRECT(port)         P##port##IFG = 0x00
#define PORT_SET_AS_OUTPUT_DIRECT(port)     P##port##DIR = 0xff
#define PORT_SET_AS_INPUT_DIRECT(port)      P##port##DIR = 0x00
#define PIN_MAP_AS_OUTPUT_DIRECT(port, pin, map)  {\
                                                /* disable interrupts */ \
                                                __dint(); __nop(); \
                                                /* get write-access to the port mapping control registers (see 9.2.1) */ \
                                                PMAPKEYID = 0x02D52; \
                                                /* allow reconfiguration of port mapping */ \
                                                PMAPCTL |= PMAPRECFG; \
                                                PIN_SELECT_DIRECT(port, pin); \
                                                PIN_SET_AS_OUTPUT_DIRECT(port, pin); \
                                                PIN_MAP_DIRECT(port, pin, map); \
                                                /* lock write-access to the port mapping control registers (see 9.2.1) */ \
                                                PMAPKEYID = 0; \
                                                /* enable interrupts */ \
                                                __eint(); __nop(); \
                                            }
#define PIN_MAP_AS_INPUT_DIRECT(port, pin, map)   {\
                                                /* disable interrupts */ \
                                                __dint(); __nop(); \
                                                /* get write-access to the port mapping control registers (see 9.2.1) */ \
                                                PMAPKEYID = 0x02D52; \
                                                /* allow reconfiguration of port mapping */ \
                                                PMAPCTL |= PMAPRECFG; \
                                                PIN_SELECT_DIRECT(port, pin); \
                                                PIN_SET_AS_INPUT_DIRECT(port, pin); \
                                                PIN_MAP_DIRECT(port, pin, map); \
                                                /* lock write-access to the port mapping control registers (see 9.2.1) */ \
                                                PMAPKEYID = 0; \
                                                /* enable interrupts */ \
                                                __eint(); __nop(); \
                                            }
                                            
/**
 * @brief toggle a (port, pin) output, i.e. set it high when it was low and vice-versa
 */                                                
#define PIN_TOGGLE(p)                       PIN_TOGGLE_DIRECT(p)  
/**
 * @brief set a (port, pin) output, i.e. push it's output signal high
 */                   
#define PIN_SET(p)                          PIN_SET_DIRECT(p)     
/**
 * @brief clear a (port, pin) output, i.e. pull it's output signal low
 */                            
#define PIN_CLEAR(p)                        PIN_CLEAR_DIRECT(p)
/**
 * @brief select a (port, pin), i.e. configure it in module function mode
 */
#define PIN_SELECT(p)                       PIN_SELECT_DIRECT(p)
/**
 * @brief unselect a (port, pin), i.e. configure it in port mode (GPIO)
 */
#define PIN_UNSELECT(p)                     PIN_UNSELECT_DIRECT(p)
/**
 * @brief set a (port, pin) as output
 * @note don't forget to unselect a pin if you want to use it in GPIO mode
 */
#define PIN_SET_AS_OUTPUT(p)                PIN_SET_AS_OUTPUT_DIRECT(p)
/**
 * @brief set a (port, pin) as input
 */
#define PIN_SET_AS_INPUT(p)                 PIN_SET_AS_INPUT_DIRECT(p)
/**
 * @brief equivalent to PIN_SELECT 
 */
#define PIN_SET_AS_MODULE_FUNC(p)           PIN_SELECT_DIRECT(p)
/**
 * @brief map a (port, pin) as output 
 */
#define PIN_MAP_AS_OUTPUT(p, map)           PIN_MAP_AS_OUTPUT_DIRECT(p, map)
/**
 * @brief map a (port, pin) as input 
 */
#define PIN_MAP_AS_INPUT(p, map)            PIN_MAP_AS_INPUT_DIRECT(p, map)
/**
 * @brief clear the interrupt flag of the specified (port, pin)
 */
#define PIN_CLEAR_IFG(p)                    PIN_CLEAR_IFG_DIRECT(p)
/*
 * @brief enable the resistor for the specified (port, pin), only if configured as input!
 */
#define PIN_RESISTOR_EN(p)                  PIN_RESISTOR_EN_DIRECT(p)
/**
 * @brief select the rising edge as interrupt edge for the specified (port, pin)
 */
#define PIN_IES_RISING(p)                   PIN_IES_RISING_DIRECT(p)
/**
 * @brief select the falling edge as interrupt edge for the specified (port, pin)
 */
#define PIN_IES_FALLING(p)                  PIN_IES_FALLING_DIRECT(p)
/**
 * @brief toggle the interrupt edge for the specified (port, pin)
 */
#define PIN_IES_TOGGLE(p)                   PIN_IES_TOGGLE_DIRECT(p)
/**
 * @brief enable the port interrupt for the specified (port, pin)
 */
#define PIN_EINT(p)                         PIN_EINT_DIRECT(p)
/**
 * @brief configure the port interrupt for the specified (port, pin), available for pins on port 1 and 2 only
 */
#define PIN_CFG_PORT_INT(p)                 PIN_CFG_PORT_INT_DIRECT(p)
/**
 * @brief check the port interrupt flag of the specified (port, pin), available for pins on port 1 and 2 only
 */                    
#define PIN_IFG(portandpin)             PIN_IFG_DIRECT(portandpin)
/**
 * @brief read the 1-bit signal level at the specified (port, pin), either 0 or 1
 */
#define PIN_GET_INPUT_BIT(portandpin)   PIN_GET_INPUT_BIT_DIRECT(portandpin) 


#endif /* __PORTS_H__ */