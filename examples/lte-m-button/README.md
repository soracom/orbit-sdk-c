# SORACOM Orbit SDK for C/C++ - Sample for SORACOM LTE-M Button Series

## Overview
This is a sample code for processing data sent from the SORACOM LTE-M Button series ([SORACOM LTE-M Button Plus](https://soracom.jp/store/5207/) and [SORACOM LTE-M Button for Enterprise](https://soracom.jp/store/5206/)) using [SORACOM Orbit](https://soracom.jp/services/orbit/).

Normally, the following JSON is sent from the SORACOM LTE-M Button series:

```json
{
    "clickType":1,
    "clickTypeName":"SINGLE",
    "batteryLevel":1,
    "binaryParserEnabled":true
}
```

This sample code adds IMSI and name for identifying the SIM, and location information obtained through the [Simple Positioning Feature](https://users.soracom.io/ja-jp/docs/air/get-location-info/#%e3%83%87%e3%83%90%e3%82%a4%e3%82%b9%e3%81%ae%e7%b0%a1%e6%98%93%e7%9a%84%e3%81%aa%e4%bd%8d%e7%bd%ae%e6%83%85%e5%a0%b1-%e7%b0%a1%e6%98%93%e4%bd%8d%e7%bd%ae%e6%b8%ac%e4%bd%8d%e6%a9%9f%e8%83%bd-%e3%82%92%e9%80%81%e4%bf%a1%e3%81%99%e3%82%8b-plan-km1-%e3%81%ae%e3%81%bf) to transform it into the following JSON:

```
{
    "clickType":1,
    "clickTypeName":"SINGLE",
    "batteryLevel":1,
    "binaryParserEnabled":true,
    "imsi":"xxxxxxxxxxxxxxx",
    "name":"name of button",
    "location":{
        "lat":35.12345,
        "lon":138.12345,
    },
    "userdata":"Hello, userdata"
}
```

## How to Run

1. Compile the source code.
   ```
   make
   ```
   You must install [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) in advance to compile.

2. Create a Soralet to be a container for the uploaded code (only for the first time).
   ```
   make create-soralet
   ```
   You need to create a SORACOM account and install [soracom-cli](https://github.com/soracom/soracom-cli) in advance.
   If you want to use a profile other than the default, specify it as an argument:
   ```
   make create-soralet soracom_profile=${profile_name}
   ```
   By default, a Soralet with the ID `lte-m-button` is created. If you want to use a different ID, specify it like this:
   ```
   make create-soralet soralet_id=${soralet_id}
   ```
   You can specify both `soralet_id` and `soracom_profile` at the same time.

3. Upload the compiled code.
   ```
   make upload
   ```
   To override the profile or Soralet ID, follow the same method as in step 2.

4. Test the uploaded code.
   ```
   make test
   ```
   If you see the following result, it's successful:
   ```
   {
     "body": "{\"batteryLevel\":1,\"clickType\":1,\"clickTypeName\":\"SINGLE\",\"imsi\":\"001019999999999\",\"location\":{\"lat\":43.12345,\"lon\":138.112233},\"name\":\"test name\",\"user_data\":\"\"}",
     "contentType": "application/json",
     "encodingType": "plain",
     "resultCode": 0
   }
   ```

5. If it doesn’t work, check the logs.
   ```
   make log
   ```
   The log will be displayed like the following. You can check how far it has been executed or the values stored in variables by comparing it with the messages output by the `soracom_log()` function in the sample code.
   ```
   [
        {
                "createdTime": 1702618252569,
                "message": "hello, orbit!\nreceived data: {\"batteryLevel\":1,\"clickType\":1,\"clickTypeName\":\"SINGLE\"}\nimsi = 001019999999999\nname = test name\nuserdata: \n",
                "operatorId": "OP00xxxxxxxx",
                "soraletId": "lte-m-button",
                "version": 2
        },
   ...
   ]
   ```

6. Once you have confirmed the operation, link the uploaded Soralet to the LTE-M button group settings, and try pressing the button in reality. For details, please refer to the [User Documentation](https://developers.soracom.io/en/docs/orbit/configuration/).
