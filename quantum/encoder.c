/*
 * Copyright 2018 Jack Humbert <jack.humb@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "encoder.h"
#ifdef SPLIT_KEYBOARD
#    include "split_util.h"
#endif

// for memcpy
#include <string.h>

#if !defined(ENCODER_RESOLUTIONS) && !defined(ENCODER_RESOLUTION)
#    define ENCODER_RESOLUTION 4
#endif

#if !defined(ENCODERS_PAD_A) || !defined(ENCODERS_PAD_B)
#    error "No encoder pads defined by ENCODERS_PAD_A and ENCODERS_PAD_B"
#endif

#define NUMBER_OF_ENCODERS (sizeof(encoders_pad_a) / sizeof(*encoders_pad_a))

#ifdef ENCODER_MATRIX
#define ENCODER_ROWS (sizeof(encoder_row_pins)/sizeof(*encoder_row_pins))
#define ENCODER_COLS (sizeof(encoder_col_pins)/sizeof(*encoder_col_pins))
static pin_t encoder_row_pins[] = ENCODER_ROW_PINS;
static pin_t encoder_col_pins[] = ENCODER_COL_PINS;
typedef struct {
    uint8_t row;
    uint8_t col;
} encoder_pin_pair;
static encoder_pin_pair PROGMEM encoders_pad_a[] = ENCODERS_PAD_A;
static encoder_pin_pair PROGMEM encoders_pad_b[] = ENCODERS_PAD_B;
/* max 32 cols for now */
static uint32_t encoder_matrix[ENCODER_ROWS];

#define ENCODER_MATRIX_READ(row, col) (!!(encoder_matrix[row] & (((uint32_t)1) << col)))
#else
static pin_t encoders_pad_a[] = ENCODERS_PAD_A;
static pin_t encoders_pad_b[] = ENCODERS_PAD_B;
#endif

#ifdef ENCODER_RESOLUTIONS
static uint8_t encoder_resolutions[] = ENCODER_RESOLUTIONS;
#endif

#ifndef ENCODER_DIRECTION_FLIP
#    define ENCODER_CLOCKWISE true
#    define ENCODER_COUNTER_CLOCKWISE false
#else
#    define ENCODER_CLOCKWISE false
#    define ENCODER_COUNTER_CLOCKWISE true
#endif
static int8_t encoder_LUT[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

static uint8_t encoder_state[NUMBER_OF_ENCODERS]  = {0};
static int8_t  encoder_pulses[NUMBER_OF_ENCODERS] = {0};

#ifdef SPLIT_KEYBOARD
// right half encoders come over as second set of encoders
static uint8_t encoder_value[NUMBER_OF_ENCODERS * 2] = {0};
// row offsets for each hand
static uint8_t thisHand, thatHand;
#else
static uint8_t encoder_value[NUMBER_OF_ENCODERS] = {0};
#endif

__attribute__((weak)) void encoder_update_user(int8_t index, bool clockwise) {}

__attribute__((weak)) void encoder_update_kb(int8_t index, bool clockwise) { encoder_update_user(index, clockwise); }

#ifdef VIAL_ENCODERS_ENABLE
#include "vial.h"
#define encoder_update_kb vial_encoder_update
#endif

#ifdef ENCODER_MATRIX
static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON { setPinInputHigh(pin); }
}
#endif

void encoder_init(void) {
#if defined(SPLIT_KEYBOARD) && defined(ENCODERS_PAD_A_RIGHT) && defined(ENCODERS_PAD_B_RIGHT)
    if (!isLeftHand) {
        const pin_t encoders_pad_a_right[] = ENCODERS_PAD_A_RIGHT;
        const pin_t encoders_pad_b_right[] = ENCODERS_PAD_B_RIGHT;
#    if defined(ENCODER_RESOLUTIONS_RIGHT)
        const uint8_t encoder_resolutions_right[] = ENCODER_RESOLUTIONS_RIGHT;
#    endif
        for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
            encoders_pad_a[i] = encoders_pad_a_right[i];
            encoders_pad_b[i] = encoders_pad_b_right[i];
#    if defined(ENCODER_RESOLUTIONS_RIGHT)
            encoder_resolutions[i] = encoder_resolutions_right[i];
#    endif
        }
    }
#endif

#ifdef ENCODER_MATRIX
    for (size_t i = 0; i < ENCODER_ROWS; ++i)
        setPinInputHigh_atomic(encoder_row_pins[i]);
    for (size_t i = 0; i < ENCODER_COLS; ++i)
        setPinInputHigh_atomic(encoder_col_pins[i]);
#else
    for (int i = 0; i < NUMBER_OF_ENCODERS; i++) {
        setPinInputHigh(encoders_pad_a[i]);
        setPinInputHigh(encoders_pad_b[i]);

        encoder_state[i] = (readPin(encoders_pad_a[i]) << 0) | (readPin(encoders_pad_b[i]) << 1);
    }
#endif

#ifdef SPLIT_KEYBOARD
    thisHand = isLeftHand ? 0 : NUMBER_OF_ENCODERS;
    thatHand = NUMBER_OF_ENCODERS - thisHand;
#endif
}

static bool encoder_update(int8_t index, uint8_t state) {
    bool    changed = false;
    uint8_t i       = index;

#ifdef ENCODER_RESOLUTIONS
    int8_t resolution = encoder_resolutions[i];
#else
    int8_t resolution = ENCODER_RESOLUTION;
#endif

#ifdef SPLIT_KEYBOARD
    index += thisHand;
#endif
    encoder_pulses[i] += encoder_LUT[state & 0xF];
    if (encoder_pulses[i] >= resolution) {
        encoder_value[index]++;
        changed = true;
        encoder_update_kb(index, ENCODER_COUNTER_CLOCKWISE);
    }
    if (encoder_pulses[i] <= -resolution) {  // direction is arbitrary here, but this clockwise
        encoder_value[index]--;
        changed = true;
        encoder_update_kb(index, ENCODER_CLOCKWISE);
    }
    encoder_pulses[i] %= resolution;
    return changed;
}

#ifdef ENCODER_MATRIX
static void encoder_matrix_read_row(size_t row) {
    setPinOutput_writeLow(encoder_row_pins[row]);
    matrix_io_delay();

    uint32_t line = 0;
    for (size_t col = 0; col < ENCODER_COLS; ++col) {
        uint8_t pin_state = readPin(encoder_col_pins[col]);
        line |= pin_state ? 0 : (((uint32_t)1) << col);
    }

    setPinInputHigh_atomic(encoder_row_pins[row]);
    matrix_io_delay();

    encoder_matrix[row] = line;
}

static void encoder_matrix_read(void) {
    for (size_t row = 0; row < ENCODER_ROWS; ++row)
        encoder_matrix_read_row(row);
}
#endif

bool encoder_read(void) {
#ifdef ENCODER_MATRIX
    encoder_matrix_read();
#endif

    bool changed = false;
    for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        uint8_t state;
#ifdef ENCODER_MATRIX
        size_t row_a = pgm_read_byte(&encoders_pad_a[i].row), col_a = pgm_read_byte(&encoders_pad_a[i].col);
        size_t row_b = pgm_read_byte(&encoders_pad_b[i].row), col_b = pgm_read_byte(&encoders_pad_b[i].col);
        state = (ENCODER_MATRIX_READ(row_a, col_a) << 0) | (ENCODER_MATRIX_READ(row_b, col_b) << 1);
#else
        state = (readPin(encoders_pad_a[i]) << 0) | (readPin(encoders_pad_b[i]) << 1);
#endif

        encoder_state[i] <<= 2;
        encoder_state[i] |= state;
        changed |= encoder_update(i, encoder_state[i]);
    }
    return changed;
}

#ifdef SPLIT_KEYBOARD
void last_encoder_activity_trigger(void);

void encoder_state_raw(uint8_t* slave_state) { memcpy(slave_state, &encoder_value[thisHand], sizeof(uint8_t) * NUMBER_OF_ENCODERS); }

void encoder_update_raw(uint8_t* slave_state) {
    bool changed = false;
    for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        uint8_t index = i + thatHand;
        int8_t  delta = slave_state[i] - encoder_value[index];
        while (delta > 0) {
            delta--;
            encoder_value[index]++;
            changed = true;
            encoder_update_kb(index, ENCODER_COUNTER_CLOCKWISE);
        }
        while (delta < 0) {
            delta++;
            encoder_value[index]--;
            changed = true;
            encoder_update_kb(index, ENCODER_CLOCKWISE);
        }
    }

    // Update the last encoder input time -- handled external to encoder_read() when we're running a split
    if (changed) last_encoder_activity_trigger();
}
#endif
