#include <Wire.h>
#include <MPU6050.h>

MPU6050 imu;

struct AML_PID_Struct {
    double Kp;
    double Ki;
    double Kd;
    double tau;
    double sampleTime;

    double integratol;
    double prevError;
    double differentiator;
    double prevMeasurement;
    double out;

    double limMax;
    double limMin;

    double linMaxInt;
    double linMinInt;

    unsigned long lastTime;
};

AML_PID_Struct pid;

void AML_PID_Init(AML_PID_Struct *pid, double kp, double ki, double kd, double tau, double sampleTime) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->tau = tau;
    pid->sampleTime = sampleTime;

    pid->integratol = 0;
    pid->prevError = 0;

    pid->differentiator = 0;
    pid->prevMeasurement = 0;

    pid->out = 0;

    pid->limMax = 255; // PWM max value
    pid->limMin = -255; // PWM min value
    pid->linMaxInt = 200; // Limit integrator
    pid->linMinInt = -200;
    pid->lastTime = millis();
}

double AML_PID_Compute(AML_PID_Struct *pid, double measurement, double setpoint) {
    unsigned long now = millis();
    unsigned long timeChange = now - pid->lastTime;

    if (timeChange >= pid->sampleTime) {
        double error = setpoint - measurement;
        double pTerm = pid->Kp * error;

        pid->integratol += error * pid->sampleTime;
        pid->integratol += 0.5 * pid->Ki * pid->sampleTime * (error + pid->prevError);

        if (pid->integratol > pid->linMaxInt)
            pid->integratol = pid->linMaxInt;
        else if (pid->integratol < pid->linMinInt)
            pid->integratol = pid->linMinInt;

        double iTerm = pid->Ki * pid->integratol;

        pid->differentiator = -(2.0 * pid->Kd * (measurement - pid->prevMeasurement) +
                                (2.0 * pid->tau - pid->sampleTime) * pid->differentiator) /
                              (2.0 * pid->tau + pid->sampleTime);

        double dTerm = pid->Kd * pid->differentiator;

        pid->out = pTerm + iTerm + dTerm;

        if (pid->out > pid->limMax)
            pid->out = pid->limMax;
        else if (pid->out < pid->limMin)
            pid->out = pid->limMin;

        pid->prevMeasurement = measurement;
        pid->prevError = error;
        pid->lastTime = now;
    }
    return pid->out;
}

void setup() {
    Serial.begin(115200);
    imu.initialize();

    AML_PID_Init(&pid, 2.0, 1.0, 0.5, 0.02, 100); // Adjust PID constants as needed
}

void loop() {
    double currentAngle = imu.getAngleZ(); // Replace with the correct function for your IMU library
    double targetAngle = 90.0; // Target angle

    double controlSignal = AML_PID_Compute(&pid, currentAngle, targetAngle);

    // Map control signal to motor speed
    int motorSpeed = (int)controlSignal;

    if (motorSpeed > 0) {
        // Rotate right
        analogWrite(LEFT_MOTOR_PIN, motorSpeed);
        analogWrite(RIGHT_MOTOR_PIN, 0);
    } else {
        // Rotate left
        analogWrite(LEFT_MOTOR_PIN, 0);
        analogWrite(RIGHT_MOTOR_PIN, -motorSpeed);
    }

    // Stop motors if close to the target angle
    if (abs(targetAngle - currentAngle) < 1.0) {
        analogWrite(LEFT_MOTOR_PIN, 0);
        analogWrite(RIGHT_MOTOR_PIN, 0);
    }

    delay(10); // Adjust as necessary
}


