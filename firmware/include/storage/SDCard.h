#pragma once
#include <Arduino.h>
#include <WString.h>

class SDCard
{
public:
    bool begin();

    // bool bo chcemy wiedziec czy sie udalo czy nie
    bool writeLine(String filePath,String text);
    
    bool exists(const String& path);

};