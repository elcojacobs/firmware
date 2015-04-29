/**
 ******************************************************************************
 * @file    wifi_credentials_reader.cpp
 * @author  Zachary Crockett and Satish Nair
 * @version V1.0.0
 * @date    24-April-2013
 * @brief  
 ******************************************************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#include "wifi_credentials_reader.h"
#include "delay_hal.h"
#include "wlan_hal.h"
#include "system_cloud.h"
#include "system_update.h"
#include "spark_wiring.h"   // for serialReadLine
#include "spark_wiring_wifi.h"

#if PLATFORM_ID > 2
#define SETUP_LISTEN_MAGIC 1
void loop_wifitester(int c);
#include "spark_wiring_usartserial.h"
#include "wifitester.h"
#endif

#ifndef SETUP_LISTEN_MAGIC
#define SETUP_LISTEN_MAGIC 0
#endif

#define SETUP_SERIAL Serial1

class StreamAppender : public Appender
{
    Stream& stream_;
    
public:
    StreamAppender(Stream& stream) : stream_(stream) {}
    
    bool append(const uint8_t* data, size_t length) {
        return stream_.write(data, length)==length;
    }
};

WiFiCredentialsReader::WiFiCredentialsReader(ConnectCallback connect_callback)
{
#if SETUP_OVER_SERIAL1    
    serial1Enabled = false;
    magicPos = 0;
    Serial1.begin(9600);
    this->tester = NULL;
#endif    
    this->connect_callback = connect_callback;
    if (serial.baud()==0)
        serial.begin(9600);
    
}

WiFiCredentialsReader::~WiFiCredentialsReader() 
{
#if SETUP_OVER_SERIAL1    
    delete this->tester;
#endif    
}

void WiFiCredentialsReader::read(void)
{        
#if SETUP_OVER_SERIAL1
    int c = -1;
    if (SETUP_SERIAL.available()) {
        c = SETUP_SERIAL.read();
    }
    if (SETUP_LISTEN_MAGIC) {
        static uint8_t magic_code[] = { 0xe1, 0x63, 0x57, 0x3f, 0xe7, 0x87, 0xc2, 0xa6, 0x85, 0x20, 0xa5, 0x6c, 0xe3, 0x04, 0x9e, 0xa0 };        
        if (!serial1Enabled) {
            if (c>=0) {                
                if (c==magic_code[magicPos++]) {
                    serial1Enabled = magicPos==sizeof(magic_code);
                    if (serial1Enabled) {
                        if (tester==NULL)
                            tester = new WiFiTester();
                        tester->setup(SETUP_OVER_SERIAL1);
                    }
                }
                else {
                    magicPos = 0;
                }
                c = -1;
            }
        }
        else {                
            if (tester)
                tester->loop(c);
        }
    }        
#endif    
    if (serial.available()) {
        int c = serial.read();
        if (c>=0)
            handle((char)c);        
    }
}

void WiFiCredentialsReader::handle(char c)
{
    if ('w' == c)
    {
        memset(ssid, 0, 33);
        memset(password, 0, 65);
        memset(security_type_string, 0, 2);

        print("SSID: ");
        read_line(ssid, 32);

        do
        {
            print("Security 0=unsecured, 1=WEP, 2=WPA, 3=WPA2: ");
            read_line(security_type_string, 1);
        }
        while ('0' > security_type_string[0] || '3' < security_type_string[0]);

#if PLATFORM_ID<3        
        if ('1' == security_type_string[0])
        {
            print("\r\n ** Even though the CC3000 supposedly supports WEP,");
            print("\r\n ** we at Spark have never seen it work.");
            print("\r\n ** If you control the network, we recommend changing it to WPA2.\r\n");
        }
#endif        

        unsigned long security_type = security_type_string[0] - '0';
        if (0 < security_type)
        {
            print("Password: ");
            read_line(password, 64);
        }

        print("Thanks! Wait " 
#if PLATFORM_ID<3
    "about 7 seconds "    
#endif            
            "while I save those credentials...\r\n\r\n");

        connect_callback(ssid, password, security_type);

        print("Awesome. Now we'll connect!\r\n\r\n");
        print("If you see a pulsing cyan light, your "
#if PLATFORM_ID==0            
            "Spark Core"
#else
            "device"
#endif            
            "\r\n");
        print("has connected to the Cloud and is ready to go!\r\n\r\n");
        print("If your LED flashes red or you encounter any other problems,\r\n");
        print("visit https://www.spark.io/support to debug.\r\n\r\n");
        print("    Spark <3 you!\r\n\r\n");
    }
    else if ('i' == c)
    {
#if PLATFORM_ID<3        
        print("Your core id is ");
#else
        print("Your device id is ");
#endif        
        String id = spark_deviceID();
        print(id.c_str());
        print("\r\n");
    }
    else if ('m' == c)
    {
        print("Your core MAC address is\r\n");
        WLanConfig ip_config;
        wlan_fetch_ipconfig(&ip_config);
        uint8_t* addr = ip_config.uaMacAddr;
        print(bytes2hex(addr++, 1).c_str());
        for (int i = 1; i < 6; i++)
        {
            print(":");
            print(bytes2hex(addr++, 1).c_str());
        }
        print("\r\n");
    }
    else if ('f' == c)
    {
        system_serialFirmwareUpdate(&serial);
    }
    else if ('x' == c)
    {
        // exit without changes
        connect_callback(NULL, NULL, 0);
    }
    else if ('s' == c) 
    {
        StreamAppender appender(serial);
        system_module_info(append_instance, &appender);
        print("\r\n");        
    }
    
}

/* private methods */

void WiFiCredentialsReader::print(const char *s)
{
    for (size_t i = 0; i < strlen(s); ++i)
    {
        serial.write(s[i]);
        HAL_Delay_Milliseconds(1); // ridonkulous, but required
    }
}

void WiFiCredentialsReader::read_line(char *dst, int max_len)
{
    serialReadLine(&serial, dst, max_len, 0); //no timeout
    print("\r\n");
    while (0 < serial.available())
        serial.read();
}