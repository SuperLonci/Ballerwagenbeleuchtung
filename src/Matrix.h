enum Hintergrund {rainbow_plasma, rainbow_stripe};
enum Sprite {none, smiley};

template<uint8_t LED_PIN>
class Matrix {

    #define COLOR_ORDER    GRB
    #define CHIPSET        WS2812B
    #define MATRIX_WIDTH   16
    #define MATRIX_HEIGHT  32
    #define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX

    // For Plasma Function
    #define PLASMA_X_FACTOR  24
    #define PLASMA_Y_FACTOR  24

    uint16_t PlasmaTime, PlasmaShift;
    uint8_t plasmaLowHue, plasmaHighHue, plasmaBrightness;

    uint8_t seconds = 0;
    uint8_t secondsTrigger = 0;

    // sprites
    // uint8_t sprite = 4;
    // Smiley
    const uint8_t SpriteSmileyData[480] = {
    // Frame 1 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111111),B8_2BIT(11111000),
    B8_2BIT(01111211),B8_2BIT(12111100),
    B8_2BIT(01111211),B8_2BIT(12111100),
    B8_2BIT(11111211),B8_2BIT(12111110),
    B8_2BIT(11111111),B8_2BIT(11111110),
    B8_2BIT(11111111),B8_2BIT(11111110),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(01121111),B8_2BIT(11121100),
    B8_2BIT(01112111),B8_2BIT(11211100),
    B8_2BIT(00111222),B8_2BIT(22111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000),
    // Frame 2 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111111),B8_2BIT(12111000),
    B8_2BIT(01111111),B8_2BIT(21111100),
    B8_2BIT(01121112),B8_2BIT(11111100),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(11211111),B8_2BIT(11121110),
    B8_2BIT(11211111),B8_2BIT(11211110),
    B8_2BIT(11211111),B8_2BIT(11111110),
    B8_2BIT(11121111),B8_2BIT(11111110),
    B8_2BIT(01121111),B8_2BIT(11111100),
    B8_2BIT(01112211),B8_2BIT(11211100),
    B8_2BIT(00111122),B8_2BIT(22111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000),
    // Frame 3 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111221),B8_2BIT(11111000),
    B8_2BIT(01112111),B8_2BIT(11111100),
    B8_2BIT(01121111),B8_2BIT(11111100),
    B8_2BIT(11211111),B8_2BIT(12221110),
    B8_2BIT(11211111),B8_2BIT(11111110),
    B8_2BIT(11211111),B8_2BIT(11111110),
    B8_2BIT(11211111),B8_2BIT(11111110),
    B8_2BIT(11211111),B8_2BIT(12221110),
    B8_2BIT(01121111),B8_2BIT(11111100),
    B8_2BIT(01112111),B8_2BIT(11111100),
    B8_2BIT(00111221),B8_2BIT(11111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000),
    // Frame 4 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111122),B8_2BIT(22111000),
    B8_2BIT(01112211),B8_2BIT(11211100),
    B8_2BIT(01121111),B8_2BIT(11111100),
    B8_2BIT(11121111),B8_2BIT(11111110),
    B8_2BIT(11211111),B8_2BIT(11111110),
    B8_2BIT(11211111),B8_2BIT(11211110),
    B8_2BIT(11211111),B8_2BIT(11121110),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(01121112),B8_2BIT(11111100),
    B8_2BIT(01111111),B8_2BIT(21111100),
    B8_2BIT(00111111),B8_2BIT(12111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000),
    // Frame 5 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111222),B8_2BIT(22111000),
    B8_2BIT(01112111),B8_2BIT(11211100),
    B8_2BIT(01121111),B8_2BIT(11121100),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(11111111),B8_2BIT(11111110),
    B8_2BIT(11111111),B8_2BIT(11111110),
    B8_2BIT(11111211),B8_2BIT(12111110),
    B8_2BIT(01111211),B8_2BIT(12111100),
    B8_2BIT(01111211),B8_2BIT(12111100),
    B8_2BIT(00111111),B8_2BIT(11111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000),
    // Frame 6 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111222),B8_2BIT(21111000),
    B8_2BIT(01112111),B8_2BIT(12211100),
    B8_2BIT(01111111),B8_2BIT(11121100),
    B8_2BIT(11111111),B8_2BIT(11121110),
    B8_2BIT(11111111),B8_2BIT(11112110),
    B8_2BIT(11112111),B8_2BIT(11112110),
    B8_2BIT(11121111),B8_2BIT(11112110),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(01111112),B8_2BIT(11121100),
    B8_2BIT(01111121),B8_2BIT(11111100),
    B8_2BIT(00111211),B8_2BIT(11111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000),
    // Frame 7 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111111),B8_2BIT(22111000),
    B8_2BIT(01111111),B8_2BIT(11211100),
    B8_2BIT(01111111),B8_2BIT(11121100),
    B8_2BIT(11122211),B8_2BIT(11112110),
    B8_2BIT(11111111),B8_2BIT(11112110),
    B8_2BIT(11111111),B8_2BIT(11112110),
    B8_2BIT(11111111),B8_2BIT(11112110),
    B8_2BIT(11122211),B8_2BIT(11112110),
    B8_2BIT(01111111),B8_2BIT(11121100),
    B8_2BIT(01111111),B8_2BIT(11211100),
    B8_2BIT(00111111),B8_2BIT(22111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000),
    // Frame 8 done
    B8_2BIT(00000111),B8_2BIT(11000000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00111211),B8_2BIT(11111000),
    B8_2BIT(01111121),B8_2BIT(11111100),
    B8_2BIT(01111112),B8_2BIT(11121100),
    B8_2BIT(11211111),B8_2BIT(11112110),
    B8_2BIT(11121111),B8_2BIT(11112110),
    B8_2BIT(11112111),B8_2BIT(11112110),
    B8_2BIT(11111111),B8_2BIT(11112110),
    B8_2BIT(11111111),B8_2BIT(11121110),
    B8_2BIT(01111111),B8_2BIT(11121100),
    B8_2BIT(01112111),B8_2BIT(12211100),
    B8_2BIT(00111222),B8_2BIT(21111000),
    B8_2BIT(00011111),B8_2BIT(11110000),
    B8_2BIT(00000111),B8_2BIT(11000000)};
    
  const uint8_t SpriteSmileyMask[30] = {
    B8_1BIT(00000111),B8_1BIT(11000000),
    B8_1BIT(00011111),B8_1BIT(11110000),
    B8_1BIT(00111111),B8_1BIT(11111000),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(11111111),B8_1BIT(11111110),
    B8_1BIT(00011111),B8_1BIT(11110000),
    B8_1BIT(00000111),B8_1BIT(11000000) };
  
    const struct CRGB SpriteSmileyCols[2] = { CRGB(255,255,0), CRGB(0,0,0) };

    cLEDSprites Sprites;
    cSprite sprSmiley;


    cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;


    public:

        Matrix():Sprites(&leds){
            this->setup();
        };

        void loop(Hintergrund hintergrund, Sprite sprite){

            EVERY_N_SECONDS(1) {
                seconds++;
            }

            switch (hintergrund){
                case rainbow_plasma:
                    plasma();
                    break;
                case rainbow_stripe:
                    stripedRainbow();
                    break;
            }

            if(seconds % 7 == 0 && secondsTrigger != seconds) {
                secondsTrigger = seconds;
                Sprites.RemoveAllSprites();
                //sprite = 0; //= ++sprite % 6;
                switch (sprite){
                    case none:
                        // nothing
                        break;
                    case smiley:
                        setupSmiley();
                        break;
                    // case 2:
                    //     // setupInvader();
                    //     // setupInvader3();
                    //     break;
                    // case 3:
                    //     // Do nothing
                    //     break;
                    // case 4:
                    //     // setupAlien();
                    //     break;
                    // case 5:
                    //     // Do nothing
                    //     break;
                }
            }

            Sprites.UpdateSprites();
            Sprites.RenderSprites();
            
            FastLED.show();
            delay(10);
        }

        void clear(){
            for (int16_t x=0; x<leds.Width(); x++)
            {
                for (int16_t y=0; y<leds.Height(); y++)
                {
                    leds(x, y) = CRGB::Black;
                }
            }
            FastLED.show();
            delay(10);
        }


    private:
        void setup() {

            PlasmaShift = (random8(0, 5) * 32) + 64;
            PlasmaTime = 0;
            plasmaLowHue = 100;
            plasmaHighHue = 150;
            plasmaBrightness = 200;

            FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());

        }

        void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
        {
            byte lineStartHue = startHue8;
            for( byte y = 0; y < leds.Height(); y++) {
                lineStartHue += yHueDelta8;
                byte pixelHue = lineStartHue;      
                for( byte x = 0; x < leds.Width(); x++) {
                    pixelHue += xHueDelta8;
                    leds(x, y)  = CHSV( pixelHue, 255, 255);
                }
            }
        }

        // Fill background with plasma
        void plasma() {    
            for (int16_t x=0; x<leds.Width(); x++)
            {
                for (int16_t y=0; y<leds.Height(); y++)
                {
                    int16_t r = sin16(PlasmaTime) / 256;
                    int16_t h = sin16(x * r * PLASMA_X_FACTOR + PlasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + PlasmaTime) + sin16(y * x * (cos16(-PlasmaTime) / 256) / 2);
                    leds(x, y) = CHSV((uint8_t)((h / 256) + 128), 255, plasmaBrightness);
                }
            }
            uint16_t OldPlasmaTime = PlasmaTime;
            PlasmaTime += PlasmaShift;
            if (OldPlasmaTime > PlasmaTime)
            PlasmaShift = (random8(0, 5) * 32) + 64;
            // FastLED.show();
            // delay(10);
        }

        // striped Rainbow background
        void stripedRainbow() {
            uint32_t ms = millis();
            int32_t yHueDelta32 = ((int32_t)cos16( ms * 27 ) * (350 / leds.Width()));
            int32_t xHueDelta32 = ((int32_t)cos16( ms * 39 ) * (310 / leds.Height()));
            DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
            // FastLED.show();
            // delay(10);
        }

        // Smiley
        void setupSmiley() {
            sprSmiley.Setup(15, 15, SpriteSmileyData, 8, _2BIT, SpriteSmileyCols, SpriteSmileyMask);
            sprSmiley.SetPosition(-18,9);
            // xRate und FrameRate gleich halten
            sprSmiley.SetFrame(0,15); 
            sprSmiley.SetMotion(1,15,0,0);
            Sprites.AddSprite(&sprSmiley);
        }
};
