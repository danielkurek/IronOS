/*
 * flash.c
 *
 *  Created on: 29 May 2020
 *      Author: Ralim
 */

#include "BSP.h"
#include "BSP_Flash.h"
#include "ch32f20x_flash.h"
#include "string.h"

void flash_save_buffer(const uint8_t *buffer, const uint16_t length) {
  resetWatchdog();
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPRTERR | FLASH_FLAG_BSY);
//   HAL_Delay(1);
  resetWatchdog();
  FLASH_Status flash_status = FLASH_ErasePage(SETTINGS_START_PAGE);  //Erase 4KB
  if(flash_status != FLASH_COMPLETE){
    // TODO: error
  }
  // erased the chunk
  // now we program it
  uint16_t *data = (uint16_t *)buffer;
  FLASH_Unlock();
  for (uint16_t i = 0; i < (length / 2); i++) {
    resetWatchdog();
    flash_status = FLASH_ProgramHalfWord(SETTINGS_START_PAGE + (i * sizeof(uint16_t)), data[i]);
    if(flash_status != FLASH_COMPLETE){
        // TODO: error
    }
  }
  FLASH_Lock();
}

void flash_read_buffer(uint8_t *buffer, const uint16_t length) { memcpy(buffer, (uint8_t *)SETTINGS_START_PAGE, length); }
