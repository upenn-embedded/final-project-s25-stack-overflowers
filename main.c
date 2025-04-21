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

#define DOT1_PIN PC0
#define DOT2_PIN PC1
#define DOT3_PIN PC2
#define DOT4_PIN PC3
#define DOT5_PIN PC4
#define DOT6_PIN PC5
#define SOLENOID_PORT PORTC
#define SOLENOID_DDR  DDRC
#define MAX_LINE_LENGTH 64
char buffer[MAX_LINE_LENGTH];
int buffer_index = 0;

#define ACTUATOR_UP_PIN   PD2
#define ACTUATOR_DOWN_PIN PD3

#define ACTUATOR_PORT PORTD
#define ACTUATOR_DDR  DDRD

//driver will actually use pwm not gpio
//in video he uses 12v battery connected to driver and 3.7 lithium batteery for breadboard
//vcc (3.7v), ground, both enable pins to positive on breadboard, two jumpers for pwm right and left
//provide with 3.3 or 5 volts, he recommends 5volts
//ground, vcc, ignore next two they are current alarms, forward/reverse next two and speed on pwm
//if we dont need speed control, can just tie the pwm pins high or low
//for moor left to right: batt neg, batt plus, two motor leads, doesnt matter which one where,
//but swap polarity to swap direction of each command
//enable internal pulldown resistors?
//active high signals
//have to make sure logic ground and breadboard ground are
//have to enable both, send pwm on one for forward, the other for reverse and thats it
//both enable pins have to be high
//do we need a 12v battery or is there another option//wall socket or smth?
//motor supply is 6v-27v range

//maybe done make it go all the way back in

void actuator_move_out() {
    ACTUATOR_PORT &= ~(1 << ACTUATOR_DOWN_PIN);   // RIGHT LOW
    ACTUATOR_PORT |= (1 << ACTUATOR_UP_PIN);      // LEFT HIGH
    printf("Actuator moving OUT\n");              // ✅ Corrected print
    _delay_ms(2400);
    ACTUATOR_PORT &= ~(1 << ACTUATOR_UP_PIN);     // stop
}

void actuator_move_in() {
    ACTUATOR_PORT &= ~(1 << ACTUATOR_UP_PIN);     // LEFT LOW
    ACTUATOR_PORT |= (1 << ACTUATOR_DOWN_PIN);    // RIGHT HIGH
    printf("Actuator moving IN\n");               // ✅ Corrected print
    _delay_ms(15000);
    ACTUATOR_PORT &= ~(1 << ACTUATOR_DOWN_PIN);   // stop
}


void activate_solenoids(uint8_t pattern) {
    // clear solenoids
    SOLENOID_PORT &= ~0b00111111;
    // set the solenoids based on braille map
    SOLENOID_PORT |= (pattern & 0b00111111);

    _delay_ms(1000);  // delay to stamp down
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
            printf("PC%d ", i);
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
    {'e', 0b010001},
    {'f', 0b001011},
    {'g', 0b011011},
    {'h', 0b010011},
    {'i', 0b001010},
    {'j', 0b011010},
    {'k', 0b000101},
    {'l', 0b000111},
    {'m', 0b001101},
    {'n', 0b011101},
    {'o', 0b010101},
    {'p', 0b001111},
    {'q', 0b011111},
    {'r', 0b010111},
    {'s', 0b001110},
    {'t', 0b011110},
    {'u', 0b100101},
    {'v', 0b100111},
    {'w', 0b111010},
    {'x', 0b110011},
    {'y', 0b111101},
    {'z', 0b110101}
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
   
   
    // set solenoid pins (PD0?PD5) as output
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
            int char_count = 0;

            // print the whole string now
            //for each character in buffer print it out and move cursor forward
            for (int i = 0; i < buffer_index; i++) {
               
                LCD_drawChar(cursor_x, cursor_y, buffer[i], WHITE, BLACK);
                cursor_x += 6;
               
                //get braille pattern
                uint8_t pattern = get_braille_pattern(buffer[i]);
                activate_solenoids(pattern);
               
                actuator_move_out();
                char_count++;
               
                if (char_count == 14) {
                actuator_move_in();
                char_count = 0;
                }
               
               
                //go to next line if overflow
                if (cursor_x >= LCD_WIDTH - 6) {
                    cursor_x = 0;
                    cursor_y += 10;
                   
//                    actuator_move_in();
                   
                }
               
                //go to new screen if overflow
                if (cursor_y >= LCD_HEIGHT - 10) {
                    LCD_setScreen(BLACK);
                    cursor_x = 0;
                    cursor_y = 0;
                   
                    actuator_move_in();
                   
                }
            }
            //start new input on new line
            actuator_move_in();
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

//int main(void) {
//    // Set PD2 and PD3 as output
//    ACTUATOR_DDR |= (1 << ACTUATOR_UP_PIN) | (1 << ACTUATOR_DOWN_PIN);
//    uart_init();
//
//    while (1) {
//      actuator_move_out();
//       _delay_ms(2000); // Wait 2 seconds
//    }
//}