#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <MouseTo.h>
//Define my special keys
typedef struct {
  String key;
  int value;
} specialKey;
const specialKey keySpecials[] {
    {"KEY_LEFT_CTRL", KEY_LEFT_CTRL},
    {"KEY_LEFT_SHIFT", KEY_LEFT_SHIFT},
    {"KEY_LEFT_ALT", KEY_LEFT_ALT},
    {"KEY_LEFT_GUI", KEY_LEFT_GUI},
    {"KEY_RIGHT_CTRL", KEY_RIGHT_CTRL},
    {"KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT},
    {"KEY_RIGHT_ALT", KEY_RIGHT_ALT},
    {"KEY_RIGHT_GUI", KEY_RIGHT_GUI},
    {"KEY_UP_ARROW", KEY_UP_ARROW},
    {"KEY_DOWN_ARROW", KEY_DOWN_ARROW},
    {"KEY_LEFT_ARROW", KEY_LEFT_ARROW},
    {"KEY_RIGHT_ARROW", KEY_RIGHT_ARROW},
    {"KEY_BACKSPACE", KEY_BACKSPACE},
    {"KEY_TAB", KEY_TAB},
    {"KEY_RETURN", KEY_RETURN},
    {"KEY_ESC", KEY_ESC},
    {"KEY_INSERT", KEY_INSERT},
    {"KEY_DELETE", KEY_DELETE},
    {"KEY_PAGE_UP", KEY_PAGE_UP},
    {"KEY_PAGE_DOWN", KEY_PAGE_DOWN},
    {"KEY_HOME", KEY_HOME},
    {"KEY_END", KEY_END},
    {"KEY_CAPS_LOCK", KEY_CAPS_LOCK},
    {"KEY_F1", KEY_F1},
    {"KEY_F2", KEY_F2},
    {"KEY_F3", KEY_F3},
    {"KEY_F4", KEY_F4},
    {"KEY_F5", KEY_F5},
    {"KEY_F6", KEY_F6},
    {"KEY_F7", KEY_F7},
    {"KEY_F8", KEY_F8},
    {"KEY_F9", KEY_F9},
    {"KEY_F10", KEY_F10},
    {"KEY_F11", KEY_F11},
    {"KEY_F12", KEY_F12},
    {"KEY_F13", KEY_F13},
    {"KEY_F14", KEY_F14},
    {"KEY_F15", KEY_F15},
    {"KEY_F16", KEY_F16},
    {"KEY_F17", KEY_F17},
    {"KEY_F18", KEY_F18},
    {"KEY_F19", KEY_F19},
    {"KEY_F20", KEY_F20},
    {"KEY_F21", KEY_F21},
    {"KEY_F22", KEY_F22},
    {"KEY_F23", KEY_F23},
    {"KEY_F24", KEY_F24},
    {"KEY_MENU_BACK", 0xB1}
};

String content = "";
char character;
char separator = '\t';
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Keyboard.begin();
  Mouse.begin();
  MouseTo.setCorrectionFactor(1);
}
void loop() {
  while (Serial1.available())
  {
    character = Serial1.read();
    if(character != '\n'){
    content.concat(character);
  }
    if (character == '\n')
    {
    String cmd = getValue(content, separator, 0);
    if(cmd == "WriteLine")
    {
      String arg = getValue(content, separator, 1);
      Keyboard.println(arg);
    }
    if(cmd == "Write")
    {
      String arg = getValue(content, separator, 1);
      const char* toType = arg.c_str();
      int len = strlen(toType);
      for (int i = 0; i < len; i++)
      {
        char curChar = toType[i];
        Keyboard.write(curChar);
        delay(50);
      }
    }
    if(cmd == "PressSpecial")
    {
      String arg = getValue(content, separator, 1);
      for(uint8_t i = 0; i < sizeof(keySpecials)/sizeof(specialKey); ++i) {
          if(keySpecials[i].key==arg)
          {
            Keyboard.press(keySpecials[i].value);
          }
      }
    }
    if(cmd == "Press")
    {
      String arg = getValue(content, separator, 1);
      Keyboard.press(arg.c_str()[0]);
    }
    if(cmd == "MouseMoveTo")
    {

      String s_x = getValue(content, separator, 1);
      String s_y = getValue(content, separator, 2);
      int x = atoi(s_x.c_str());
      int y = atoi(s_y.c_str());
      MouseTo.setTarget(x, y);
      while (MouseTo.move() == false) {}
    }
    if(cmd == "MouseMove")
    {

      String s_x = getValue(content, separator, 1);
      String s_y = getValue(content, separator, 2);
      int x = atoi(s_x.c_str());
      int y = atoi(s_y.c_str());
      Mouse.move(x, y);
    }
    if(cmd == "MouseLClick")
    {
      Mouse.click(MOUSE_LEFT);
    }
    if(cmd == "MouseRClick")
    {
      Mouse.click(MOUSE_RIGHT);
    }
    if(cmd == "MouseMClick")
    {
      Mouse.click(MOUSE_MIDDLE);
    }
    if(cmd == "MouseWheel")
    {
      String steps = getValue(content, separator, 1);
      String direction = getValue(content, separator, 2);
      int int_steps= atoi(steps.c_str());
      if(direction =="Up")
      {
        Mouse.move(0,0, 0-int_steps);
      }
      if(direction =="Down")
      {
        Mouse.move(0,0, int_steps);
      }

    }
    if(cmd == "MouseDrag")
    {

      String s_x1 = getValue(content, separator, 1);
      String s_y1 = getValue(content, separator, 2);
      String s_x2 = getValue(content, separator, 3);
      String s_y2 = getValue(content, separator, 4);
      int x1 = atoi(s_x1.c_str());
      int y1 = atoi(s_y1.c_str());
      int x2 = atoi(s_x2.c_str()) - atoi(s_x1.c_str());
      int y2 = atoi(s_y2.c_str()) - atoi(s_y1.c_str());
      MouseTo.setTarget(x1, y1);
      while (MouseTo.move() == false) {}
      Mouse.press(MOUSE_LEFT);
      Mouse.move(x2, y2);
      delay(300);
      Mouse.release(MOUSE_LEFT);
    }


    if(cmd == "Release")
    {
      Keyboard.releaseAll();
    }
    if(cmd == "Special")
    {
      String arg = getValue(content, separator, 1);
      for(uint8_t i = 0; i < sizeof(keySpecials)/sizeof(specialKey); ++i) {
          if(keySpecials[i].key==arg)
          {
            Keyboard.write(keySpecials[i].value);
          }
      }
    }
    if(cmd == "Raw")
    {
      String arg = getValue(content, separator, 1);
      int key_int = atoi(arg.c_str());
      Keyboard.write(key_int);
    }
    if(cmd == "LongSpecial")
    {
      String arg = getValue(content, separator, 1);
      String time_delay = getValue(content, separator, 2);
      int int_delay = atoi(time_delay.c_str());
      for(uint8_t i = 0; i < sizeof(keySpecials)/sizeof(specialKey); ++i) {
          if(keySpecials[i].key==arg)
          {
            Keyboard.press(keySpecials[i].value);
            delay(int_delay*1000);
            Keyboard.releaseAll();
          }
      }
    }
    if(cmd == "Ctrl")
    {
      String arg = getValue(content, separator, 1);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(arg.c_str()[0]);
      delay(100);
      Keyboard.releaseAll();
    }
    content = "";
    }
  }
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
