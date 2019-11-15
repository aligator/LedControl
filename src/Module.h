#ifndef MODULE_H_
#define MODULE_H_

/**
 * Module is the subclass for all modules.
 * Modules define different operating modes.
 * For example the TextModule is for displaying text.
 */
class Module {
public:
    void loop();
    void setup();
private:
    virtual void doLoop() = 0;
    virtual void doSetup() = 0;
};

#endif /* MODULE_H_ */