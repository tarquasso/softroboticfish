#ifndef BTUROTARY_H
#define BTUROTARY_H

#include "PidController.h"
#include "mbed.h"
#include "QEI.h"
#include "MS5837.h" // pressure sensor
#include "Servo.h"
#include "utility.h"


#define P_ATMOS_MBAR 1000
#define P_WATER_SURFACE_MBAR 98.02

#define POSITION_CTRL_MODE 1
#define VELOCITY_CTRL_MODE 2
#define DEPTH_CTRL_MODE 3
#define SPEC_POSITION_CTRL_MODE 4

#define DEPTH_MIN 0
#define DEPTH_MAX 5

#define VEL_MIN -280.0          /* deg/sec */
#define VEL_MAX 280.0

#define POS_MIN -91.0
#define POS_MAX 91.0

#define PERIOD_PWM 0.00345

#define DEP_K_C 1.0
#define DEP_TAU_I 0.0
#define DEP_TAU_D 0.0
#define PID_FREQ 0.05

#define SP_K_C 23.7
#define SP_TAU_I 0.175
#define SP_TAU_D 0.05

#define SERVO_PWM_WIDTH 0.0006
#define SERVO_DEGREE_WIDTH 91.0

#define PIN_IMU_SDA p28
#define PIN_IMU_SCL p27
#define PIN_PWM_SERVO p23

#define TIME_STEP_MIN 0.01


/**
 * This class is used for controlling and accessing data from the Buoyancy Test Unit
 * It includes instances of the classes PwmOut
 */
class BtuRotary {
private:
  PidController m_depthPid;
  PidController m_specPid;
  MS5837 m_pressureSensor;
  Servo m_motorServo;
  int m_mode;
  float m_kc, m_tauI, m_tauD;

  void positionControl(float setPosDeg);
  void velocityControl(float setVel);
  void depthControl(float setDepthMeters);
  void specialPosControl(float setPosDeg);

public:
  BtuRotary();
  ~BtuRotary();
  void init();
  void stop();
  void update(int mode, float kc, float tauI, float tauD);
  void updateMode(int mode);
  void runCycle(float setVal);
  void updateAndRunCycle(int mode, float value);
  float getPressure();
  float getDepth();
  int getMode() { return m_mode; };
  float getKc() { return m_kc; };
  float getTauI() { return m_tauI; };
  float getTauD() { return m_tauD; };
  float getServoPos() { return m_motorServo.readPosition(); };


};

#endif
