#include <TCA9548.h>
#include <Wire.h>
#include <FastIMU.h>
#include <WiFi.h>
#include <esp_now.h>

#define NUM_IMUS 3 // Number of MPU-6500 modules
#define MUX_ADDRESS 0x70 // Address of the PCA9548A multiplexer
#define CHANNEL 1 //the same channel should be on both esps
#define RAD_TO_DEG (180.0 / PI) // radians to degrees conversion factor


uint8_t broadcastAddress[] = {0x10, 0x06, 0x1C, 0xF6, 0x83, 0x78};//Mac Address
String data;
esp_now_peer_info_t peerInfo;// Peer info

TCA9548 mux = TCA9548(MUX_ADDRESS); // PCA9548A multiplexer object
MPU6500 IMU[NUM_IMUS]; // Array of MPU-6500 objects
calData calib[NUM_IMUS]; // Array of calibration data
AccelData accelData[NUM_IMUS]; // Array of accelerometer data
GyroData gyroData[NUM_IMUS]; // Array of gyroscope data


// --- FILTER VARIABLES ---
double pitch[3]= {0, 0, 0}, roll[3]={0, 0, 0};  // Orientation angles (degrees)
double dt = 0.02;                     // Loop time in seconds (50 Hz)

// --- KALMAN FILTER PARAMETERS ---
double Q_angle = 0.001;  // Process noise variance for the accelerometer
double Q_bias = 0.003;   // Process noise variance for the gyroscope bias
double R_measure = 0.03; // Measurement noise variance
double angle[3] = {0, 0, 0}, bias[3] = {0, 0, 0}, rate[3] = {0, 0, 0}; // Kalman filter state variables
double P[3][2][2] = {{{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}};    // Error covariance matrix

unsigned long lastTime; // For calculating loop time

String startByte = "#";
String destROLL[3] = {"A","D", "G"}; // Destination bytes for roll
String destPITCH[3] = {"B","E","H"}; // Destination bytes for pitch
String destYAW[3] = {"C","F","I"}; // Destination bytes for yaw

void setup() {
  Serial.begin(115200); // Set serial baud rate
  Serial.println("Setup started");
  Wire.begin();
  Wire.setClock(400000); // 400 kHz clock

  //ESP-NOW Setup
  WiFi.mode(WIFI_STA);
  // Initialize ESP-NOW
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESP-NOW initialized successfully");
  } else {
    Serial.println("Error initializing ESP-NOW");
  }
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  //Register peer 
  memcpy(peerInfo.peer_addr , broadcastAddress,6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //Add peer 
  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Fail to add peer!");
  }

mux.begin(); // Initialize the PCA9548A multiplexer

  // Initialize the MPU
  for (int i = 0; i < NUM_IMUS; i++) {
    mux.selectChannel(i); // Select the I2C channel for the current MPU-6500 module
    Serial.print("Initializing IMU ");
    Serial.println(i);
    //IMU[i].setGyroRange(500);
    IMU[i].init(calib[i], 0x68); // Initialize the MPU-6500 module
    //init_suit();
    if (IMU[i].init(calib[i], 0x68) != 0) {
      Serial.print("Error initializing IMU ");
      Serial.print(i);
      Serial.println(": ");
      Serial.println(IMU[i].init(calib[i], 0x68));
      while (true) {
        ;
        }
     }
  }

/*
Serial.println("FastIMU calibration ");
for (int i = 0; i < NUM_IMUS; i++){
  delay(500);
  Serial.println("Keep IMU level.");
  delay(500);
  IMU[i].calibrateAccelGyro(&calib[i]);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib[i].accelBias[0]);
  Serial.print(", ");
  Serial.print(calib[i].accelBias[1]);
  Serial.print(", ");
  Serial.println(calib[i].accelBias[2]);
  Serial.println("Gyro biases X/Y/Z: ");
  Serial.print(calib[i].gyroBias[0]);
  Serial.print(", ");
  Serial.print(calib[i].gyroBias[1]);
  Serial.print(", ");
  Serial.println(calib[i].gyroBias[2]);
  for (int j =0 ; j <3 ; j++)

    if (calib[i].gyroBias[j] > 0)
      calib[i].gyroBias[j]=calib[i].gyroBias[j]*-1;

  delay(5000);
  IMU[i].init(calib[i], 0x68);
}*/

  lastTime = millis();
}

double R[3][3];

double gx[NUM_IMUS],gy[NUM_IMUS];
double roll_acc[NUM_IMUS] = {0,0,0},pitch_acc[NUM_IMUS] = {0,0,0};
double alpha=0.4; // FOR COMPLEMENTARY FILTER
double hpf_alpha=0.8;
double lpf_alpha=0.95;
double prev_FilteredValueroll[NUM_IMUS] = {0,0,0};//for high-pass
double prev_FilteredValuepich[NUM_IMUS] = {0,0,0};//for High-pass
double filtered_valueroll[NUM_IMUS] = {0,0,0}; //for low-pass filter
double filtered_valuepich[NUM_IMUS] = {0,0,0}; //for low-pass filter

void loop() {
  for (int i = 0; i < NUM_IMUS; i++) { 

    mux.selectChannel(i); // Select the current IMU channel
    IMU[i].update();
    IMU[i].getAccel(&accelData[i]);
    IMU[i].getGyro(&gyroData[i]);

    // Convert raw gyroscope data to angular velocities (degrees/s)
    if(i==0)
    {
     gx[0]= (gyroData[0].gyroX); // Convert to degrees/s +1.20547
     gy[0] = (gyroData[0].gyroY); // Convert to degrees/s -2.6067
     
    }
    else if(i==1)
    {
     gx[1] = (gyroData[1].gyroX)/131; // Convert to degrees/s +1.20547
     gy[1] = (gyroData[1].gyroY)/131; // Convert to degrees/s -2.6067
     
    } else if (i==2)
    {
     gx[2] = (gyroData[2].gyroX); // Convert to degrees/s +1.20547
     gy[2] = (gyroData[2].gyroY); // Convert to degrees/s -2.6067
     
    }
      /*// --- FILTERING THE DATA ---
      gx[i] = lpf_alpha * gx[i] + (1 - lpf_alpha) * filtered_valueroll[i] ;//low-pass filter
      gy[i] = lpf_alpha * gy[i] + (1 - lpf_alpha) * filtered_valuepich[i] ;//low-pass filter

      gx[i] = hpf_alpha * (prev_FilteredValueroll[i] + gx[i] - prev_FilteredValueroll[i]); // High-pass filter 
      prev_FilteredValueroll[i] = gx[i]; //High-pass filter
      
      gy[i] = hpf_alpha * (prev_FilteredValuepich[i] + gy[i] - prev_FilteredValuepich[i]); // High-pass filter 
      prev_FilteredValuepich[i] = gy[i]; //High-pass filter  */
     
    // --- CALCULATE TIME STEP ---
    unsigned long currentTime = millis();
    dt = (currentTime - lastTime) / 1000.0; // Calculate time in seconds
    if (dt == 0) dt = 0.001;               // Prevent division by zero
    lastTime = currentTime;

    // Calculate Euler angles from the accelerometer data
    roll_acc[i] = atan2(accelData[i].accelY, accelData[i].accelZ)* 180.0 / PI;
    pitch_acc[i] = atan(-accelData[i].accelX / sqrt(accelData[i].accelY * accelData[i].accelY + accelData[i].accelZ * accelData[i].accelZ))* 180.0 / PI; 
    

    
   /* // --- KALMAN FILTER FOR PITCH AND ROLL ---
    pitch[i] = Kalman_filter(i,&pitch[i], &gx[i], &pitch_acc[i]);
    roll[i] = Kalman_filter(i,&roll[i], &gy[i], &roll_acc[i]);*/
    
   /*
    roll[i] += gy[i] * 0.05; // Integrate X-axis angular velocity
    pitch[i] += gx[i] * 0.05; // Integrate Y-axis angular velocity*/

    // Complementary filter
    roll[i] = alpha * (roll[i] + gy[i] * dt) + (1 - alpha) * roll_acc[i];
    pitch[i] = alpha * (pitch[i] + gx[i] * dt) + (1 - alpha) * pitch_acc[i];
    
    double roll0=roll[0];
    double pitch0=pitch[0];
    double yaw=0;
    rpyToRotationMatrix(roll0,pitch0,yaw,R);
   
    // Compute theta3 in radians
    double theta3 = acos(-R[1][2]);
    double st3 = sin(theta3);
    double ct3 = cos(theta3);

    // Compute theta2 in radians
    if (st3==0)
    st3=1;
    double theta2 = acos(-R[2][2] / st3);

    // Compute theta4 in radians
    double theta4 = asin(-R[1][1] / st3);

    // Convert to degrees and adjust
    double t2 = degrees(theta2) - 180;
    double t3 = degrees(theta3) - 90;
    double t4 = degrees(theta4) + 90;

   /* Serial.print(roll[0]);
    Serial.print(" , ");
    Serial.print(pitch[0]);

    Serial.print(" , ");

    Serial.print(roll[1]);
    Serial.print(" , ");
    Serial.print(pitch[1]);

    Serial.print(" , ");

    Serial.print(roll[2]);
    Serial.print(" , ");
    Serial.println(pitch[2]);*/

    //Sending Data
   /*if (i==1)
      roll[1]=roll[1]-roll[0];*/


    Serial.println(Packet(startByte, roll[i] , destROLL[i]));
    //esp_err_t result = esp_now_send(broadcastAddress , (uint8_t*) &data, sizeof(data));
    delay(25);

     /*if (i==1)
      pitch[1]=pitch[1]-pitch[0];*/


    Serial.println(Packet(startByte, pitch[i], destPITCH[i]));
   // result = esp_now_send(broadcastAddress , (uint8_t*) &data, sizeof(data));
    
    delay(50);
    
  }

}

String Packet(String startBy, float VAL, String dest) {
  String packet = "";
  packet += String(startBy);
  packet += String(dest);
  packet += String(VAL, 2);
  data=packet;
  return data;
}

void OnDataSent(const uint8_t *mac_addr , esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status: \t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
double S[NUM_IMUS];
double y[NUM_IMUS];
double Kalman_filter(int imu_index,double angle[], double gyroRate[], double accelAngle[]) {
  // Predict
  rate[imu_index] = gyroRate[imu_index] - bias[imu_index];
  angle[imu_index] += dt * rate[imu_index];
 
  P[imu_index][0][0] += dt * (dt * P[imu_index][1][1] - P[imu_index][0][1] - P[imu_index][1][0] + Q_angle);
  P[imu_index][0][1] -= dt * P[imu_index][1][1];
  P[imu_index][1][0] -= dt * P[imu_index][1][1];
  P[imu_index][1][1] += Q_bias * dt;
 
  // Update
  S[imu_index] = P[imu_index][0][0] + R_measure; // Estimate error
  double K[NUM_IMUS][2];                    // Kalman gain
  K[imu_index][0] = P[imu_index][0][0] / S[imu_index];
  K[imu_index][1] = P[imu_index][1][0] / S[imu_index];
 
   y[imu_index] = accelAngle[imu_index] - angle[imu_index]; // Angle difference
  angle[imu_index] += K[imu_index][0] * y[imu_index];
  bias[imu_index] += K[imu_index][1] * y[imu_index];
 
  double P00_temp = P[imu_index][0][0];
  double P01_temp = P[imu_index][0][1];
 
  P[imu_index][0][0] -= K[imu_index][0] * P00_temp;
  P[imu_index][0][1] -= K[imu_index][0] * P01_temp;
  P[imu_index][1][0] -= K[imu_index][1] * P00_temp;
  P[imu_index][1][1] -= K[imu_index][1] * P01_temp;
 
  return angle[imu_index];
}   

void rpyToRotationMatrix(double roll, double pitch, double yaw, double R[3][3]) {
    // Convert degrees to radians
    roll = roll * (M_PI / 180.0);
    pitch = pitch * (M_PI / 180.0);
    yaw = yaw * (M_PI / 180.0);

    // Calculate rotation matrix elements
    double R_x[3][3] = {
        {1, 0, 0},
        {0, cos(roll), -sin(roll)},
        {0, sin(roll), cos(roll)}
    };
    
    double R_y[3][3] = {
        {cos(pitch), 0, sin(pitch)},
        {0, 1, 0},
        {-sin(pitch), 0, cos(pitch)}
    };

    double R_z[3][3] = {
        {cos(yaw), -sin(yaw), 0},
        {sin(yaw), cos(yaw), 0},
        {0, 0, 1}
    };

    // Initialize the final rotation matrix
    double temp[3][3] = {0};

    // R = R_z * R_y * R_x
    // First R_z * R_y
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[i][j] = R_z[i][0] * R_y[0][j] + R_z[i][1] * R_y[1][j] + R_z[i][2] * R_y[2][j];
        }
    }
}
