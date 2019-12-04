#ifndef FULLBLINKMODULE_H_
#define FULLBLINKMODULE_H_

#include "Arduino.h"
#include "LedStrip.h"
#include "LedModule.h"

class FullBlinkModule: public LedModule {
public:
    FullBlinkModule(LedStrip* led);
    virtual ~FullBlinkModule();
    
private:
    bool currentlyOn = false;

    virtual bool doProcessMqtt(String topic, String message) override;
    virtual const char* doGetModuleTopic() override;
    virtual void doLoop() override;
    virtual void doSetup() override;
};

#endif /* FULLBLINKMODULE_H_ */