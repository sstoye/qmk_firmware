/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software : you can redistribute it and /or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.If not, see < http://www.gnu.org/licenses/>.
 */

#include "quantum.h"

// clang-format off

#ifdef RGB_MATRIX_ENABLE
const snled27351_led_t g_snled27351_leds[RGB_MATRIX_LED_COUNT] = {
/* Refer to SNLED27351 manual for these locations
 *   driver
 *   |  R location
 *   |  |       G location
 *   |  |       |       B location
 *   |  |       |       | */
    {0, J_2,	L_2,   K_2},
    {0, J_3,	L_3,   K_3},
    {0, J_4,	L_4,   K_4},
    {0, J_5,	L_5,   K_5},

    {0, J_6,	L_6,   K_6},
    {0, J_7,	L_7,   K_7},
    {0, J_8,	L_8,   K_8},
    {0, J_9,	L_9,   K_9},
    {0, J_10,	L_10,  K_10},
	
    {0, G_1,	I_1,   H_1},
    {0, G_2,	I_2,   H_2},
    {0, G_3,	I_3,   H_3},
    {0, G_4,	I_4,   H_4},
    {0, G_5,	I_5,   H_5},

    {0, G_6,	I_6,   H_6},
    {0, G_7,	I_7,   H_7},
    {0, G_8,	I_8,   H_8},
    {0, G_9,	I_9,   H_9},

    {0, A_1,    C_1,   B_1},
    {0, A_2,    C_2,   B_2},
    {0, A_3,    C_3,   B_3},
    {0, A_4,    C_4,   B_4},
    {0, A_5,    C_5,   B_5},

    {0, A_6,    C_6,   B_6},
    {0, A_7,    C_7,   B_7},
    {0, A_9,    C_9,   B_9},
};

#define __ NO_LED

led_config_t g_led_config = {
    {
        // Key Matrix to LED Index
        { __,  0,  1,  2,  3 },
        {  4,  5,  6,  7,  8 }, 
        {  9, 10, 11, 12, 13 },
        { 14, 15, 16, 17, __ },
        { 18, 19, 20, 21, 22 },
        { 23, 24, __, 25, __ }

    },
    {
        //  LED Index to Physical Position
                 {102, 0},{117, 0}, {132, 0}, {147, 0}, 
        {87,15}, {102,15},{117,15}, {132,15}, {147,15}, 
        {87,26}, {102,26},{117,26}, {132,26}, {147,32}, 
        {87,38}, {102,38},{117,38}, {132,38}, 
        {87,49}, {102,49},{117,49}, {132,49}, {147,55}, 
        {87,61}, {110,61},          {132,61},                 
    },
    {
        // RGB LED Index to Flag
           1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1,    1,         
    }
};
#endif

extern const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];

// Define a value for “no LED” (the __ entries in your mapping)
#ifndef NO_LED
#define NO_LED 0xFF
#endif

// Custom indicator LED indexes (for keys that show the current active layer)
enum custom_leds {
  LAYER_LED_0 = 0,
  LAYER_LED_1,
  LAYER_LED_2,
  LAYER_LED_3,
  // ... add more if needed
};

// If the SET_LED_ON/OFF macros aren’t available in your keyboard.c,
// define them as simple wrappers around rgb_matrix_set_color.
#ifndef SET_LED_ON
#define SET_LED_ON(i)  rgb_matrix_set_color((i), 0xFF, 0xFF, 0xFF)
#endif
#ifndef SET_LED_OFF
#define SET_LED_OFF(i) rgb_matrix_set_color((i), 0x00, 0x00, 0x00)
#endif

uint8_t get_key_action_layer(uint8_t row, uint8_t col) {
    keypos_t key = { .row = row, .col = col };
    // Iterate from the highest active layer down to 0.
    for (int8_t layer = biton32(layer_state); layer >= 0; layer--) {
        action_t action = action_for_key(layer, key);
        if (action.code != KC_TRNS) {
            return layer;
        }
    }
    return 0; // Fallback to base layer if no non-transparent action is found.
}

// The rgb_matrix_indicators_user function is called by QMK to update LED colors.
bool rgb_matrix_indicators_user(void) {
    static const uint8_t layer_colors[4][3] = {
        {0x99, 0xd9, 0xEA}, // Layer 0: turkish
        {0xFF, 0x3B, 0x8B}, // Layer 1: rose
        {0x3B, 0x7C, 0xFF}, // Layer 2: blue
        {0x3B, 0xFF, 0x3B}  // Layer 3: lime
    };

    // --- Custom Indicator Logic for Designated Keys ---
    // Get the highest active layer (this is used only for the indicator keys).
    uint8_t active_layer = get_highest_layer(layer_state);
    
    // Clear the custom indicator LEDs.
    for (uint8_t i = LAYER_LED_0; i <= LAYER_LED_3; i++) {
        SET_LED_OFF(i);
    }
    
    // Retrieve the color for that layer.
    uint8_t r = layer_colors[active_layer][0];
    uint8_t g = layer_colors[active_layer][1];
    uint8_t b = layer_colors[active_layer][2];


    // Light one of the custom indicator LEDs based on the current active layer.
    switch (active_layer) {
        case 0:
	    rgb_matrix_set_color(LAYER_LED_0, r, g, b);
            break;
        case 1:
	    rgb_matrix_set_color(LAYER_LED_1, r, g, b);
            break;
        case 2:
	    rgb_matrix_set_color(LAYER_LED_2, r, g, b);
            break;
        case 3:
	    rgb_matrix_set_color(LAYER_LED_3, r, g, b);
            break;
        default:
            // For any layers beyond 3, light all the indicators.
            for (uint8_t i = LAYER_LED_0; i <= LAYER_LED_3; i++) {
                SET_LED_ON(i);
            }
            break;
    }
    HSV      hsv  = rgb_matrix_config.hsv;	
    uint8_t brightness = hsv.v;

    // Iterate over the key matrix (using MATRIX_ROWS and MATRIX_COLS defined in your config)
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            // Look up the LED index for this key from your key-to-LED mapping.
            uint8_t led_index = g_led_config.matrix_co[row][col];
            
            // Skip if there’s no LED for this position.
            if (led_index == NO_LED) {
                continue;
            }
            // Skip keys that are reserved for the custom indicator logic.
            if (led_index >= LAYER_LED_0 && led_index <= LAYER_LED_3) {
                continue;
            }
            
            // Determine on which layer this key’s action is defined.
            uint8_t defined_layer = get_key_action_layer(row, col);
            // If your layer_colors array is defined only for layers 0–3, bound the index.
            if (defined_layer >= (sizeof(layer_colors) / sizeof(layer_colors[0]))) {
                defined_layer = 0; // fallback to layer 0
            }
            

            // Retrieve the color for that layer.
            r = layer_colors[defined_layer][0];
            g = layer_colors[defined_layer][1];
            b = layer_colors[defined_layer][2];
           
            uint8_t r_dimmed = (r * brightness) / 255;
            uint8_t g_dimmed = (g * brightness) / 255;
            uint8_t b_dimmed = (b * brightness) / 255;

            // Set the LED for this key to the appropriate color.
	    rgb_matrix_set_color(led_index, r_dimmed, g_dimmed, b_dimmed);
        }
    }
    return true;
}
