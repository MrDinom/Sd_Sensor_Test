#ifndef SD_H
#define SD_H

#include "vex.h"

using namespace vex;


// ============================================================
// SETTINGS
// ============================================================

#define AUTON_TIME_MS       15000
#define DRIVER_TIME_MS      105000
#define LOG_INTERVAL_MS     250

#define MAX_AUTON_SAMPLES   61
#define MAX_DRIVER_SAMPLES  421


// ============================================================
// DATA STRUCTURE
// ============================================================

struct MatchSample {
    int timeMs;
    double motor19Deg;
    bool clawOpen;
};


// ============================================================
// DATA STORAGE
// ============================================================

MatchSample autonData[MAX_AUTON_SAMPLES];
MatchSample driverData[MAX_DRIVER_SAMPLES];

int autonSampleCount = 0;
int driverSampleCount = 0;


// ============================================================
// LOGGER REFERENCES
// ============================================================

brain* loggerBrain = nullptr;
motor* loggerMotor19 = nullptr;
bool* loggerClawOpen = nullptr;


// ============================================================
// AUTON LOGGER
// ============================================================

int autonLoggerTask() {

    autonSampleCount = 0;

    int startTime = loggerBrain->timer(msec);

    while (
        loggerBrain->timer(msec) - startTime < AUTON_TIME_MS &&
        autonSampleCount < MAX_AUTON_SAMPLES
    ) {

        autonData[autonSampleCount].timeMs =
            loggerBrain->timer(msec) - startTime;

        autonData[autonSampleCount].motor19Deg =
            loggerMotor19->position(degrees);

        autonData[autonSampleCount].clawOpen =
            *loggerClawOpen;

        autonSampleCount++;

        wait(LOG_INTERVAL_MS, msec);
    }

    return 0;
}


// ============================================================
// DRIVER LOGGER
// ============================================================

int driverLoggerTask() {

    driverSampleCount = 0;

    int startTime = loggerBrain->timer(msec);

    while (
        loggerBrain->timer(msec) - startTime < DRIVER_TIME_MS &&
        driverSampleCount < MAX_DRIVER_SAMPLES
    ) {

        driverData[driverSampleCount].timeMs =
            loggerBrain->timer(msec) - startTime;

        driverData[driverSampleCount].motor19Deg =
            loggerMotor19->position(degrees);

        driverData[driverSampleCount].clawOpen =
            *loggerClawOpen;

        driverSampleCount++;

        wait(LOG_INTERVAL_MS, msec);
    }

    return 0;
}


// ============================================================
// START AUTON LOGGER
// ============================================================

void startAutonLogger(
    brain& brainObject,
    motor& motor19Object,
    bool& clawState
) {

    loggerBrain = &brainObject;
    loggerMotor19 = &motor19Object;
    loggerClawOpen = &clawState;

    task autonTask(autonLoggerTask);
}


// ============================================================
// START DRIVER LOGGER
// ============================================================

void startDriverLogger(
    brain& brainObject,
    motor& motor19Object,
    bool& clawState
) {

    loggerBrain = &brainObject;
    loggerMotor19 = &motor19Object;
    loggerClawOpen = &clawState;

    task driverTask(driverLoggerTask);
}


// ============================================================
// SAVE AUTON DATA
// ============================================================

void saveAutonData(brain& brainObject) {

    if (!brainObject.SDcard.isInserted()) {

        brainObject.Screen.clearScreen();
        brainObject.Screen.print("NO SD CARD");

        return;
    }

    // Delete/overwrite the old file by creating the header
    const char header[] =
        "Time,Motor19Deg,Claw\n";

    brainObject.SDcard.savefile(
        "Auton.csv",
        (uint8_t*)header,
        sizeof(header) - 1
    );


    // Write each sample
    for (int i = 0; i < autonSampleCount; i++) {

        char line[80];

        int length = sprintf(
            line,
            "%.2f,%.2f,%s\n",
            autonData[i].timeMs / 1000.0,
            autonData[i].motor19Deg,
            autonData[i].clawOpen ? "Open" : "Closed"
        );

        brainObject.SDcard.appendfile(
            "Auton.csv",
            (uint8_t*)line,
            length
        );
    }


    brainObject.Screen.clearScreen();
    brainObject.Screen.print("AUTON SAVED");
}


// ============================================================
// SAVE DRIVER DATA
// ============================================================

void saveDriverData(brain& brainObject) {

    if (!brainObject.SDcard.isInserted()) {

        brainObject.Screen.clearScreen();
        brainObject.Screen.print("NO SD CARD");

        return;
    }

    const char header[] =
        "Time,Motor19Deg,Claw\n";

    brainObject.SDcard.savefile(
        "Driver.csv",
        (uint8_t*)header,
        sizeof(header) - 1
    );


    // Write each sample
    for (int i = 0; i < driverSampleCount; i++) {

        char line[80];

        int length = sprintf(
            line,
            "%.2f,%.2f,%s\n",
            driverData[i].timeMs / 1000.0,
            driverData[i].motor19Deg,
            driverData[i].clawOpen ? "Open" : "Closed"
        );

        brainObject.SDcard.appendfile(
            "Driver.csv",
            (uint8_t*)line,
            length
        );
    }


    brainObject.Screen.clearScreen();
    brainObject.Screen.print("DRIVER SAVED");
}

#endif