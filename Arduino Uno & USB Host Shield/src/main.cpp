/*
  Based on example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
  For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
*/
#include <Arduino.h>
#include <XBOXRECV.h>
#include <SPI.h>
#include <avr/wdt.h>

USB Usb;
XBOXRECV Xbox(&Usb);

//timing and state variables
unsigned long rumbleStartTime = millis();
bool rumbleState = false;
unsigned long batteryStateTime = millis();

void setup()
{
  wdt_disable();

  Serial.begin(115200);

  delay(1000);

  if (Usb.Init() == -1)
  {
    Serial.println(F("OSC did not start"));
    Serial.println(F("STOP"));
    while (1)
      ; //halt
  }

  wdt_enable(WDTO_2S);

  Serial.println(F("Xbox Wireless Receiver Library Started"));
  Serial.println(F("START"));
}

void loop()
{
  Usb.Task();

  if (Xbox.XboxReceiverConnected)
  {
    //We'll deal with multiple controllers another day
    // for (uint8_t i = 0; i < 4; i++)
    // {
    uint8_t i = 0;

    if (Xbox.Xbox360Connected[i])
    {
      //send battery state every 10 seconds
      if (millis() - batteryStateTime >= 10000) //test whether the period has elapsed
      {
        batteryStateTime = millis();
        Serial.print(F("Battery: "));
        Serial.println(Xbox.getBatteryLevel(i)); // The battery level in the range 0-3
      }

      //sort out rumbles...
      //rumble fail safe , turn off after 10 seconds
      if (rumbleState == true)
      {
        if (millis() - rumbleStartTime >= 10000) //test whether the period has elapsed
        {
          Xbox.setRumbleOff(i);
          rumbleState = false;
        }
      }

      //see if there is a command on the Serial line
      if (Serial.available())
      {
        auto command = Serial.readStringUntil('\n');

        Serial.print("Comand Recieved '");
        Serial.print(command);
        Serial.println("'");

        if (command == (String)"setRumbleOn: BIG")
        {
          Xbox.setRumbleOn(1, 0, i);
          rumbleState = true;
          rumbleStartTime = millis();
        }
        if (command == (String)"setRumbleOn: SMALL")
        {
          Xbox.setRumbleOn(0, 1, i);
          rumbleState = true;
          rumbleStartTime = millis();
        }
        if (command == (String)"setRumbleOn: BOTH")
        {
          Xbox.setRumbleOn(1, 1, i);
          rumbleState = true;
          rumbleStartTime = millis();
        }
        if (command == (String)"setRumbleOff")
        {
          Xbox.setRumbleOff(i);
          rumbleState = false;
        }
      }

      if (Xbox.getButtonPress(L2, i))
      {
        Serial.print("L2: ");
        Serial.println(Xbox.getButtonPress(L2, i));
      }

      if (Xbox.getButtonPress(R2, i))
      {
        Serial.print("R2: ");
        Serial.println(Xbox.getButtonPress(R2, i));
      }

      if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500 || Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500 || Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500 || Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500)
      {
        if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500)
        {
          Serial.print(F("LeftHatX: "));
          Serial.println(Xbox.getAnalogHat(LeftHatX, i));
        }
        if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500)
        {
          Serial.print(F("LeftHatY: "));
          Serial.println(Xbox.getAnalogHat(LeftHatY, i));
        }
        if (Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500)
        {
          Serial.print(F("RightHatX: "));
          Serial.println(Xbox.getAnalogHat(RightHatX, i));
        }
        if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500)
        {
          Serial.print(F("RightHatY: "));
          Serial.println(Xbox.getAnalogHat(RightHatY, i));
        }
      }

      if (Xbox.getButtonClick(UP, i))
      {
        //Xbox.setLedOn(LED1, i);
        Serial.println(F("Up"));
      }
      if (Xbox.getButtonClick(DOWN, i))
      {
        //Xbox.setLedOn(LED4, i);
        Serial.println(F("Down"));
      }
      if (Xbox.getButtonClick(LEFT, i))
      {
        //Xbox.setLedOn(LED3, i);
        Serial.println(F("Left"));
      }
      if (Xbox.getButtonClick(RIGHT, i))
      {
        //Xbox.setLedOn(LED2, i);
        Serial.println(F("Right"));
      }

      if (Xbox.getButtonClick(START, i))
      {
        //Xbox.setLedMode(ALTERNATING, i);
        Serial.println(F("Start"));
      }
      if (Xbox.getButtonClick(BACK, i))
      {
        //Xbox.setLedBlink(ALL, i);
        Serial.println(F("Back"));
      }
      if (Xbox.getButtonClick(L3, i))
      {
        Serial.println(F("L3"));
      }
      if (Xbox.getButtonClick(R3, i))
      {
        Serial.println(F("R3"));
      }
      if (Xbox.getButtonClick(L1, i))
      {
        Serial.println(F("L1"));
      }
      if (Xbox.getButtonClick(R1, i))
      {
        Serial.println(F("R1"));
      }
      if (Xbox.getButtonClick(XBOX, i))
      {
        // Xbox.setLedMode(ROTATING, i);

        // Serial.println(F(")"));
        Serial.println(F("XBOX"));
      }
      if (Xbox.getButtonClick(SYNC, i))
      {
        Serial.println(F("XBOX"));
        Xbox.disconnect(i);
      }

      if (Xbox.getButtonClick(A, i))
      {
        Serial.println(F("A"));
      }
      if (Xbox.getButtonClick(B, i))
      {
        Serial.println(F("B"));
      }
      if (Xbox.getButtonClick(X, i))
      {
        Serial.println(F("X"));
      }
      if (Xbox.getButtonClick(Y, i))
      {
        Serial.println(F("Y"));
      }
      //} multiple controller looop
    }

    //reset watchdog timer
    wdt_reset();
  }
  // else
  // {
  //   //XboxReceiverConnected not connected
  //   Serial.println(F("XboxReceiverConnected: False"));

  //   delay(1000);
  // }
  Serial.flush();
}
