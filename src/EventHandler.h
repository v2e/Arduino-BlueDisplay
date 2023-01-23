/*
 * EventHandler.h
 *
 *
 *  Copyright (C) 2014  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of BlueDisplay https://github.com/ArminJo/android-blue-display.
 *
 *  BlueDisplay is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#ifndef _EVENTHANDLER_H
#define _EVENTHANDLER_H

#include "Colors.h"

#if !defined(DO_NOT_NEED_BASIC_TOUCH_EVENTS)
//#define DO_NOT_NEED_BASIC_TOUCH_EVENTS // Disables basic touch events like down, move and up. Saves 620 bytes program memory and 36 bytes RAM
#endif

#if !defined(DISABLE_REMOTE_DISPLAY)
#include "BlueDisplayProtocol.h"
#endif

extern bool sBDEventJustReceived; // is set to true by handleEvent() and can be reset by main loop.
extern unsigned long sMillisOfLastReceivedBDEvent; // is updated with millis() at each received event. Can be used for timeout detection.

#define TOUCH_STANDARD_CALLBACK_PERIOD_MILLIS 20 // Period between callbacks while touched (a swipe is app 100 ms)
#define TOUCH_STANDARD_LONG_TOUCH_TIMEOUT_MILLIS 800 // Millis after which a touch is classified as a long touch
//
#define TOUCH_SWIPE_THRESHOLD 10  // threshold for swipe detection to suppress long touch handler calling
#define TOUCH_SWIPE_RESOLUTION_MILLIS 20

#if defined(SUPPORT_LOCAL_DISPLAY)
#   if defined(AUTOREPEAT_BY_USING_LOCAL_EVENT)
extern struct BluetoothEvent localTouchEvent;
#  endif
/*
 * helper variables
 */
extern bool sNothingTouched;
extern bool sSliderIsMoveTarget;
extern bool sDisableTouchUpOnce; // set normally by application if long touch action was made
extern bool sDisableMoveEventsUntilTouchUpIsDone; // Skip all touch move and touch up events until touch is released

void resetTouchFlags();
#endif

extern struct BluetoothEvent remoteEvent;
#if defined(AVR)
// Is used for touch down events. If remoteEvent is not empty, it is used as buffer for next regular event to avoid overwriting of remoteEvent
extern struct BluetoothEvent remoteTouchDownEvent;
#endif

#if !defined(DO_NOT_NEED_BASIC_TOUCH_EVENTS)
extern bool sTouchIsStillDown;
extern struct TouchEvent sDownPosition;
extern struct TouchEvent sCurrentPosition;
extern struct TouchEvent sUpPosition;
#endif

void delayMillisWithCheckAndHandleEvents(unsigned long aDelayMillis);
bool delayMillisAndCheckForEvent(unsigned long aDelayMillis);

void checkAndHandleEvents(void);

void registerLongTouchDownCallback(void (*aLongTouchCallback)(struct TouchEvent *), uint16_t aLongTouchTimeoutMillis);

void registerSwipeEndCallback(void (*aSwipeEndCallback)(struct Swipe *));
void setSwipeEndCallbackEnabled(bool aSwipeEndCallbackEnabled);

void registerConnectCallback(void (*aConnectCallback)(void));
void registerReorientationCallback(void (*aReorientationCallback)(void));

/*
 * Connect and reorientation always include a redraw
 */
void registerRedrawCallback(void (*aRedrawCallback)(void));
void (* getRedrawCallback(void))(void);

void registerSensorChangeCallback(uint8_t aSensorType, uint8_t aSensorRate, uint8_t aFilterFlag,
        void (*aSensorChangeCallback)(uint8_t aSensorType, struct SensorCallback *aSensorCallbackInfo));

// defines for backward compatibility
#define registerSimpleConnectCallback(aConnectCallback) registerConnectCallback(aConnectCallback)
#define registerSimpleResizeAndReconnectCallback(aRedrawCallback) registerRedrawCallback(aRedrawCallback)
#define registerSimpleResizeAndConnectCallback registerRedrawCallback(aRedrawCallback)
#define registerSimpleResizeCallback(aRedrawCallback) registerRedrawCallback(aRedrawCallback)
#define getSimpleResizeAndConnectCallback() getRedrawCallback()

#if !defined(DO_NOT_NEED_BASIC_TOUCH_EVENTS)
void registerTouchDownCallback(void (*aTouchDownCallback)(struct TouchEvent *aActualPositionPtr));
void registerTouchMoveCallback(void (*aTouchMoveCallback)(struct TouchEvent *aActualPositionPtr));
void registerTouchUpCallback(void (*aTouchUpCallback)(struct TouchEvent *aActualPositionPtr));
void setTouchUpCallbackEnabled(bool aTouchUpCallbackEnabled);
void (* getTouchUpCallback(void))(struct TouchEvent * );
#endif

#if defined(SUPPORT_LOCAL_DISPLAY)
void handleLocalTouchUp();
void callbackLongTouchDownTimeout();
void simpleTouchDownHandler(struct TouchEvent *aActualPositionPtr);
void simpleTouchHandlerOnlyForButtons(struct TouchEvent *aActualPositionPtr);
void simpleTouchDownHandlerOnlyForSlider(struct TouchEvent *aActualPositionPtr);
void simpleTouchDownHandlerForSlider(struct TouchEvent *aActualPositionPtr);
void simpleTouchMoveHandlerForSlider(struct TouchEvent *aActualPositionPtr);

// for local autorepeat button
void registerPeriodicTouchCallback(bool (*aPeriodicTouchCallback)(int, int), uint32_t aCallbackPeriodMillis);
void setPeriodicTouchCallbackPeriod(uint32_t aCallbackPeriod);

bool getDisplayXYValuesFlag();
void setDisplayXYValuesFlag(bool aEnableDisplay);
void printTPData(int x, int y,  color16_t aColor,  color16_t aBackColor);
#endif

#ifdef __cplusplus
extern "C" {
#endif
void handleEvent(struct BluetoothEvent *aEvent);
#ifdef __cplusplus
}
#endif

#endif // _EVENTHANDLER_H
