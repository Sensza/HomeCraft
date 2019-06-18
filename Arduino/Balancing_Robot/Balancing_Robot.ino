/* ARDUINO NANO CLONE
 * Sketch uses 15528 bytes (50%) of program storage space. Maximum is 30720 bytes.
 * Global variables use 595 bytes (29%) of dynamic memory, leaving 1453 bytes for local variables. Maximum is 2048 bytes.
 * 
 * 
 * 
 */

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


// ================================================================
// ===                        BLUETOOTH                         ===
// ================================================================
#define Rx 0
#define Tx 1
#define X_READING_ZERO 1
#define Y_READING_ZERO 2
#define XY_READINGS_ZERO 3
#define XY_READINGS_NO_ZEROS 4


int receivedByte = 0, readingsRecv = 0, hdrFlag = 0, xLen = 0, yLen = 0;
int rawReadingX[2], rawReadingY[2], xControl = 0, yControl = 0;
boolean largeByte = false, packetStarted = false, firstReadingRecv = false, fullPacketReceived = false, xLenRecv = false, yLenRecv = false;

int convertFrom2SToInt(int* inputData) {
   
    if(inputData[1] != 0) {
        switch(inputData[0]) {
          case 255: 
                return -(256-inputData[1]);
                break;
          case 254: 
                return -(512-inputData[1]);
                break;
          case 253: 
                return -(768-inputData[1]);
                break;
          case 0: 
                return ( (inputData[0]<<8)|inputData[1] );
                break;
          case 1: 
                return ( (inputData[0]<<8)|inputData[1] );
                break;
          case 2: 
                return ( (inputData[0]<<8)|inputData[1] );
                break;
          default:
                return 0;
        }
           
    }
    else {
        if(inputData[0] < 128)
            return inputData[0];
        else
            return -(256-inputData[0]);
    }
}


// ================================================================
// ===                        IMU                               ===
// ================================================================

MPU6050 mpu;
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

//INTERRUPT DETECTION ROUTINE
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


// ================================================================
// ===                       PWM AND PID                        ===
// ================================================================
int motorPinPWM_1 = 9, motorPinPWM_2 = 10, leftMotorPower = 0, rightMotorPower = 0;
int leftMotorA1 = 14, leftMotorA2 = 15, rightMotorA3 = 16, rightMotorA4 = 17;
const int Kp = 4, Kd = 1, Ki = 0.02, sampleTime = 0.005;
int error = 0, errorSum = 0, errorPrev = 0, desiredValue = 0, regulatorOutput = 0;

void calculatePID(){

    error = desiredValue - ypr[1];   // Lub ypr[2]
    if(error > -0.25 && error < 0.25)
        error = 0;
    errorSum += error;
    if(errorSum > 200) //Integral windup
        errorSum = 200;
    if(errorSum < -200) //Integral windup
        errorSum = -200;
    regulatorOutput = Kp*error + Ki*errorSum*sampleTime + Kd*(error - errorPrev)/sampleTime;
    errorPrev = error;
}

void calculatePWM(){

    if(ypr[1] >= 0){
        leftMotorPower = regulatorOutput + xControl/10;
        rightMotorPower = regulatorOutput - xControl/10;
    }
    else{
        leftMotorPower = regulatorOutput - xControl/10;
        rightMotorPower = regulatorOutput + xControl/10;
    }
    if(yControl >= 0 && rightMotorPower < 0)
        rightMotorPower = 0;
    if(yControl >= 0 && leftMotorPower < 0)
        leftMotorPower = 0;

    if(yControl < 0 && rightMotorPower > 0)
        rightMotorPower = 0;
    if(yControl < 0 && leftMotorPower > 0)
        leftMotorPower = 0;
        
    if(leftMotorPower > 255)
      leftMotorPower = 255;
    else if(leftMotorPower < -255)
      leftMotorPower = -255;
      
    if(rightMotorPower > 255)
      rightMotorPower = 255;
    else if(rightMotorPower < -255)
      rightMotorPower = -255;

    rightMotorPower *= 0.8;
}

void setMotors(){

    if(leftMotorPower >= 0){
        digitalWrite(leftMotorA1, HIGH);
        digitalWrite(leftMotorA2, LOW);
    }
    else{
        digitalWrite(leftMotorA1, LOW);
        digitalWrite(leftMotorA2, HIGH);
        leftMotorPower *= -1;
    }

    if(rightMotorPower >= 0){
        digitalWrite(rightMotorA3, LOW);
        digitalWrite(rightMotorA4, HIGH);
    }
    else{
        digitalWrite(rightMotorA3, HIGH);
        digitalWrite(rightMotorA4, LOW);
        rightMotorPower *= -1;
    }

    analogWrite(motorPinPWM_1, leftMotorPower);
    analogWrite(motorPinPWM_2, rightMotorPower);
}



// ================================================================
// ===                         SETUP                            ===
// ================================================================
void setup() {

    Serial.begin(9600);
    while (!Serial);
    Serial.println("Bluetooth ready");
            
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    Serial.println(F("Initializing I2C devices... "));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);
    
    // verify connection
    Serial.println(F("Testing device connections... "));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    delay(50);
    // load and configure the DMP
    Serial.println(F("Initializing DMP... "));
    devStatus = mpu.dmpInitialize();
    delay(50);
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP... "));
        mpu.setDMPEnabled(true);
    
        // enable Arduino interrupt detection
        Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
        Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        Serial.println(F(")..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
    
        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt... "));
        dmpReady = true;
    
        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
    
    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
}

void loop() {

// ================================================================
// ===                   MPU6050 DMP ROUTINE                    ===
// ================================================================
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        if (mpuInterrupt && fifoCount < packetSize) {
          // try to get out of the infinite loop 
          fifoCount = mpu.getFIFOCount();
        }  
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        fifoCount = mpu.getFIFOCount();

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        ypr[0] = ypr[0] * 180/M_PI;
        ypr[1] = ypr[1] * 180/M_PI;
        ypr[2] = ypr[2] * 180/M_PI;

        /*Serial.print("ypr\t");
        Serial.print(ypr[0]);
        Serial.print("\t");
        Serial.print(ypr[1]);
        Serial.print("\t");
        Serial.println(ypr[2]);*/
    }



// ================================================================
// ===                BLUETOOTH RECEIVER ROUTINE                ===
// ================================================================
    
    // Check if received any data by bluetooth
    if (Serial.available() > 0) {
        
        // read the incoming byte:
        receivedByte = Serial.read();
        
        if(!packetStarted)
        {
            switch(receivedByte) {
              case 60:
                  hdrFlag = XY_READINGS_NO_ZEROS;
                  packetStarted = true;
                  break;
              case 61:
                  hdrFlag = Y_READING_ZERO;
                  packetStarted = true;
                  break;                
              case 62:
                  hdrFlag = X_READING_ZERO;
                  packetStarted = true;
                  break;
              case 63:
                  hdrFlag = XY_READINGS_ZERO;
                  packetStarted = true;
                  break;
            }
        }
        else {
            if(!firstReadingRecv) {  //X_reading
            
                if(!xLenRecv){
                    if(receivedByte != 1)
                        largeByte = true;
                    else
                        largeByte = false;
                    xLenRecv = true;
                }
                else {
                    if(hdrFlag == X_READING_ZERO || hdrFlag == XY_READINGS_ZERO) {
                        /*if(receivedByte != 0)
                            Serial.println("Error packet - no 'zero' on X position received.");*/
                        rawReadingX[0] = 0;
                        rawReadingX[1] = 0;
                        firstReadingRecv = true;
                    }
                    else{
                        if(!largeByte){
                                rawReadingX[0] = receivedByte;
                                rawReadingX[1] = 0;
                                firstReadingRecv = true;
                        }
                        else {
                            rawReadingX[xLen++] = receivedByte;
                            if(xLen == 2)
                                firstReadingRecv = true;
                        }
                    }
                }
            }
            else { //Y_Reading
             
                if(!yLenRecv){
                    if(receivedByte != 1)
                        largeByte = true;
                    else
                        largeByte = false;
                    yLenRecv = true;
                }
                else {
                    if(hdrFlag == Y_READING_ZERO || hdrFlag == XY_READINGS_ZERO) {
                        /*if(receivedByte != 0)
                            Serial.println("Error packet - no 'zero' on Y position received.");*/
                        rawReadingY[0] = 0;
                        rawReadingY[1] = 0;
                        fullPacketReceived = true;
                    }
                    else{
                        if(!largeByte){
                                rawReadingY[0] = receivedByte;
                                rawReadingY[1] = 0;
                                fullPacketReceived = true;
                        }
                        else {
                            rawReadingY[yLen++] = receivedByte;
                            if(yLen == 2)
                                fullPacketReceived = true;
                        }
                    }
                }
             }
         }
    }//Packet received
    
    if( fullPacketReceived ){

         xControl = convertFrom2SToInt(rawReadingX);
         yControl = convertFrom2SToInt(rawReadingY);
        // say what you got:
        /*Serial.print("X: ");
        Serial.print(xControl , DEC);
        Serial.print("   Y: ");
        Serial.println(yControl , DEC);*/
        
        hdrFlag = 0;
        xLen = 0;
        yLen = 0;
        firstReadingRecv = false;
        xLenRecv = false;
        yLenRecv = false;
        packetStarted = false;
        fullPacketReceived = false;
    }

// ================================================================
// ===                   CONTROLLER ROUTINE                     ===
// ================================================================

    //yControl expected in range [-300 : 300] so it's safe to assume the reading
    //as -30.0 : 30.0 angle of inclination. Some constraints planned in future.
    desiredValue = yControl/10;
    calculatePID();
    calculatePWM();
    setMotors();
}
