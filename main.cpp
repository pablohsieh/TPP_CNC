#include <Arduino.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_system.h>


#define ESP_INTERNAL_LED 2
#define DELAY_TIME 500


//*********************** DISPLAY ***********************//
// Using a display of 4 ROWS x 20 COLS
#define DISPLAY_MAX_ROW       4
#define DISPLAY_MAX_COLS      20
#include <LiquidCrystal_I2C.h>
// Create object lcd with direction: 0x27 and 20 cols by 4 rows
// PCF8574TI -> 0x27
// PCF8574NxP -> 0x3F
// https://cursos.mcielectronics.cl/2022/12/09/interfaz-de-un-lcd-i2c-con-arduino/
LiquidCrystal_I2C lcd(0x27,DISPLAY_MAX_COLS,DISPLAY_MAX_ROW);  //



/* 
################################
########### Functions ##########
################################
*/ 


void hardware_init(){
  // Initialize the esp internal led
  pinMode(ESP_INTERNAL_LED, OUTPUT);

  // Initialize the LCD
  lcd.init();
  // Turning ON the LCD backlight
  lcd.backlight();

}

void print_lcd_hard_info(){
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);

  lcd.setCursor(1,2);
  lcd.printf("Chip:%s", CONFIG_IDF_TARGET);
  lcd.setCursor(1,3);
  lcd.printf("Cores:%d", chip_info.cores);


}

void get_esp_info(){

  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);



  Serial.printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET, chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  Serial.printf("silicon revision v%d.%d, ", major_rev, minor_rev);

  if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    printf("Get flash size failed");
    return;
  }
  Serial.printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024), 
    (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
  Serial.printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

}


/* 
################################
############# MAIN #############
################################
*/ 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.println("");
  Serial.println("");
  Serial.println("Inicio Setup");

  // get_esp_info();
  hardware_init();
  
  lcd.print("Bienvenido");
  print_lcd_hard_info();


}


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ESP_INTERNAL_LED, HIGH);
//  Serial.println("LED is on");
  delay(DELAY_TIME);
  digitalWrite(ESP_INTERNAL_LED, LOW);
//  Serial.println("LED is off");
  delay(DELAY_TIME); 
  


}




