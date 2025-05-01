#define F_CPU 16000000UL

 #include <avr/io.h>
 #include "lib\ST7735.h"
 #include "lib\LCD_GFX.h"
 #include <util/delay.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include "uart.h"
#include <avr/interrupt.h>

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
volatile int buffer_index = 0;
volatile uint8_t line_ready = 0;

#define ACTUATOR_UP_PIN   PD2
#define ACTUATOR_DOWN_PIN PD3

#define ACTUATOR_PORT PORTD
#define ACTUATOR_DDR  DDRD
#define BUFFER_COUNT 2

void actuator_move_out() {
    ACTUATOR_PORT &= ~(1 << ACTUATOR_DOWN_PIN);   // right LOW
    ACTUATOR_PORT |= (1 << ACTUATOR_UP_PIN);      // left HIGH
    printf("Actuator moving OUT\n");
    //corresponds to the correct spacing of 12mm in between braille characters
    //for double-size braille
    _delay_ms(2400);
    ACTUATOR_PORT &= ~(1 << ACTUATOR_UP_PIN);     // stop
}

void actuator_move_in() {
    ACTUATOR_PORT &= ~(1 << ACTUATOR_UP_PIN);     // left LOW
    ACTUATOR_PORT |= (1 << ACTUATOR_DOWN_PIN);    // right HIGH
    printf("Actuator moving IN\n");
    //time to make it go all the way back in fromm full extension
    _delay_ms(2400);
    ACTUATOR_PORT &= ~(1 << ACTUATOR_DOWN_PIN);   // stop
}

void actuator_fully_extend() {
    ACTUATOR_PORT &= ~(1 << ACTUATOR_DOWN_PIN);   // right LOW
    ACTUATOR_PORT |= (1 << ACTUATOR_UP_PIN);      // left HIGH
    printf("Fully extending actuator\n");
    // Longer time to ensure full extension
    _delay_ms(15000);
    ACTUATOR_PORT &= ~(1 << ACTUATOR_UP_PIN);     // stop
}


void activate_solenoids(uint8_t pattern) {
    // clear solenoids
    SOLENOID_PORT &= ~0b00111111;
    // set the solenoids based on braille map
    SOLENOID_PORT |= (pattern & 0b00111111);

    _delay_ms(1000);  // delay to stamp down
    SOLENOID_PORT &= ~0b00111111;//clear solenoids again
   
//    testing without hardware:
    printf("Braille pattern: 0b");
    for (int i = 5; i >= 0; i--) {
        printf("%d", (pattern >> i) & 1); // print each bit from dot6 to dot1
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
    SOLENOID_PORT &= ~0b00111111;
}

// 6 bits: dot1 is LSB, dot6 is MSB
typedef struct {
    char ascii;
    uint8_t braille_pattern;
} BrailleMap;

BrailleMap braille_table[] = {
    {'a', 0b000001},
    {'b', 0b000011},
    {'c', 0b001001},
    {'d', 0b011001},
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
   
    // set solenoid pins (PD0 through PD5) as output
    SOLENOID_DDR |= 0b00111111;
   
    //setup actuator
    ACTUATOR_DDR |= (1 << ACTUATOR_UP_PIN) | (1 << ACTUATOR_DOWN_PIN);
    int cursor_x = 0;
    int cursor_y = 0;
   
    actuator_fully_extend();


    while (1) {
        if (uart_available()) {
        char c = uart_receive(NULL);
//        printf("Got: %c\n", c);

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
               
                actuator_move_in();//
                char_count++;
               
                //maximum extension of the linear actuator
                //moves it back in to start a new line after 14 char
                if (char_count == 14) {
                actuator_move_out();//changed from move in
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
                   
                    actuator_move_out(); //changed from move in
                   
                }
            }
            //start new input on new line
            actuator_move_out(); //changed from move in
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

////test just the linear actuator
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

//
//// double buffers
//volatile struct {
//    char data[BUFFER_COUNT][MAX_LINE_LENGTH];
//    volatile uint8_t write_index;    //buffer being written to
//    volatile uint8_t read_index;     // buffer to be read
//    volatile uint8_t position;       //pos in writing buffer
//    volatile uint8_t buffer_ready;   // flag to show a buffer is ready to read
//    volatile uint8_t buffer_full;    // Flag to show curr write buffer is full
//} uart_buffer = {
//    .write_index = 0,
//    .read_index = 1,
//    .position = 0,
//    .buffer_ready = 0,
//    .buffer_full = 0
//};
//
//ISR(USART0_RX_vect) {
//    // read the received character
//    char c = UDR0;
//    
//    // check error flags
//    uint8_t status = UCSR0A;
//    if (status & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) {
//        // clear error
//        return;
//    }
//    
//    // debug message
//    static uint8_t debug_counter = 0;
//    
//    // add to buffer
//    if (uart_buffer.position < MAX_LINE_LENGTH - 1) {
//        uart_buffer.data[uart_buffer.write_index][uart_buffer.position++] = c;
//        if (c == '\r' || c == '\n') {
//            // add null terminate
//            uart_buffer.data[uart_buffer.write_index][uart_buffer.position-1] = '\0';
//            
//            // mark as ready
//            uart_buffer.buffer_ready = 1;
//
//            uint8_t temp = uart_buffer.write_index;
//            uart_buffer.write_index = uart_buffer.read_index;
//            uart_buffer.read_index = temp;
//            uart_buffer.position = 0;
//        }
//    }
//    debug_counter++;
//}
//
//// check if line is available
//uint8_t uart_line_available(void) {
//    return uart_buffer.buffer_ready;
//}
//
//// function to get line
//char* uart_get_line(void) {
//    if (uart_buffer.buffer_ready) {
//        return (char*)uart_buffer.data[uart_buffer.read_index];
//    }
//    return NULL;
//}
//
//void uart_line_processed(void) {
//    uart_buffer.buffer_ready = 0;
//}
//
//int main(void) {
//    // initialize
//    uart_init();
//    lcd_init();
//    LCD_setScreen(BLACK);
//    
//    // set solenoids
//    SOLENOID_DDR |= 0b00111111;
//    
//    // setup actuator pins
//    ACTUATOR_DDR |= (1 << ACTUATOR_UP_PIN) | (1 << ACTUATOR_DOWN_PIN);
//    
//    LCD_drawString(0, 0, "uart ready", WHITE, BLACK);
//    
//    // enable interrupts
//    sei();
//    
//    //test
//    printf("uart test\r\n");
//    
//    int cursor_x = 0;
//    int cursor_y = 20;
//    
//    actuator_fully_extend();
//
//    while (1) {
//        // check if available
//        if (uart_line_available()) {
//            // Get the line
//            char* current_line = uart_get_line();
//            
//            // update
//            LCD_drawString(0, 10, "processing", WHITE, BLACK);
//            
//            actuator_fully_extend();
//            int char_count = 0;
//            
//            // process characters in line
//            for (int i = 0; current_line[i] != '\0'; i++) {
//                // display
//                LCD_drawChar(cursor_x, cursor_y, current_line[i], WHITE, BLACK);
//                cursor_x += 6;
//                
//                // map solenoids
//                uint8_t pattern = get_braille_pattern(current_line[i]);
//                activate_solenoids(pattern);
//
//                actuator_move_in();
//                char_count++;
//                
//                if (char_count == 14) {
//                    actuator_move_out();
//                    char_count = 0;
//                }
//                
//                if (cursor_x >= LCD_WIDTH - 6) {
//                    cursor_x = 0;
//                    cursor_y += 10;
//                }
//                
//                if (cursor_y >= LCD_HEIGHT - 10) {
//                    LCD_setScreen(BLACK);
//                    LCD_drawString(0, 0, "uart is ready", WHITE, BLACK);
//                    cursor_x = 0;
//                    cursor_y = 20;
//                    actuator_move_out();
//                }
//            }
//            
//            //mark as processed
//            uart_line_processed();
//            
//            // get ready for next input
//            actuator_move_out();
//            cursor_y += 10;
//            cursor_x = 0;
//            
//            LCD_drawString(0, 10, "ready for next line", WHITE, BLACK);
//        }
//        
//        _delay_ms(10);
//    }
//}
//
//