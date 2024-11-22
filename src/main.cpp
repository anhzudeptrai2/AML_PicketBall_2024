#include <Arduino.h>

#include <AML_ControlMotor.h>
#include <AML_IA6B.h>
#include <AML_IMU.h>
int32_t Base_PWM = 120;
float Kp = 2.5;
// float Ki = 0.005;
// float Kd = 0.05;
float Target_AngleZ = 0;
void Control_Motor_Straight()
{
  int32_t Error = Target_AngleZ - AML_IMU_GetAngle();
  // Integral += Error;
  // float Derivative = Error - Previous_Error;
  uint32_t Fix_PWM = Kp * Error;

  int32_t FL_PWM = constrain(Base_PWM - 1, 0, 150);
  int32_t FR_PWM = constrain(Base_PWM, 0, 150);
  int32_t BL_PWM = constrain(Base_PWM - 1, 0, 150);
  int32_t BR_PWM = constrain(Base_PWM, 0, 150);

  if (Error > 0)
  {
    FL_PWM = constrain(Base_PWM + Fix_PWM, 0, 150);
    FR_PWM = constrain(Base_PWM - Fix_PWM, 0, 150);
    BL_PWM = constrain(Base_PWM + Fix_PWM, 0, 150);
    BR_PWM = constrain(Base_PWM - Fix_PWM, 0, 150);

    AML_ControlMotor_PWM_Left(FL_PWM - 2, BL_PWM - 2);
    AML_ControlMotor_PWM_Right(FR_PWM, BR_PWM);
  }
  else if (Error < 0)
  {
    FL_PWM = constrain(Base_PWM + Fix_PWM, 0, 150);
    FR_PWM = constrain(Base_PWM - Fix_PWM, 0, 150);
    BL_PWM = constrain(Base_PWM + Fix_PWM, 0, 150);
    BR_PWM = constrain(Base_PWM - Fix_PWM, 0, 150);
  }
  else
  {
    AML_ControlMotor_PWM_Left(FL_PWM - 2, BL_PWM - 2);
    AML_ControlMotor_PWM_Right(FR_PWM, BR_PWM);
  }

  Serial.print("Error: ");
  Serial.print(Error);
  Serial.print(" |Current ZAngle: ");
  Serial.print(AML_IMU_GetAngle());
  Serial.print(" | FL PWM: ");
  Serial.print(FL_PWM);
  Serial.print(" | FR PWM: ");
  Serial.print(FR_PWM);
  Serial.print(" | BL PWM: ");
  Serial.print(BL_PWM);
  Serial.print(" | BR PWM: ");
  Serial.println(BR_PWM);
}
void setup()
{
  Serial.begin(115200);
  AML_ControlMotor_BTSsetup();
  AML_ControlMotor_PWM_Left(0, 0);
  AML_ControlMotor_PWM_Right(0, 0);
  AML_IMU_Setup();
}

void loop()
{
  // AML_IA6B_ReadPulseValues();
  // AML_IA6B_PrintPulseValues();
  // AML_IA6B_ControlMotor();
  AML_IMU_SerialEvent();
  AML_IMU_Read();
  // AML_IMU_GetAngle();
  Control_Motor_Straight();
}
