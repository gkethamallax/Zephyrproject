set(FLASH_SCRIPT nrf_flash.sh)
set_property(GLOBAL APPEND PROPERTY FLASH_SCRIPT_ENV_VARS
  NRF_FAMILY=NRF51
  )
