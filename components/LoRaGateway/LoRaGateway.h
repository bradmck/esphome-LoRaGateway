#ifndef LORAGATEWAY
#define LORAGATEWAY


#include "esphome.h"

#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace loragateway {

static const char *const TAG = "TDisplayS3";

class LoRaGateway : public PollingComponent
{
  public:
    void setup() override {

    }

    void loop() override {
    }





  private:

};

}  // namespace loragateway
}  // namespace esphome

#endif
