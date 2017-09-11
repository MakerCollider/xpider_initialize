#include "xpider_info.h"
#include "xpider_memory.h"

#define UUID 116
#define HARDWARE_VERSION "v1.0.2"

XpiderInfo g_xpider_info;
XpiderMemory* g_xpider_memory;

void PrintCustomData() {
  Serial.println("= = = = = = Custom Data = = = = = =");
  Serial.print("Name: ");
  Serial.println(g_xpider_info.custom_data_.name);
  Serial.print("Hardware Version: ");
  Serial.println(g_xpider_info.custom_data_.hardware_version);
  Serial.print("UUID: ");
  Serial.println(g_xpider_info.custom_data_.uuid);
  Serial.println("= = = = = = = = = = = = = = = = = =");
}

void Step1() {
  if(SerialFlash.exists(XpiderMemory::kCustomDataFileName)) {
    Serial.println("Custom data exists, Get Data......");
    if(g_xpider_memory->OpenDataFile()) {
      if(g_xpider_memory->ReadCustomData(&g_xpider_info.custom_data_)) {
        PrintCustomData();
        Serial.println("Erase exists data? [input [e] or exit]");
        while (!Serial.available());
        char input_select = Serial.read();
        if(input_select == 'e') {
          Serial.print("Erase and initialize custom data......");
          g_xpider_memory->EraseFlash();
          g_xpider_memory->InitializeCustomData();
          Serial.println("Done!");
        }
      } else {
        Serial.println("Read data fail");
        Serial.print("Initialize Custom data......");
        g_xpider_memory->InitializeCustomData();
        Serial.println("Done!");
      }
    } else {
      Serial.println("Open data file fail");
      Serial.print("Initialize Custom data......");
      g_xpider_memory->InitializeCustomData();
      Serial.println("Done!");
    }
  } else {
    Serial.println("Custom data does not exists");
    Serial.print("Initialize Custom data......");
    g_xpider_memory->InitializeCustomData();
    Serial.println("Done!");
  }
}

void Step2() {
  String temp_string;
  g_xpider_info.custom_data_.uuid = UUID;

  /* Initialize Name by UUID */
  temp_string = "Xpider_" + String(UUID);
  temp_string.toCharArray(g_xpider_info.custom_data_.name, temp_string.length()+1);

  temp_string = HARDWARE_VERSION;
  temp_string.toCharArray(g_xpider_info.custom_data_.hardware_version, temp_string.length()+1);

  if(g_xpider_memory->WriteCustomData(&g_xpider_info.custom_data_)) {
    if(g_xpider_memory->ReadCustomData(&g_xpider_info.custom_data_)) {
      Serial.println("Write data success");
      PrintCustomData();
    }
  } else {
    Serial.println("Write failed");
  }

  Serial.println("Done!");
}

void setup() {
  Serial.begin(115200);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  while (!Serial);
  
  // wait for ready
  // Serial.println("*** Send any character to begin ***");
  // while (Serial.available() && Serial.read());      // empty buffer
  // while (!Serial.available());                       // wait for data
  // while (Serial.available() && Serial.read());      // empty buffer again

  Serial.println("                           __   __      _     _           ");
  Serial.println("                           \\ \\ / /     (_)   | |          ");
  Serial.println("                            \\ V / _ __  _  __| | ___ _ __ ");
  Serial.println("                             > < | '_ \\| |/ _` |/ _ \\ '__|");
  Serial.println("                            / . \\| |_) | | (_| |  __/ |   ");
  Serial.println("                           /_/ \\_\\ .__/|_|\\__,_|\\___|_|   ");
  Serial.println("                                 | |                      ");
  Serial.println("                                 |_|        ");
  Serial.println("            Copyright (C) 2015-2017, Maker Collider, All Rights Reserved.");

  g_xpider_memory = XpiderMemory::instance();
  g_xpider_memory->Initialize();
  
  Serial.println("      = = = = = = = = = = = Xpider Memory Initialize Tool = = = = = = = = = = = =\n\n\n");
  
  Serial.println("## 1    Checking customs data file");
  Step1();
  Serial.println("## 2    Setting Custom data");
  Step2();
  Serial.println("## 3    All Finish");
}

void loop() {
  // put your main code here, to run repeatedly:

}
