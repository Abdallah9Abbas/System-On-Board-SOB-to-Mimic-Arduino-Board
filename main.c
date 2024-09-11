#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// Define the keypad configuration
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3

#define KEYPAD_DDR  DDRB
#define KEYPAD_PORT PORTB
#define KEYPAD_PIN  PINB

#define KEYPAD_R1 PB2
#define KEYPAD_R2 PB3
#define KEYPAD_R3 PB4
#define KEYPAD_R4 PB5
#define KEYPAD_C1 PC0
#define KEYPAD_C2 PC1
#define KEYPAD_C3 PC2

#define KEYPAD_DEBOUNCE_MS 50 // Reduce the debounce time to 10ms for faster response

#define setbit(Reg, pinNumber) Reg |= (1 << pinNumber) // Use the bitwise OR operator to set a bit high
#define clrbit(Reg, pinNumber) Reg &= ~(1 << pinNumber) // Use the bitwise AND operator to set a bit low
#define togglebit(Reg, pinNumber) Reg ^= (1 << pinNumber) // Use the bitwise XOR operator to toggle a bit
#define readbit(REG, pinNumber) ((REG >> pinNumber) & 1) // Use bit-shifting and bitwise AND to read a bit

// Create a lookup table for the display values
const uint8_t DISPLAY_LOOKUP[] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

uint8_t keypad_scan(void) {
	uint8_t key = 0xFF;
	uint8_t row, col;

	for (col = 0; col < KEYPAD_COLS; col++) {
		PORTC = (~(1 << (col + KEYPAD_C1)));
		_delay_ms(KEYPAD_DEBOUNCE_MS);
		
		for (row = 0; row < KEYPAD_ROWS; row++) {
			if (!readbit(KEYPAD_PIN, row + KEYPAD_R1)) {
				key = (row * KEYPAD_COLS) + col + 1;
				break;
			}
		}
		PORTC |= (1 << (col + KEYPAD_C1));
	}

	return key;
}

int main(void) {
	uint8_t key;
	uint8_t last_key = 0xFF;

	// Set keypad columns as output
	DDRC |= (1 << KEYPAD_C1) | (1 << KEYPAD_C2) | (1 << KEYPAD_C3);

	// Set keypad rows as input with pull-up resistors enabled
	KEYPAD_DDR &= ~((1 << KEYPAD_R1) | (1 << KEYPAD_R2) | (1 << KEYPAD_R3) | (1 << KEYPAD_R4));
	KEYPAD_PORT |= (1 << KEYPAD_R1) | (1 << KEYPAD_R2) | (1 << KEYPAD_R3) | (1 << KEYPAD_R4);

	// Set PORTD as output
	DDRD = 0xFF;
	PORTD = 0x00;

	while (1) {
		key = keypad_scan(); //Check the pressed button 
		if (key != last_key) { //Check that the pressed key is not repeated
			if (key != 0xFF) { //Check that the button is pressed
				switch (key) {
					case 10:
					PORTD = 0x00;
					break;
					case 11:
					PORTD = 0x3F;
					break;
					case 1:
					PORTD = 0x06;
					break;
					case 2:
					PORTD = 0x5B;
					break;
					case 3:
					PORTD = 0x4F;
					break;
					case 4:
					PORTD = 0x66;
					break;
					case 5:
					PORTD = 0x6D;
					break;
					case 6:
					PORTD = 0x7D;
					break;
					case 7:
					PORTD = 0x07;
					break;
					case 8:
					PORTD = 0x7F;
					break;
					case 9:
					PORTD = 0x6F;
					break;
				}

				}
			last_key = key; //Updated the pressed key
		}

		_delay_ms(KEYPAD_DEBOUNCE_MS);
	}
	return 0;
}
