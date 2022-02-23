template<uint8_t LED_PIN, int NUM_LEDS>
class Dosenrohr {

    CRGB* leds;

    public:
        
        Dosenrohr(){
            this->setup();
        }


        void acceleration(){
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CRGB::BlueViolet;

                delay(log(NUM_LEDS)); //(LED_COUNT-i));
                if (i > 3){
                    leds[i-4] = CRGB::Black;
                }
                FastLED.show();
            }
            // FastLED.clear();
            clear();
            delay(300);
        }


        void shine(){
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Green;
            }
            FastLED.show();
            delay(500);
            // for blink
            // clear();
            // delay(500);
        }


        void bouncingColoredBalls(int BallCount, byte colors[][3]) {
            float Gravity = -9.81;
            int StartHeight = 1;
            
            float Height[BallCount];
            float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
            float ImpactVelocity[BallCount];
            float TimeSinceLastBounce[BallCount];
            int   Position[BallCount];
            long  ClockTimeSinceLastBounce[BallCount];
            float Dampening[BallCount];
            
            for (int i = 0 ; i < BallCount ; i++) {  
                ClockTimeSinceLastBounce[i] = millis();
                Height[i] = StartHeight;
                Position[i] = 0;
                ImpactVelocity[i] = ImpactVelocityStart;
                TimeSinceLastBounce[i] = 0;
                Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
            }

            while (true) {
                for (int i = 0 ; i < BallCount ; i++) {
                TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
                Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
            
                if ( Height[i] < 0 ) {                      
                    Height[i] = 0;
                    ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
                    ClockTimeSinceLastBounce[i] = millis();
            
                    if ( ImpactVelocity[i] < 0.01 ) {
                    ImpactVelocity[i] = ImpactVelocityStart;
                    }
                }
                Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
                }
            
                for (int i = 0 ; i < BallCount ; i++) {                    
                    leds[Position[i]].r = colors[i][0];
                    leds[Position[i]].g = colors[i][1];
                    leds[Position[i]].b = colors[i][2];
                }
            
                FastLED.show();
                // strip.show();
                // setAll(0,0,0);
                // FastLED.clear();
                delay(10);
                clear();
            }
        }


        void runningLightGroup(byte red, byte green, byte blue, int WaveDelay) {

            int Position=0;
            
            for(int j=0; j<NUM_LEDS*2; j++)
            {
                Position++; // = 0; //Position + Rate;
                for(int i=0; i<NUM_LEDS; i++) {
                    // sine wave, 3 offset waves make a rainbow!
                    //float level = sin(i+Position) * 127 + 128;
                    //strip.setPixelColor(i,level,0,0);
                    //float level = sin(i+Position) * 127 + 128;
                    leds[i].r = ((sin(i+Position) * 127 + 128)/255)*red;
                    leds[i].g = ((sin(i+Position) * 127 + 128)/255)*green;
                    leds[i].b = ((sin(i+Position) * 127 + 128)/255)*blue;
                }
                
                FastLED.show();
                delay(WaveDelay);
            }
        }


        void Fire(int Cooling, int Sparking, int SpeedDelay) {
            static byte heat[NUM_LEDS];
            int cooldown;
            
            // Step 1.  Cool down every cell a little
            for( int i = 0; i < NUM_LEDS; i++) {
                cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
            
                if(cooldown>heat[i]) {
                    heat[i]=0;
                } else {
                    heat[i]=heat[i]-cooldown;
                }
            }
            
            // Step 2.  Heat from each cell drifts 'up' and diffuses a little
            for( int k = NUM_LEDS - 1; k >= 2; k--) {
                heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
            }
            
            // Step 3.  Randomly ignite new 'sparks' near the bottom
            if( random(255) < Sparking ) {
                int y = random(7);
                heat[y] = heat[y] + random(160,255);
                //heat[y] = random(160,255);
            }

            // Step 4.  Convert heat to LED colors
            for( int j = 0; j < NUM_LEDS; j++) {
                setPixelHeatColor(j, heat[j] );
            }

            FastLED.show();
            delay(SpeedDelay);
        }


        void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
            setAll(0,0,0);
            
            for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
            
            
                // fade brightness all LEDs one step
                for(int j=0; j<NUM_LEDS; j++) {
                    if( (!meteorRandomDecay) || (random(10)>5) ) {      
                        leds[j].fadeToBlackBy( meteorTrailDecay );
                    }
                }
            
                // draw meteor
                for(int j = 0; j < meteorSize; j++) {
                    if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
                        // setPixel(i-j, red, green, blue);
                        leds[i-j].r = red;
                        leds[i-j].g = green;
                        leds[i-j].b = blue;
                    }
                }
            
                FastLED.show();
                delay(SpeedDelay);
            }
        }

        void clear(){
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
            }
            FastLED.show();
            delay(10);
        }


    private:
        void setup(){
            leds = new CRGB[NUM_LEDS];
            FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
        }

        void setAll(byte red, byte green, byte blue) {
            for(int i = 0; i < NUM_LEDS; i++ ) {
                leds[i].r = red;
                leds[i].g = green;
                leds[i].b = blue;
            }
            FastLED.show();
        }

        void setPixelHeatColor (int Pixel, byte temperature) {
            // Scale 'heat' down from 0-255 to 0-191
            byte t192 = round((temperature/255.0)*191);
            
            // calculate ramp up from
            byte heatramp = t192 & 0x3F; // 0..63
            heatramp <<= 2; // scale up to 0..252
            
            // figure out which third of the spectrum we're in:
            if( t192 > 0x80) {                     // hottest
                // setPixel(Pixel, 255, 255, heatramp);
                leds[Pixel].r = 255;
                leds[Pixel].g = 255;
                leds[Pixel].b = heatramp;
            } else if( t192 > 0x40 ) {             // middle
                // setPixel(Pixel, 255, heatramp, 0);
                leds[Pixel].r = 255;
                leds[Pixel].g = heatramp;
                leds[Pixel].b = 0;
            } else {                               // coolest
                // setPixel(Pixel, heatramp, 0, 0);
                leds[Pixel].r = heatramp;
                leds[Pixel].g = 0;
                leds[Pixel].b = 0;
            }
        }      
};
