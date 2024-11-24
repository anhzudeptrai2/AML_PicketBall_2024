#include <Arduino.h>

uint32_t Re_buf[33], position = 0;
uint32_t sign = 0;
float g[3] = {0}, Current_AngleZ = 0;


void AML_IMU_Read();

void AML_IMU_ResetAngleZ()
{
    Serial1.write(0xFF);
    Serial1.write(0xAA);
    Serial1.write(0x52);
}

void AML_IMU_Setup()
{
    Serial1.begin(115200);
    Serial.begin(115200);
    AML_IMU_ResetAngleZ();
}
void AML_IMU_SerialEvent()
{
    while (Serial1.available())
    {
        uint8_t byte_in = Serial1.read();
        if (position == 0 && byte_in != 0x55)
        {
            position = 0; //
            continue;
        }
        Re_buf[position] = byte_in;
        position++;
        if (position == 33)
        {
            position = 0;
            sign = 1;
            AML_IMU_Read();
        }
    }
}

void AML_IMU_Read()
{
    if (sign)
    {
        // Serial.println(Re_buf[23]);
        sign = 0;

        if (Re_buf[22] == 0x55 && Re_buf[23] == 0x53)
        {
            g[0] = (short(Re_buf[25] << 8 | Re_buf[24])) / 32768.0 * 180;
            g[1] = (short(Re_buf[27] << 8 | Re_buf[26])) / 32768.0 * 180;
            g[2] = (short(Re_buf[29] << 8 | Re_buf[28])) / 32768.0 * 180;
            Current_AngleZ = g[2];
        }
    }
}
float AML_IMU_GetAngle()
{
    return Current_AngleZ;
}
