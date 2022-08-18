#include "Tube.h";

Tube::Tube(float *sensing_value)
{
    this->float_value_ptr = sensing_value;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->size = 0;
    this->element_empty = true;
    this->token_empty = true;
}

Tube::Tube(int *sensing_value)
{
    this->float_value_ptr = nullptr;
    this->int_value_ptr = sensing_value;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->size = 0;
    this->element_empty = true;
    this->token_empty = true;
}

Tube::Tube(long *sensing_value)
{
    this->float_value_ptr = nullptr;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = sensing_value;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->size = 0;
    this->element_empty = true;
    this->token_empty = true;
}

Tube::Tube(unsigned int *sensing_value)
{
    this->float_value_ptr = nullptr;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = sensing_value;
    this->unsigned_long_value_ptr = nullptr;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->size = 0;
    this->element_empty = true;
    this->token_empty = true;
}

Tube::Tube(unsigned long *sensing_value)
{
    this->float_value_ptr = nullptr;
    this->int_value_ptr = nullptr;
    this->long_value_ptr = nullptr;
    this->unsigned_int_value_ptr = nullptr;
    this->unsigned_long_value_ptr = sensing_value;
    this->token = "init";
    this->channel = "init";
    this->name = "init";
    this->size = 0;
    this->element_empty = true;
    this->token_empty = true;
}

Tube::~Tube()
{
}

bool Tube::begin(String channel_, String name_)
{
    channel = channel_;
    name = name_;

    // TODO: Get TUBE record if channel and name exist

    token = "token";
    token_empty = false;
    return true;
}

bool Tube::append(timestamp_tz_t timestamp)
{
    bool add = false;
    //TODO : Test
    if (float_value_ptr != nullptr)
    {
        add = element.append(float_value_ptr, timestamp);
        size += 1;
        element_empty = false;
    }
    else if (int_value_ptr != nullptr)
    {
        add = element.append(int_value_ptr, timestamp);
        size += 1;
        element_empty = false;
    }
    else if (long_value_ptr != nullptr)
    {
        add = element.append(long_value_ptr, timestamp);
        size += 1;
        element_empty = false;
    }
    else if (unsigned_int_value_ptr != nullptr)
    {
        add = element.append(unsigned_int_value_ptr, timestamp);
        size += 1;
        element_empty = false;
    }
    else if (unsigned_long_value_ptr != nullptr)
    {
        add = element.append(unsigned_long_value_ptr, timestamp);
        size += 1;
        element_empty = false;
    }
    else 
    {
        //Error
    }
    return add;
}


http_code_t Tube::send()
{
    if (!token_empty)
    {
        // TODO: Post Tube record
        // use ArduinoJson
        /*
        if (float_value_ptr != nullptr)
        {
            Serial.println("float");
        }
        else if (int_value_ptr != nullptr)
        {
            Serial.println("int");
        }
        else if (long_value_ptr != nullptr)
        {
            Serial.println("long");
        }
        else if (unsigned_int_value_ptr != nullptr)
        {
            Serial.println("unsigned int");
        }
        else if (unsigned_long_value_ptr != nullptr)
        {
            Serial.println("unsigned long");
        }
        else
        {
            Serial.println("unknown");
        }
        */
    }
    size = 0;
    element_empty = true;
    http_code_t r = "201";
    return r;
}

void Tube::fetch(float *fresh_value)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value = 3.9;
}

void Tube::fetch(int *fresh_value)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value = -39;
}

void Tube::fetch(long *fresh_value)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value = -339;
}

void Tube::fetch(unsigned int *fresh_value)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value = 39;
}

void Tube::fetch(unsigned long *fresh_value)
{
    // TODO: fetch from "/rest/v1"  +  "/fresh_elements"
    *fresh_value = 339;
}