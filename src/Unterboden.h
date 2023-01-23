template<uint8_t LED_PIN, int NUM_LEDS>
class Unterboden {

    CRGB* leds;
    CLEDController* controller;
    
    long currentPixelHue = 0;
    int offset = NUM_LEDS / 2;
    int regenbogen_offset = 0;

    public:
        
        Unterboden(){
            this->setup();
        }

        void acceleration(){
            for (int i = 0; i < NUM_LEDS + 4; i++) {
                if (i < NUM_LEDS){
                    leds[i] = CRGB::DeepPink;
                }
                delay(log(NUM_LEDS)); //(LED_COUNT-i));
                if (i > 3){
                    leds[i-4] = CRGB::Black;
                }
                controller->showLeds();
            }
            // FastLED.clear();
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
            }
            delay(300);
        }


        void dualColorRangeRotation() {

            offset++;
            if (offset > NUM_LEDS)
                offset = 0;

            currentPixelHue += 64;
            if (currentPixelHue >= 65536)
                currentPixelHue = 0;

            // while (offset < NUM_LEDS){
            //     while (currentPixelHue < 65536){

            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CRGB::Red; //currentPixelHue; //CHSV(currentPixelHue,255,64) //HSVHue::HUE_GREEN; // currentPixelHue;

                // strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(currentPixelHue)));

            }

            for (int i = 0; i < NUM_LEDS / 2; i++) {
                int pos = i + offset;

                if (pos >= NUM_LEDS)
                pos = (i + offset) - NUM_LEDS;

                leds[pos] = CRGB::Green; //(currentPixelHue + 65536 / 2) % 65536;

                // strip.setPixelColor(pos, strip.gamma32(strip.ColorHSV((currentPixelHue + 65536 / 2) % 65536)));
                // strip.setPixelColor(pos, strip.gamma32(strip.ColorHSV(65536 - currentPixelHue))); //alt idk
            }

            currentPixelHue += 64;
            offset++;
            controller->showLeds();
            delay(100);

            //     }
            //     currentPixelHue = 0;
            // }
            // offset = 0;
        }

        void regenbogen_cycle(int SpeedDelay){
            byte *c;
            uint16_t i, j;
      
            for(i=0; i< NUM_LEDS; i++) {
                c=Wheel(((i * 200 / NUM_LEDS) + regenbogen_offset) & 255);
                // setPixel(i, *c, *(c+1), *(c+2));
                leds[i].r = *c;
                leds[i].g = *(c+1);
                leds[i].b = *(c+2);
            }
            controller->showLeds(15);
            delay(SpeedDelay);

            regenbogen_offset++;
            if (regenbogen_offset>=256){
                regenbogen_offset = 0;
            }
            delay(10);
        }

        void clear(){
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
            }
            controller->showLeds(15);
            delay(10);
        }


    private:
        void setup(){
                leds = new CRGB[NUM_LEDS];
                controller = &FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
            }

        byte * Wheel(byte WheelPos) {
            static byte c[3];
            
            if(WheelPos < 85) {
                c[0]=WheelPos * 3;
                c[1]=255 - WheelPos * 3;
                c[2]=0;
            } else if(WheelPos < 170) {
                WheelPos -= 85;
                c[0]=255 - WheelPos * 3;
                c[1]=0;
                c[2]=WheelPos * 3;
            } else {
                WheelPos -= 170;
                c[0]=0;
                c[1]=WheelPos * 3;
                c[2]=255 - WheelPos * 3;
            }
            return c;
        }
        
};
