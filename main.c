/*
 * ESE519_Lab4_Pong_Starter.c
 *
 * Created: 9/21/2021 21:21:21 AM
 * Author : J. Ye
 */

 #define F_CPU 16000000UL

 #include <avr/io.h>
 #include "lib\ST7735.h"
 #include "lib\LCD_GFX.h"
 #include <util/delay.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include "uart.h"

#define DOT1_PIN PD0
#define DOT2_PIN PD1
#define DOT3_PIN PD2
#define DOT4_PIN PD3
#define DOT5_PIN PD4
#define DOT6_PIN PD5
#define SOLENOID_PORT PORTD
#define SOLENOID_DDR  DDRD
#define MAX_LINE_LENGTH 64
char buffer[MAX_LINE_LENGTH];
int buffer_index = 0;

#define ACTUATOR_UP_PIN   PB0
#define ACTUATOR_DOWN_PIN PB1

#define ACTUATOR_PORT PORTB
#define ACTUATOR_DDR  DDRB

//void actuator_step_right() {
//    ACTUATOR_PORT &= ~(1 << ACTUATOR_UP_PIN);     // make left  LOW
//    ACTUATOR_PORT |= (1 << ACTUATOR_DOWN_PIN);    // make right HIGH
//    printf("Actuator moving RIGHT\n");
//    _delay_ms(1500);  // time for one character width movement
//    ACTUATOR_PORT &= ~(1 << ACTUATOR_DOWN_PIN);   //stop moving
//}
//
//void actuator_step_left() {
//    ACTUATOR_PORT &= ~(1 << ACTUATOR_DOWN_PIN);   // make right LOW
//    ACTUATOR_PORT |= (1 << ACTUATOR_UP_PIN);      // make LEFT high
//    printf("Actuator moving LEFT\n");
//    _delay_ms(1500);  // time to move for one character
//    ACTUATOR_PORT &= ~(1 << ACTUATOR_UP_PIN);     // stop moving
//}


void activate_solenoids(uint8_t pattern) {
    // clear solenoids
    SOLENOID_PORT &= ~0b00111111;

    // set the solenoids based on braille map
    SOLENOID_PORT |= (pattern & 0b00111111);

//    _delay_ms(1000);  // delay to stamp down
    SOLENOID_PORT &= ~0b00111111;//clear solenoids again
   
    //testing without hardware:
    printf("Braille pattern: 0b");
    for (int i = 5; i >= 0; i--) {
        printf("%d", (pattern >> i) & 1); // Print each bit from dot6 to dot1
    }
    printf("\n");
    printf("Solenoids activated on pins: ");
    for (int i = 0; i < 6; i++) {
        if ((pattern >> i) & 1) {
            printf("PD%d ", i);
        }
    }
    printf("\n\n");
   
    _delay_ms(300); // time to press
    SOLENOID_PORT &= ~0b00111111; //what does this line do?
}



// 6 bits: dot1 is LSB, dot6 is MSB
typedef struct {
    char ascii;
    uint8_t braille_pattern;
} BrailleMap;

BrailleMap braille_table[] = {
    {'a', 0b000001},
    {'b', 0b000011},
    {'c', 0b000101},
    {'d', 0b001101},
    {'e', 0b001001},
    {'f', 0b000111},
    {'g', 0b001111},
    {'h', 0b001011},
    {'i', 0b000110},
    {'j', 0b001110},
    {'k', 0b000101},
    {'l', 0b000111},
    {'m', 0b000111},
    {'n', 0b001111},
    {'o', 0b001011},
    {'p', 0b000111},
    {'q', 0b001111},
    {'r', 0b001011},
    {'s', 0b000110},
    {'t', 0b001110},
    {'u', 0b100101},
    {'v', 0b100111},
    {'w', 0b101110},
    {'x', 0b100111},
    {'y', 0b101111},
    {'z', 0b101011}
};

uint8_t get_braille_pattern(char input) {
    //go through every element in braille table
    for (int i = 0; i < sizeof(braille_table)/sizeof(BrailleMap); i++) {
        //check if ascii field is char of input
        if (braille_table[i].ascii == input) {
            //return the bit value of the braille
            return braille_table[i].braille_pattern;
        }
    }
    return 0;
}

int main(void) {
    uart_init();      
    lcd_init();      
    LCD_setScreen(BLACK);
   
    // set solenoid pins (PD0–PD5) as output
    SOLENOID_DDR |= 0b00111111;
   
    //setup actuator
    ACTUATOR_DDR |= (1 << ACTUATOR_UP_PIN) | (1 << ACTUATOR_DOWN_PIN);

    int cursor_x = 0;
    int cursor_y = 0;

    while (1) {
        if (uart_available()) {
        char c = uart_receive(NULL);

        if (c == '\r' || c == '\n') {
            buffer[buffer_index] = '\0'; // end

            // print the whole string now
            //for each character in buffer print it out and move cursor forward
            for (int i = 0; i < buffer_index; i++) {
                LCD_drawChar(cursor_x, cursor_y, buffer[i], WHITE, BLACK);
                cursor_x += 6;
               
                //get braille pattern
                uint8_t pattern = get_braille_pattern(buffer[i]);
                activate_solenoids(pattern);
               
//                actuator_step_right();
               
                //go to next line if overflow
                if (cursor_x >= LCD_WIDTH - 6) {
                    cursor_x = 0;
                    cursor_y += 10;
                   
//                    actuator_step_left();
                }
               
                //go to new screen if overflow
                if (cursor_y >= LCD_HEIGHT - 10) {
                    LCD_setScreen(BLACK);
                    cursor_x = 0;
                    cursor_y = 0;
                   
//                    actuator_step_left();
                }
            }
            //start new input on new line
            cursor_y += 10;
            cursor_x = 0;

            // reset buffer
            buffer_index = 0;
           
            } else {
                if (buffer_index < MAX_LINE_LENGTH - 1) {
                    buffer[buffer_index++] = c;
                }
            }
        }
        _delay_ms(1);
    }
}
