#define LEDPINS (1 << 18 | 1 << 20 | 1 << 21 | 1 << 23)
#define LEDPORT 1

extern const int LEDS[];
void ledNumber(int number);
void clearLeds();
void setupLeds();