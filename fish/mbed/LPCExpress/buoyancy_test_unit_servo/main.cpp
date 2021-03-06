#include "BtuRotary.cpp"
#include "mbed.h"

#define NUM_FLOATS 5
#define TIMESTEP 0.05

#include "MODSERIAL.h"
#include "SerialComm.h"

MODSERIAL pcSerial(USBTX, USBRX);
AnalogIn pot1(p15);
DigitalOut TestLED(LED1);
DigitalOut TestLED2(LED2);

// bool clk = true;
BtuRotary btu = BtuRotary();
int counter = 0;
int mode = 2;
float kC = 1.0;
float TauI = 0.0;
float TauD = 0.0;
float setVal = 0.0;             // meters

void runControl() {
  counter = (counter + 1) % 20;
  if(!counter) {
    TestLED = !TestLED;
  }

  setVal = pot1;
  float a1, b1;
  if(mode == 1 || mode == 4) {               //  pos control
    a1 = POS_MIN;
    b1 = POS_MAX;
  } else if(mode == 2) {
      a1 = VEL_MIN;
      b1 = VEL_MAX;

  } else {
    a1 = 0.1;
    b1 = 2.0;
  }
  setVal = (b1-a1)*setVal+a1;


  btu.update(mode, kC, TauI, TauD);
  btu.updateAndRunCycle(mode, setVal);
}

int main() {
  pcSerial.printf("Start!\n");

  SerialComm serialComm(&pcSerial);

  btu.init();
  pcSerial.printf("pressure at start: %.6f\r\n", btu.getPressure());
  Ticker timer;
  timer.attach(&runControl, TIMESTEP);
  TestLED = 0;
  float valueFloats[NUM_FLOATS];

  while(1) {
      float depth = btu.getDepth();
      pcSerial.printf("m:%d, kc:%f, ti:%f, td:%f, s:%.2f, cu:%.2f, de:%.2f, depth_er:%.4f, osc:%d\r\n",
                      btu.getMode(), btu.getKc(), btu.getTauI(), btu.getTauD(), setVal, btu.getServoPos(), depth, setVal - depth);

      if(serialComm.checkIfNewMessage()) {
          serialComm.getFloats(valueFloats, NUM_FLOATS);

          mode = (int) valueFloats[0];
          kC = valueFloats[1];
          TauI = valueFloats[2];
          TauD = valueFloats[3];
          setVal = valueFloats[4];
      }
      wait_ms(1000);
      TestLED2 = !TestLED2;
  }
}
