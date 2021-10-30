#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include <Arduino.h>
#include <ESP32Servo.h>
#include <PID_v2.h>

Servo rudder;
Servo motor1;
Servo motor2;
double bearingSetpoint, bearingInput, bearingOutput;
double speedSetpoint, speedInput, speedOutput;
PID bearingPid(&bearingInput, &bearingOutput, &bearingSetpoint, BEARING_P, BEARING_I, BEARING_D, DIRECT);
PID speedPid(&speedInput, &speedOutput, &speedSetpoint, SPEED_P, SPEED_I, SPEED_D, DIRECT);

void setupMovement() {
  Serial.print("Movement: Initialising");
  motor1.attach(6, 1000, 2000);
  motor2.attach(7, 1000, 2000);
  rudder.attach(5);
  speedSetpoint = TARGET_SPEED;
  bearingPid.SetMode(AUTOMATIC);
  speedPid.SetMode(AUTOMATIC);
  Serial.println("Movement: Success");
}

void drive() {
  // If we've been commanded to halt, or the current waypoint is invalid lets stop
  if (command.type == COMMAND_HALT || !waypoints[0].valid) {
    motor1.write(0);
    motor2.write(0);
    rudder.write(90);
  } else if (
    distance(
      frame.latitude.floatingPoint,
      frame.longitude.floatingPoint,
      waypoints[0].latitude.floatingPoint,
      waypoints[0].longitude.floatingPoint
    ) < MIN_DISTANCE
  ) {
    // Shift all the waypoints along one
    for (uint8_t i = 0; i < NUM_WAYPOINTS - 1; i++) {
      waypoints[i] = waypoints[i + 1];
    }
    waypoints[NUM_WAYPOINTS - 1] = { 0.0, 0.0, false };
  } else {
    // Attempt to maintain the target speed
    speedInput = frame.speed.floatingPoint;
    speedPid.Compute();
    motor1.write(map(speedOutput, 0, 255, 0, 180));
    motor2.write(map(speedOutput, 0, 255, 0, 180));

    // Attempt to maintain a bearing to the next waypoint
    bearingInput = frame.bearing.floatingPoint;
    bearingSetpoint = bearing(
      frame.latitude.floatingPoint,
      frame.longitude.floatingPoint,
      waypoints[0].latitude.floatingPoint,
      waypoints[0].longitude.floatingPoint
    );
    bearingPid.Compute();
    rudder.write(map(bearingOutput, 0, 255, 0, 180));
  }
}

#endif
