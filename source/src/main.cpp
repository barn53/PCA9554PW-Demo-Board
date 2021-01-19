#include <Arduino.h>
#include <Wire.h>

void scan()
{
    Serial.printf("Start I2C scanner ...\n\n");
    byte count(0);

    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found I2C Device: ");
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.println(")");
            ++count;
            delay(1);
        }
    }
    Serial.print("\r\n");
    Serial.println("Finish I2C scanner");
    Serial.print("Found ");
    Serial.print(count, HEX);
    Serial.println(" Device(s).");
}

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    scan();
    Serial.println();
}

class Once {
public:
    operator bool()
    {
        bool was(flag);
        flag = false;
        return was;
    }

private:
    bool flag { true };
};
Once once;

bool toggle(false);
size_t count(0);
uint8_t sequence[] = {
    0b11111110,
    0b11111101,
    0b11111011,
    0b11110111,
    0b11101111,
    0b11011111,
    0b10111111,
    0b01111111,
    0b00111111,
    0b10011111,
    0b11001111,
    0b11100111,
    0b11110011,
    0b11111001,
    0b11111100,
    0b11111110,
    0b11111111,
    0b11101111,
    0b11000111,
    0b10000011,
    0b10000001,
    0b00000000,
    0b10000001,
    0b10000011,
    0b11000111,
    0b11101111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00010000,
    0b00000000,
    0b00010000,
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
};

void loop()
{
    if (once) {
        Serial.printf("sizeof(sequence): %d\n", sizeof(sequence));

        Wire.beginTransmission(0x27);
        Wire.write(0x02); // command 2: Polarity Inversion register
#if 0
        Wire.write(0xff); // Invert all inputs
#endif
        Wire.write(0x00); // Invert no inputs
        Serial.printf("end transmission: %u\n", Wire.endTransmission());

        Wire.beginTransmission(0x27);
        Wire.write(0x03); // command 3: Configuration register
        Wire.write(0x00); // all io as outputs
#if 0
        Wire.write(0xff); // all io as input
        Wire.write(0x7f); // io 7 as output
#endif
        Serial.printf("end transmission: %u\n", Wire.endTransmission());
    }

    Wire.beginTransmission(0x27);
    Wire.write(0x01); // command 1: Output Port register
#if 0
    Wire.write(toggle ? 0xff : 0x00);
#endif
#if 0
    Wire.write(sequence[count%sizeof(sequence)]);
#endif
    Wire.write((count % 0xff) ^ 0xff);
    Wire.endTransmission();

    Wire.beginTransmission(0x27);
    Wire.write(0x00); // command 0: Input Port register∫
    Wire.endTransmission();

    Wire.requestFrom(uint8_t(0x27), size_t(1));
    uint8_t b = Wire.read();
    Serial.print(b, BIN);
    Serial.print("            \r");

    delay(1000);
    toggle = !toggle;
    ++count;
}
