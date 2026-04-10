#pragma once
#include <iomanip>
#include <algorithm>
#include <cmath> 

class Speed { 
    public: 
    Speed( double idleRpm = 800.0, double maxRpm = 7000.0, double accelRate = 1500.0 ) : idleRpm_(idleRpm), maxRpm_(maxRpm), accelRate_(accelRate), currentRpm_(idleRpm), totalTime_(0.0) {} 

double update(double throttle, double dt, double accelRate) { 
   throttle = std::clamp(throttle, 0.0, 1.0); 
   dt = std::max(dt, 0.0); 
   accelRate_ = accelRate;
 
   const double targetRpm = idleRpm_ + throttle * (maxRpm_ - idleRpm_); 
   double shiftRpm  = 2000 + throttle * (maxRpm_ - 2000); // only allow shifts above 2000 rpm
 
   if (currentRpm_ < targetRpm) {
      currentRpm_ += accelRate_ * throttle * dt; 
   }
   else if (currentRpm_ > targetRpm) {
      currentRpm_ -= accelRate_ * dt; 
   }
   else {
      currentRpm_ = targetRpm;
   }

   if (currentRpm_ >= shiftRpm && gear < 7) { // signal when to shift
         gear++; // simulate a gear shift
         currentGear = gearRatios[gear];
         currentRpm_ = currentRpm_ * (gearRatios[gear] / gearRatios[gear - 1]);
   } 
      
   currentRpm_ = std::clamp(currentRpm_, idleRpm_, maxRpm_); 
   
   if(totalTime_ > 0) {
      lastTorque_ = computeTorque(throttle, currentRpm_);  
      lastSpeed_ = computeSpeed(currentRpm_);
   }
 
   totalTime_ += dt; 
   return currentRpm_; 
} 
 
double getTorque() const { return lastTorque_; } 
double getSpeed() const { return lastSpeed_; }
double getGear() const { return gear + 1; } // gear index starts at 0
double getAccel() {return accel;}

double computeAccel(double drag, double resistance, double mass) {
   double tractiveForce = (lastTorque_ * currentGear * finalDrive * drivetrainEfficiency) / tireRadius;
   double netAcceleration = (tractiveForce - drag - resistance) / mass;
   accel = (netAcceleration < 0) ? 0 : netAcceleration;
   double accelRate = (accel / tireRadius) * (60 / (2 * pi)) * currentGear * finalDrive;
   return accelRate;
}

private: double idleRpm_; double maxRpm_; double accelRate_; double currentRpm_; 

double totalTime_ = 0.0; 
double lastTorque_ = 0.0; 
double lastSpeed_ = 0.0; 
double accel = 0.0;

int gear = 0;
double currentGear = 4.714; // start in first

const double gearRatios[8] {4.714, 3.143, 2.106, 1.667, 1.285, 1.000, 0.839, 0.667}; // gear ratios for 8-speed transmission
const double finalDrive = 3.204; 
const double pi = 3.141592;
const double tireRadius = 0.365; // radius of 285/30 R22 tire (standard for rs6)
const double tireCircumference = 2 * pi * tireRadius; 
const double drivetrainEfficiency = 0.85; //typical drivetrain efficieny for rs6

double computeTorque(double throttle, double rpm) const { 
   double shape;
   double maxTorque = 850.0; // max torque of audi engine

   if (rpm < 2300) {
      double t = (rpm - 800) / (2300 - 800);
      shape = 0.29 + (0.71 * t);
   }
   else if (rpm <= 4500 && rpm >= 2300) { // max torque btwn 2300 to 4500
      shape = 1.0;
   }
   else {
      double t = (rpm - 4500) / (7000 - 4500); // linear interpolation to find how close to redline
      shape = 1 - (0.2 * t); // loses torque as closer to max rpm
   }
   
   if (rpm > 7000) { // rev limiter
      return 0.0;
   }
   // Make throttle slightly nonlinear (realistic airflow behavior) 
   double throttleEffect = std::pow(throttle, 1.2); // max torque only when throttle is 1.0
 
   return maxTorque * shape * throttleEffect; 
} 
 
double computeSpeed(double rpm) {
   double wheelRPM = rpm / (currentGear * finalDrive);
   double speed = (wheelRPM * tireCircumference * 60.0) / 1000.0; // derivation shown in appendix
   return speed;
}


}; 