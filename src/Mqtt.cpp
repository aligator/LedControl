#include "Mqtt.h"
#include "Util.h"
#include <functional>
#include <ArduinoJson.h>

#define TOPIC_ALL MQTT_TOPIC "/#"
#define TOPIC_SET_BRIGHTNESS MQTT_TOPIC "/set/brightness"
#define TOPIC_SET_FPS MQTT_TOPIC "/set/fps"
#define TOPIC_SET_COLOR MQTT_TOPIC "/set/color"
#define TOPIC_ADD_COLOR MQTT_TOPIC "/add/color"
#define TOPIC_SET_SWITCH MQTT_TOPIC "/set/switch"

#define TOPIC_GET_BRIGHTNESS MQTT_TOPIC "/get/brightness"
#define TOPIC_GET_SWITCH MQTT_TOPIC "/get/switch"

#define DISCOVERY_MESSAGE_MAX_SIZE 1024

/**
 * converts bytes to a String
 */
String Mqtt::_byte2str(byte *data, unsigned int length)
{
    char *buffer = (char *)malloc(length + 1);
    memcpy(buffer, data, length);
    buffer[length] = 0;

    // or for unicode:
    // const char* str((const wchar_t*)buffer);
    String out = String((const char *)buffer);
    free(buffer);
    return out;
}

void Mqtt::_receiveCallback(char *topic, byte *payload, unsigned int length)
{
    String message = Mqtt::_byte2str(payload, length);
    String topicString = String(topic);

    // first check global settings
    if (topicString.equals(TOPIC_SET_BRIGHTNESS))
    {
        if (Util::isValidInt(message))
        {
            int brightness = message.toInt();
            if (brightness >= 0 && brightness <= 255)
            {
                led->setBrightness(brightness);
                this->client.publish(TOPIC_GET_BRIGHTNESS, message.c_str(), true);
            }
            else
            {
                Serial.println("the brightness has to be an integer from 0 to 255");
            }
        }
        return;
    }
    else if (topicString.equals(TOPIC_SET_FPS))
    {
        // -1 is fps disabled -> as fast as it can
        if (Util::isValidFloat(message))
        {
            float fps = message.toFloat();
            if (fps > 0.0 && fps <= 500.0)
            {
                led->setFps(fps);
            }
            else
            {
                Serial.println("the fps has to be an integer from -1 to 500");
            }
        }
        return;
    }
    else if (topicString.equals(TOPIC_ADD_COLOR))
    {
        // numeric value
        if (Util::isValidInt(message))
        {
            int color = message.toInt();
            if (color >= 0 && color <= 0xFFFFFF)
            {
                color = led->matrix.Color((color & 0xFF0000) >> 16,
                                          (color & 0x00FF00) >> 8,
                                          color & 0x0000FF);

                led->addColor((uint16_t)color);
            }
            else
            {
                Serial.println("The color has to be an integer from 0 to 16777215");
            }
            return;
        }

        // json array
        // for JSON_ARRAY_SIZE(3);
        StaticJsonDocument<48> doc = StaticJsonDocument<48>();
        deserializeJson(doc, message);

        if (doc.is<JsonArray>())
        {
            JsonArray color = doc.as<JsonArray>();

            if (color.size() == 3)
            {
                if (color[0].is<short>() && (short)color[0] >= 0 && (short)color[0] <= 255 &&
                    color[1].is<short>() && (short)color[1] >= 0 && (short)color[1] <= 255 &&
                    color[2].is<short>() && (short)color[2] >= 0 && (short)color[2] <= 255)
                {

                    short r = color[0];
                    short g = color[1];
                    short b = color[2];

                    led->addColor(led->matrix.Color(r, g, b));
                }
                else
                {
                    Serial.println("the color values have to be integers from 0 to 255");
                }
            }
            else
            {
                Serial.println("the colors have to be arrays of 3 values (r, g, b)");
            }
        }
        else
        {
            // "rrr,ggg,bbb"
            // e.g. 0,100,255

            // TODO: There is a bug in the util split, as it cannot read the 'last' part
            message.concat(",0");
            String r = Util::split(message, ',', 0);
            String g = Util::split(message, ',', 1);
            String b = Util::split(message, ',', 2);

            if (Util::isValidInt(r) && Util::isValidInt(g) && Util::isValidInt(b))
            {
                led->addColor(led->matrix.Color(r.toInt(), g.toInt(), b.toInt()));
            }
            else
            {
                Serial.println("you have to pass an array of color-arrays or one color as integer");
            }
        }

        return;
    }
    else if (topicString.equals(TOPIC_SET_COLOR))
    {
        // e.g. only one color using th numeric value or "[[255,0,0],[0,0,255]]"

        if (Util::isValidInt(message))
        {
            int color = message.toInt();
            if (color >= 0 && color <= 0xFFFFFF)
            {
                color = led->matrix.Color((color & 0xFF0000) >> 16,
                                          (color & 0x00FF00) >> 8,
                                          color & 0x0000FF);

                led->setColors(std::vector<uint16_t>{(uint16_t)color});
            }
            else
            {
                Serial.println("The color has to be an integer from 0 to 16777215");
            }
            return;
        }

        // for 9*JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(9);
        StaticJsonDocument<560> doc = StaticJsonDocument<560>();
        deserializeJson(doc, message);

        std::vector<uint16_t> colors = std::vector<uint16_t>();

        if (doc.is<JsonArray>())
        {
            JsonArray colorArray = doc.as<JsonArray>();

            for (JsonVariant colorVariant : colorArray)
            {
                if (!colorVariant.is<JsonArray>())
                {
                    Serial.println("the colors have to be arrays of 3 values (r, g, b)");
                    return;
                }

                JsonArray color = colorVariant.as<JsonArray>();
                if (color.size() == 3)
                {
                    if (color[0].is<short>() && (short)color[0] >= 0 && (short)color[0] <= 255 &&
                        color[1].is<short>() && (short)color[1] >= 0 && (short)color[1] <= 255 &&
                        color[2].is<short>() && (short)color[2] >= 0 && (short)color[2] <= 255)
                    {

                        short r = color[0];
                        short g = color[1];
                        short b = color[2];

                        colors.push_back(led->matrix.Color(r, g, b));
                    }
                    else
                    {
                        Serial.println("the color values have to be integers from 0 to 255");
                    }
                }
                else
                {
                    Serial.println("the colors have to be arrays of 3 values (r, g, b)");
                }
            }

            led->setColors(colors);
        }
        else
        {
            // "rrr,ggg,bbb"
            // e.g. 0,100,255

            // TODO: There is a bug in the util split, as it cannot read the 'last' part
            message.concat(",0");
            String r = Util::split(message, ',', 0);
            String g = Util::split(message, ',', 1);
            String b = Util::split(message, ',', 2);

            if (Util::isValidInt(r) && Util::isValidInt(g) && Util::isValidInt(b))
            {
                colors.push_back(led->matrix.Color(
                    r.toInt(),
                    g.toInt(),
                    b.toInt()));

                led->setColors(colors);
            }
            else
            {
                Serial.println("you have to pass an array of color-arrays or one color as integer");
            }
        }
    }
    else if (topicString.equals(TOPIC_SET_SWITCH)) 
    {
        this->led->enabled = message.equals("ON");

        if (this->led->enabled) {
            this->client.publish(TOPIC_GET_SWITCH, "ON");
        } else {
            this->client.publish(TOPIC_GET_SWITCH, "OFF");
        }
    }
    else
    {
        // then check modules
        for (uint i = 0; i < this->modules.size(); i++)
        {
            int baseTopicLength = strlen(MQTT_TOPIC) + strlen(this->modules[i]->getModuleTopic());
            if (topicString.startsWith(String(MQTT_TOPIC) + String(this->modules[i]->getModuleTopic())))
            {
                // enable module as soon as the topic starts with the module topic
                this->led->loadModule(this->modules[i]);
                if (this->modules[i]->processMqtt(topicString.substring(baseTopicLength), message))
                {
                    return;
                }

                break;
            }
        }
    }
}

Mqtt::Mqtt(LedStrip *led)
{
    this->led = led;
    this->modules = std::vector<Module *>{
        new TextModule(led),
        new FullBlinkModule(led)};
    this->client = PubSubClient(this->espClient);
}

Mqtt::~Mqtt()
{
    for (int i = 0; (short)i < (short)this->modules.size(); i++)
    {
        this->led->loadModule(0);
        delete this->modules[i];
        this->modules.pop_back();
    }
}

void Mqtt::setup()
{
    delay(10);
    this->client.setServer(broker, port);
    this->client.setCallback(std::bind(&Mqtt::_receiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    if (this->modules.size() > 0)
    {
        this->led->loadModule(this->modules[0]);
    }
}

void Mqtt::_reconnect()
{
    if (!client.connected())
    {
        Serial.print("Reconnecting...");

        // Attempt to connect
        if (!client.connect(this->clientId, user, password))
        {
            Serial.print("failed, rc=");
            Serial.println(client.state());
        }
        else
        {
            Serial.println("Connected");
            delay(100);

            Serial.println("Sending HA discovery.");
            DynamicJsonDocument doc(DISCOVERY_MESSAGE_MAX_SIZE);

            doc["name"] = "LED Matrix Light";
            doc["stat_t"] = TOPIC_GET_SWITCH;
            doc["command_topic"] = TOPIC_SET_SWITCH;
            doc["brightness_state_topic"] = TOPIC_GET_BRIGHTNESS;
            doc["brightness_command_topic"] = TOPIC_SET_BRIGHTNESS;
            doc["rgb_command_topic"] = TOPIC_SET_COLOR;
            doc["payload_on"] = "ON";
            doc["payload_off"] = "OFF";
            doc["retain"] = true;
            doc["icon"] = "mdi:led-strip-variant";
            this->sendHADiscoveryMsg(doc, "light", "led_matrix");
            doc.clear();

            doc["name"] = "LED Matrix Add Color";
            doc["command_topic"] = TOPIC_ADD_COLOR;
            doc["icon"] = "mdi:led-strip-variant";
            this->sendHADiscoveryMsg(doc, "text", "led_matrix_add_color");
            doc.clear();

            doc["name"] = "LED Matrix Set Color";
            doc["command_topic"] = TOPIC_SET_COLOR;
            doc["icon"] = "mdi:led-strip-variant";
            this->sendHADiscoveryMsg(doc, "text", "led_matrix_set_color");
            doc.clear();

            doc["name"] = "LED Matrix Set FPS";
            doc["command_topic"] = TOPIC_SET_FPS;
            doc["icon"] = "mdi:led-strip-variant";
            doc["min"] = -1.0;
            doc["max"] = 500.0;
            doc["step"] = 0.1;
            this->sendHADiscoveryMsg(doc, "number", "led_matrix_set_fps");
            doc.clear();

            doc["name"] = "LED Matrix Set Brightness";
            doc["command_topic"] = TOPIC_SET_BRIGHTNESS;
            doc["icon"] = "mdi:led-strip-variant";
            doc["min"] = 0;
            doc["max"] = 255;
            this->sendHADiscoveryMsg(doc, "number", "led_matrix_set_brightness");
            doc.clear();


            // Send the discovery for the modules.
            for (uint i = 0; i < this->modules.size(); i++)
            {
                int iteration = 0;
                bool hasNext = true;
                while (hasNext) {
                    char type[30];
                    type[0] = '\0';

                    char objectName[30];
                    hasNext = this->modules[i]->nextDiscoveryMessage(iteration, MQTT_TOPIC, &doc, type, objectName);

                    if (type[0] != '\0') {
                        this->sendHADiscoveryMsg(doc, type, objectName);
                        doc.clear();
                    }

                    iteration++;
                }
            }

            Serial.println("DONE!");

            delay(100);
            client.subscribe(TOPIC_ALL);
        }
    }
}

void Mqtt::sendHADiscoveryMsg(DynamicJsonDocument doc, const char *type, const char *objectName)
{
    String objectId = String(this->clientId);
    objectId.concat(objectName);

    doc["uniq_id"] = objectId;

    String discoveryTopic = String("homeassistant/");
    discoveryTopic.concat(type);
    discoveryTopic.concat("/");
    discoveryTopic.concat(objectId);
    discoveryTopic.concat("/config");

    char buffer[DISCOVERY_MESSAGE_MAX_SIZE];

    size_t n = serializeJson(doc, buffer);
    String size = String(n);

    bool ok = this->client.setBufferSize(DISCOVERY_MESSAGE_MAX_SIZE);
    if (ok) {
        this->client.publish(discoveryTopic.c_str(), buffer, n);
        this->client.setBufferSize(MQTT_MAX_PACKET_SIZE);
    }
}

void Mqtt::loop()
{
    if (!client.connected())
    {
        _reconnect();
    }
    else
    {
        client.loop();
    }
}