/*
Copyright %YEAR% %YOUR_NAME%

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x0000
#define DEVICE_VER      0x0001
#define MANUFACTURER    xyz
#define PRODUCT         knobpad

/* key matrix size */
#define MATRIX_ROWS 9
#define MATRIX_COLS 4

#define MATRIX_ROW_PINS { A1, A2, A3, A4, A5, A6, B8, B9, B10 }
#define MATRIX_COL_PINS { A9, B0, B4, B7 }

#define DIODE_DIRECTION COL2ROW

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5
/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

#define VIAL_KEYBOARD_UID {0xA5, 0x46, 0x30, 0xF2, 0x20, 0xA3, 0xCE, 0x38}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 1 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }

#define USB_POLLING_INTERVAL_MS 1


#define ENCODER_MATRIX
#define ENCODER_ROW_PINS { A1, A2, A3, A4, A5, A6, B8, B9, B10 }
#define ENCODER_COL_PINS { A7, A8, A10, A15, B1, B3, B5, B6 }
/* row, col pairs */
#define ENCODERS_PAD_A { { 0, 0 }, { 0, 2 }, { 0, 4 }, { 0, 6 }, \
                         { 1, 0 }, { 1, 2 }, { 1, 4 }, { 1, 6 }, \
                         { 2, 0 }, { 2, 2 }, { 2, 4 }, { 2, 6 }, \
                         { 3, 0 }, { 3, 2 }, { 3, 4 }, { 3, 6 }, \
                         { 4, 0 }, { 4, 2 }, { 4, 4 }, { 4, 6 }, \
                         { 5, 0 }, { 5, 2 }, { 5, 4 }, { 5, 6 }, \
                         { 6, 0 }, { 6, 2 }, { 6, 4 }, { 6, 6 }, \
                         { 7, 0 }, { 7, 2 }, { 7, 4 }, { 7, 6 }, \
                         { 8, 0 }, { 8, 2 }, { 8, 4 }, { 8, 6 } }
#define ENCODERS_PAD_B { { 0, 1 }, { 0, 3 }, { 0, 5 }, { 0, 7 }, \
                         { 1, 1 }, { 1, 3 }, { 1, 5 }, { 1, 7 }, \
                         { 2, 1 }, { 2, 3 }, { 2, 5 }, { 2, 7 }, \
                         { 3, 1 }, { 3, 3 }, { 3, 5 }, { 3, 7 }, \
                         { 4, 1 }, { 4, 3 }, { 4, 5 }, { 4, 7 }, \
                         { 5, 1 }, { 5, 3 }, { 5, 5 }, { 5, 7 }, \
                         { 6, 1 }, { 6, 3 }, { 6, 5 }, { 6, 7 }, \
                         { 7, 1 }, { 7, 3 }, { 7, 5 }, { 7, 7 }, \
                         { 8, 1 }, { 8, 3 }, { 8, 5 }, { 8, 7 } }

#define aVIAL_ENCODER_DEFAULT { \
    KC_A,S(KC_A), KC_G,S(KC_G) \
}
