# SORACOM Orbit SDK for C/C++ - Sample for GPS Multi Unit SORACOM Edition (Battery Built-in Type)

## Overview
This is sample code for processing data sent from the [GPS Multi Unit SORACOM Edition (Battery Built-in Type)](https://soracom.jp/store/5235/) (hereafter, GPS Multi Unit) using [SORACOM Orbit](https://soracom.jp/services/orbit/).

Normally, the GPS Multi Unit sends the following JSON:

```
{
   "bat":3,
   "humi":68.2,
   "lat":35.12345,
   "lon":139.12345,
   "rs":0,
   "temp":10.5,
   "type":0,
   "x":64.0,
   "y":-64.0,
   "z":1024.0
}
```

This sample performs geofencing based on the GPS location information (`lat` and `lon`) included in this JSON.
Specifically, it adds a `distance_in_km` field indicating the distance from the center coordinates, an `inside_area` field indicating whether it is within a certain distance from the center coordinates, and also adds the IMSI and name for identifying the SIM.

```
{
   "bat":3,
   "distance_in_km":1.7727168801439308,
   "humi":68.2,
   "imsi":"44010xxxxxxxxxx",
   "inside_area":0,
   "lat":35.12345,
   "lon":139.12345,
   "name":"My GPS Multi-unit",
   "rs":0,
   "temp":10.5,
   "type":0,
   "userdata":"Hello, userdata",
   "x":64.0,
   "y":-64.0,
   "z":1024.0
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
   If you want to use a profile other than default, specify it as an argument:
   ```
   make create-soralet soracom_profile=${profile_name}
   ```

   By default, a Soralet with the ID `gps-multi-unit` is created. If you want to use a different ID, specify it like this:
   ```
   make create-soralet soralet_id=${soralet_id}
   ```
   You can specify both `soralet_id` and `soracom_profile`.

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
        "body": "{\"bat\":3,\"distance_in_km\":1.7727168801439308,\"humi\":68.2,\"imsi\":\"44010xxxxxxxxxx\",\"inside_area\":0,\"lat\":35.12345,\"lon\":139.12345,\"name\":\"My GPS Multi-unit\",\"rs\":0,\"temp\":10.5,\"type\":0,\"user_data\":\"Hello, userdata\",\"x\":64.0,\"y\":-64.0,\"z\":1024.0}",
        "contentType": "application/json",
        "encodingType": "plain",
        "resultCode": 0
   }
   ```

5. If it doesn’t work, check the logs.
   ```
   make log
   ```
   The log will be displayed like this. You can check how far it has been executed or the values stored in variables by comparing it with the messages output by the `soracom_log()` function in the sample code.
   ```
   [
        {
                "createdTime": 1702627747702,
                "message": "hello, orbit!\nreceived data: {\"bat\":3,\"humi\":68.2,\"lat\":35.12345,\"lon\":139.12345,\"rs\":0,\"temp\":10.5,\"type\":0, \"x\":64.0,\"y\":-64.0,\"z\":1024.0}\nimsi = 44010xxxxxxxxxx\nname = My GPS Multi-unit\ncenter_lat_str = 35.11111\ncenter_lon_str = 139.11111\nradius = 0.5\ncenter: 35.111110, 139.111110\npos: 35.123450, 139.123450\nuserdata: Hello, userdata\n",
                "operatorId": "OP00xxxxxxxx",
                "soraletId": "gps-multi-unit",
                "version": 2
        },
   ...
   ]
   ```

6. Once you have confirmed the operation, link the uploaded Soralet to the GPS Multi Unit group settings. For details on how to set it up, please refer to the [User Documentation](https://developers.soracom.io/en/docs/orbit/configuration/).
   Then, specify the `center_lat` and `center_lon` to represent the center coordinates, and `radius` to represent the radius from there, in the tags of the SIM. Obtain and set the latitude and longitude of the center coordinates from sources like Google Maps.
   Afterwards, try transmitting GPS coordinates from the GPS Multi Unit.
