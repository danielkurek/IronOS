/*
 * preRTOS.c
 *
 *  Created on: 29 May 2020
 *      Author: Ralim
 */

#include "BSP.h"
#include "I2CBB1.hpp"
// #include "I2CBB2.hpp"
#include "Pins.h"
#include "Setup.h"
#include "system_ch32f20x.h"
#include "ch32f20x_misc.h"
#include <I2C_Wrapper.hpp>

void preRToSInit() {
  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  // TODO: init peripherals
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  SystemCoreClockUpdate();
  BSPInit();
//   I2CBB2::init();
//   I2CBB1::init();
}
