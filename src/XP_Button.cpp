// XP_Button Library
// https://github.com/Xylopyrographer/XP_Button
// Copyright (C) 2023 by Xylopyrographer and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#include "XP_Button.h"

/*  ----------------------------------------------------------------------*
    initialize a Button object and the pin it's connected to.
    -----------------------------------------------------------------------*/
void Button::begin() {
    pinMode( m_pin, m_puEnable ? INPUT_PULLUP : INPUT );
    m_state = digitalRead( m_pin );
    if ( m_invert )
        m_state = !m_state;
    m_time = millis();
    m_lastState = m_state;
    m_lastTransient = !m_state;
    m_changed = false;
    m_stable = false;
    m_lastChange = m_time;
    m_lastDbTime = m_time;
}

/*  ----------------------------------------------------------------------
    returns the state of the button, true if pressed, false if released.
    does debouncing, captures and maintains times, previous state, etc.
    -----------------------------------------------------------------------*/
bool Button::read() {
    uint32_t ms = millis();
    bool pinVal = digitalRead( m_pin );
    if ( m_invert )
        pinVal = !pinVal;
    if ( pinVal != m_lastTransient ) {
        m_lastDbTime = ms;
        m_lastTransient = pinVal;
        m_stable = false;
        m_changed = false;
    }
    if ( ( ms - m_lastDbTime ) > m_dbTime ) {
        m_lastState = m_state;
        m_state = pinVal;
        m_stable = true;
        m_changed = ( m_state != m_lastState );
        if ( m_changed )
            m_lastChange = ms;
    }
    m_time = ms;
    return m_state;
}

/*  ----------------------------------------------------------------------*
    isPressed() and isReleased() check the button state when it was last
    read, and return false (0) or true (!=0) accordingly.
    these functions do not cause the button to be read.
    ----------------------------------------------------------------------*/
bool Button::isPressed() {
    return m_state;
}

bool Button::isReleased() {
    return !m_state;
}

/*  ----------------------------------------------------------------------*
    wasPressed() and wasReleased() check the button state to see if it
    changed between the last two reads and return false (0) or
    true (!=0) accordingly.
    these functions do not cause the button to be read.
    ----------------------------------------------------------------------*/
bool Button::wasPressed() {
    return m_state && m_changed;
}

bool Button::wasReleased() {
    return !m_state && m_changed;
}

/*  ----------------------------------------------------------------------*
    pressedFor(ms) and releasedFor(ms) check to see if the button is
    pressed (or released), and has been in that state for the specified
    time in milliseconds. Returns false (0) or true (!=0) accordingly.
    these functions do not cause the button to be read.
    ----------------------------------------------------------------------*/
bool Button::pressedFor( uint32_t ms ) {
    return m_state && m_time - m_lastChange >= ms;
}

bool Button::releasedFor( uint32_t ms ) {
    return !m_state && m_time - m_lastChange >= ms;
}

/*  ----------------------------------------------------------------------*
    isStable() returns true if the button has not changed state for at
    least the debounce time, false otherwise.
    does not cause the variable to be read.
    -----------------------------------------------------------------------*/
bool Button::isStable() {
    return m_stable;
}

/*  ----------------------------------------------------------------------*
    lastChange() returns the time the button last changed state,
    in milliseconds.
    ----------------------------------------------------------------------*/
uint32_t Button::lastChange() {
    return m_lastChange;
}
