template<uint8_t LED_PIN, int NUM_LEDS>
class Unterboden {

    CRGB* leds;
    
    long currentPixelHue = 0;
    int offset = NUM_LEDS / 2;

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
                FastLED.show();
            }
            // FastLED.clear();
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
            }
            delay(300);
        }

        // alter code
        // dualColorRangeRotation();
        // offset++;
        // if (offset > LED_COUNT)
        //     offset = 0;

        // currentPixelHue += 64;
        // if (currentPixelHue >= 65536)
        //     currentPixelHue = 0;

        void dualColorRangeRotation() {

            while (offset <= NUM_LEDS){
                while (currentPixelHue < 65536){

                    for (int i = 0; i < NUM_LEDS; i++) {
                        leds[i] = CRGB::AliceBlue; //currentPixelHue; //CHSV(currentPixelHue,255,64) //HSVHue::HUE_GREEN; // currentPixelHue;

                        // strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(currentPixelHue)));

                    }

                    for (int i = 0; i < NUM_LEDS / 2; i++) {
                        int pos = i + offset;

                        if (pos >= NUM_LEDS)
                        pos = (i + offset) - NUM_LEDS;

                        leds[pos] = CRGB::Amethyst; //(currentPixelHue + 65536 / 2) % 65536;

                        // strip.setPixelColor(pos, strip.gamma32(strip.ColorHSV((currentPixelHue + 65536 / 2) % 65536)));
                        // strip.setPixelColor(pos, strip.gamma32(strip.ColorHSV(65536 - currentPixelHue))); //alt idk
                    }

                    FastLED.show();
                    delay(100);
                    currentPixelHue += 64;
                    offset++;

                }
                currentPixelHue = 0;
            }
            offset = 0;
        }


    private:
        void setup(){
                leds = new CRGB[NUM_LEDS];
                FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
            }

        
};
