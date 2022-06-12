![logo](img/canaspad_logo.png)
# SDK for Canaspad API
![GitHub](https://img.shields.io/github/license/spadr/Canaspad_ESP32_Library)

Canaspad is an IoT data analytics infrastructure! You can send and receive data as well as control a microcontroller by installing the SDK in your Arduino IDE and signing up at canaspad.com.

# Installation
1. Register with your email address and password at canaspad.com
2. Download the library as a ZIP file from this repository.
![download ZIP file](img/step1.png)

3. Open the Arduino IDE and install the library.
![include sdk from ZIP file](img/step2.png)

# Reference
## Canaspad::domain()



### Description

Select the location where you want your data stored. If you use [canaspad.com](http://canaspad.com/), you do not need to change this option.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::void domain(String domain);
```

### Argument

- String **domain**
    
    ex. “[own.server.com](http://own.server.com/)”  ,  “192.168.10.2”
    

### Return value

- None
    
    

## Canaspad::wifi()



### Description

Setup a WiFi access point.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::void wifi(const char* ssid, const char* password);
```

### Argument

- const char* **ssid**
    
    
    
- const char* **password**
    
    
    

### Return value

- None
    

    

## Canaspad::begin()



### Description

Check the connection to the cloud server.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::bool begin(const char* api_username, const char* api_password, int UTC_offset=9);
```

### Argument

- const char* **api_username**
    
    
    
- const char* **api_password**
    
    
    
- int **UTC_offset**=9
    
    Time difference between UTC and your local time.
    

### Return value

- bool **true**
    
    Success
    
- bool **false**
    
    Error
    

## Canaspad::set()



### Description

For each channel and series name, a token is generated that identifies where the data will be kept on the server.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::String set(String device_name, String device_channel, String data_type="number", bool alive_monitoring=false, int alive_monitoring_interval=30);
```

### Argument

- String **device_name**
    
    Data Series Name
    
- String **device_channel**
    
    Data channel name
    
- String **data_type**="number"
    
    Data Type 
    
- bool **alive_monitoring**=false
    
    dead/ alive monitoring on/off.
    
- int **alive_monitoring_interval**=30
    
    Periodic check to see whether the device is dead or alive (minutes).
    

### Return value

- String **device_token**
    
    Token indicating the storage location on the server
    

## Canaspad::add()



### Description

Enter sensor readings into the microcontroller's database.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::bool add(String token, float value);
```

### Argument

- String **token**
    
    
- float **value**
    


### Return value

- bool **true**
    
    Success
    
- bool **false**
    
    Error
    

## Canaspad::send()



### Description

The data from the database of the microcomputer is sent to the server.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::String send();
```

### Argument

None

### Return value

- bool **true**
    
    Success
    
- bool **false**
    
    Error
    

## Canaspad::get()



### Description

Returns the most recent value from the selected data series based on the token.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::float get(String token);
```

### Argument

- String **token**
    
    
    

### Return value

- float **value**
    
    
    

## Canaspad::gettime()



### Description

The current time is shown.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::String gettime();
```

### Argument

- None

### Return value

- String **datetime**
    
    current time
    
    ex. 2022/06/06(Mon)17:25:00
    

## Canaspad::gettimestamp()



### Description

The current time is shown in UNIX time.

### Syntax

```arduino
#include “Canaspad.h”
Canaspad::unsigned long gettimestamp();
```

### Argument

- None

### Return value

- unsigned long **TimeStamp**
    
   