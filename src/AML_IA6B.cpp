#include <AML_IA6B.h>
#include <AML_ControlMotor.h>

#define CH2 10 // Kênh điều khiển 1
#define CH4 11 // Kênh điều khiển 2

int32_t volume2; // Xung đầu vào từ kênh 1
int32_t volume4; // Xung đầu vào từ kênh 2

bool debug_mode = 1;

void AML_IA6B_ReadPulseValues()
{
    // Đo độ rộng xung từ kênh quay tròn
    volume2 = pulseIn(CH2, HIGH); // Đo độ rộng xung từ kênh 1
    volume4 = pulseIn(CH4, HIGH); // Đo độ rộng xung từ kênh 2
}

void AML_IA6B_PrintPulseValues()
{
    Serial.print(" | volume2 (kênh 1): ");
    Serial.print(volume2);
    Serial.print(" | volume4 (kênh 2): ");
    Serial.println(volume4);
}
void AML_IA6B_ControlMotor()
{
    // Đọc và in giá trị xung từ các kênh
    AML_IA6B_ReadPulseValues();
    AML_IA6B_PrintPulseValues();

    // Điều khiển xe dựa vào giá trị xung
    if (volume2 < 1975 && volume2 > 1967 && volume4 < 1977 && volume4 > 1968)
    {
        AML_ControlMotor_PWM_Left(50, 50); // tien trai
        AML_ControlMotor_PWM_Right(120, 120);
        if (debug_mode)
         Serial.println("Re Phai");
    }
    else if (volume2 < 1122 && volume2 > 1104 && volume4 < 1987 && volume4 > 1968)
    {
        AML_ControlMotor_PWM_Left(120, 120);
        AML_ControlMotor_PWM_Right(50, 50);
        Serial.println("Re Trai");
    }
    else if (volume2 > 1090 && volume2 < 1110 && volume4 < 1108 && volume4 > 1090)
    {
        AML_ControlMotor_PWM_Left(-120, -120); // Lui  trai
        AML_ControlMotor_PWM_Right(-50, -50);
        Serial.println("Lui Trai");
    }
    else if (volume2 < 1975 && volume2 > 1968 && volume4 < 1108 && volume4 > 1103)
    {
        AML_ControlMotor_PWM_Left(-50, -50); // Lui  trai
        AML_ControlMotor_PWM_Right(-120, -120);
        Serial.println("Lui Phai");
    }
    else if (volume4 < 2000 && volume4 > 1900)
    {
        AML_ControlMotor_PWM_Left(80, 80); // Cả hai tín hiệu đều lớn -> Di chuyển tiến
        AML_ControlMotor_PWM_Right(80, 80);
        Serial.println("Tien");
    }
    else if (volume2 < 1600 && volume2 > 1500 && volume4 < 1108 && volume4 > 1103)
    {
        AML_ControlMotor_PWM_Left(-80, -80); // Cả hai tín hiệu đều nhỏ -> Lùi
        AML_ControlMotor_PWM_Right(-80, -80);
        Serial.println("Lui");
    }
    else if (volume4 < 1699 && volume4 > 1500)
    {
        AML_ControlMotor_PWM_Left(0, 0); // DUNG
        AML_ControlMotor_PWM_Right(0, 0);
    }
}
