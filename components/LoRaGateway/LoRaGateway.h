#ifndef LORAGATEWAY
#define LORAGATEWAY


#include "esphome.h"

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "LoRaWan_APP.h"

namespace esphome {
namespace loragateway {

static const char *const TAG = "TDisplayS3";

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 200 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

int16_t txNumber;

int16_t rssi,rxSize;

bool lora_idle = true;

long lastLoRaMillis = -1;



class LoRaGateway : public PollingComponent
{
  public:
    void setup() override {

        //Mcu.begin();
        ESP_LOGCONFIG("LoRa", "Setting Up LoRa Component")
        txNumber=0;
        rssi=0;
    
        RadioEvents.RxDone = OnRxDone;
        Radio.Init( &RadioEvents );
        Radio.SetChannel( RF_FREQUENCY );
        Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

    }

    void loop() override {
        if(lora_idle)
        {
            lora_idle = false;
            ESP_LOGD("LoRa", "Into RX mode");
            Radio.Rx(0);
        }
        Radio.IrqProcess( );
    }

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    lastLoRaMillis=millis();
    rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    Radio.Sleep( );
    ESP_LOGD("LoRa", "Received LoRa packet \"%s\" with rssi %d , length %d",rxpacket,rssi,rxSize);

    lora_idle = true;
}



  private:

};

}  // namespace loragateway
}  // namespace esphome

#endif
