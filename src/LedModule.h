#ifndef LED_MODULE_H_
#define LED_MODULE_H_

#include "Module.h"
#include "LedStrip.h"

class LedModule: public Module {
public:
    LedModule(LedStrip* led);
	virtual ~LedModule();

protected:   
    LedStrip* led;
};

#endif /* LED_MODULE_H_ */