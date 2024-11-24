#include <Arduino.h>

#include <AML_ControlMotor.h>
#include <AML_IA6B.h>
#include <AML_IMU.h>

int32_t Base_PWM = 120;

float Kp = 11;
float Ki = 0.01;
float Kd = 5.5;

int  Target_AngleZ = 0;
float Integral = 0;
float Previous_Error = 0;

void AML_ControlPID_Straight()
{

  static float Smoothed_Angle = 0;
  float Current_AngleZ = AML_IMU_GetAngle();
  Smoothed_Angle = 0.9 * Smoothed_Angle + 0.1 * Current_AngleZ;
  int32_t Error = (Target_AngleZ - Smoothed_Angle);
  
  Integral += Error;
  Integral = constrain(Integral, -100, 100);
  
  float Derivative = Error - Previous_Error;
  float Fix_PWM = Kp * Error + Ki * Integral + Kd * Derivative;
  
  Fix_PWM = constrain(Fix_PWM, -Base_PWM, Base_PWM);

  int32_t FL_PWM, FR_PWM, BL_PWM, BR_PWM;

  if (Error > 0)
  {
    
    float Motor_Offset = 0.0;
    FL_PWM = constrain(Base_PWM + Fix_PWM - Motor_Offset, 0, 255);
    FR_PWM = constrain(Base_PWM - Fix_PWM + Motor_Offset, 0, 255);
    BL_PWM = constrain(Base_PWM + Fix_PWM - Motor_Offset, 0, 255);
    BR_PWM = constrain(Base_PWM - Fix_PWM + Motor_Offset, 0, 255);
  }
  else
  {
    
    float Motor_Offset = 0.0;
    FL_PWM = constrain(Base_PWM - Fix_PWM + Motor_Offset, 0, 255);
    FR_PWM = constrain(Base_PWM + Fix_PWM - Motor_Offset, 0, 255);
    BL_PWM = constrain(Base_PWM - Fix_PWM - Motor_Offset, 0, 255);
    BR_PWM = constrain(Base_PWM + Fix_PWM + Motor_Offset, 0, 255);
  }

  AML_ControlMotor_PWM_Left(FL_PWM, BL_PWM);
  AML_ControlMotor_PWM_Right(FR_PWM, BR_PWM);

  Previous_Error = Error;
}
// Serial.print("Error: ");
// Serial.print(Error);
// Serial.print(" |Current ZAngle: ");
// Serial.print(AML_IMU_GetAngle());
// Serial.print(" | FL PWM: ");
// Serial.print(FL_PWM);
// Serial.print(" | FR PWM: ");
// Serial.print(FR_PWM);
// Serial.print(" | BL PWM: ");
// Serial.print(BL_PWM);
// Serial.print(" | BR PWM: ");
// Serial.println(BR_PWM);

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  AML_ControlMotor_BTSsetup();
  AML_ControlMotor_PWM_Left(0, 0);
  AML_ControlMotor_PWM_Right(0, 0);
  delay(500);
  AML_IMU_ResetAngleZ();
  AML_IMU_Setup();
}

void loop()
{
  // AML_IA6B_ReadPulseValues();
  // AML_IA6B_PrintPulseValues();
  // AML_IA6B_ControlMotor();
  AML_IMU_SerialEvent();
  AML_IMU_Read();
  AML_IMU_GetAngle();
  AML_ControlPID_Straight();
}
