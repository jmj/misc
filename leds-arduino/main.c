#include <WProgram.h>

#define SLEEPER 200

volatile int multiplyer = 5;
int dir1 = 1;
int dir2 = 1;
int mode = 0;


void setup() {
    /* Setup */
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
}

void loop() {
    PORTD = 0;

    switch(mode%5) {
        case 0:
            digitalWrite(3, HIGH);
            break;
        case 1:
            digitalWrite(4, HIGH);
            break;
        case 2:
            digitalWrite(5, HIGH);
            break;
        case 3:
            digitalWrite(6, HIGH);
            break;
        case 4:
            digitalWrite(7, HIGH);
            break;
    }

    if (dir2) {
        mode++;
        if (mode==4) {
            dir2 = 0;
        }
    }
    else {
        mode--;
        if (mode==0) {
            dir2 = 1;
        }
    }

    delay(SLEEPER*multiplyer);

}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

