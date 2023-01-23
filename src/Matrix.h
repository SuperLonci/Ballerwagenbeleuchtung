using namespace tgx;

#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B
#define MATRIX_WIDTH   16
#define MATRIX_HEIGHT  32
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX
#define FRAMERATE      30.0f

const int LOADED_SHADERS = TGX_SHADER_PERSPECTIVE | TGX_SHADER_ZBUFFER | TGX_SHADER_FLAT | TGX_SHADER_NOTEXTURE;

uint pcg_hash(uint input)
{
    uint state = input * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

class MatrixScene {
    public:
        Renderer3D<RGB24, LOADED_SHADERS, uint16_t>* renderer;
        Image<RGB24>* framebuffer;

        virtual void setup();
        virtual void render();
};

class SynthwaveScene : public MatrixScene {
    public:
        void setup() {
            renderer->setPerspective(60, ((float)MATRIX_WIDTH) / MATRIX_HEIGHT, 1.0f, 100.0f);
            fMat4 view;
            //view.setLookAt({ 0, -4, 5 }, { 0, -4, 0 }, { 0, 1, 0 });
            view.setLookAt({ -3, -3, 0 }, { 0, -2.0, 0 }, { 0, 1, 0 });
            renderer->setViewMatrix(view);
            renderer->setMaterial(RGBf(1.0f, 1.0f, 1.0f), 1.0f, 0.7f, 0.8f, 64);
            renderer->setCulling(0);
            FastLED.setDither(0);
        }

        void render() {
            framebuffer->fillScreen(RGB24(0.0f, 0.0f, 0.0f));
            renderer->clearZbuffer();

            framebuffer->fillCircle({ 7, 22}, 6, RGB24(0.3f, 0.1f, 0.0f), RGB24(0.3f, 0.1f, 0.0f));
            framebuffer->fillCircle({ 8, 22}, 6, RGB24(0.3f, 0.1f, 0.0f), RGB24(0.3f, 0.1f, 0.0f));
            framebuffer->fillCircle({ 7, 22}, 5, RGB24(1.0f, 0.6f, 0.0f), RGB24(1.0f, 0.6f, 0.0f));
            framebuffer->fillCircle({ 8, 22}, 5, RGB24(1.0f, 0.6f, 0.0f), RGB24(1.0f, 0.6f, 0.0f));
            framebuffer->fillRect({ 0, 0 }, { 16, 17 }, RGB24(0.0f, 0.0f, 0.0f), RGB24(0.0f, 0.0f, 0.0f));

            #define gridSizeX 7
            #define gridSizeZ 8
            float cellSize = 1.5f;
            static fVec3 vertices[gridSizeX * gridSizeZ];
            for (int x = 0; x < gridSizeX; x++) {
                for (int z = 0; z < gridSizeZ; z++) {
                    vertices[x * gridSizeZ + z] = fVec3(x * cellSize, 0.0f, z * cellSize - ((gridSizeZ-1) * cellSize) / 2.0f);
                }
            }
            static uint16_t indices[(gridSizeX - 1) * (gridSizeZ - 1) * 4];
            for (int x = 0; x < gridSizeX - 1; x++) {
                for (int z = 0; z < gridSizeZ - 1; z++) {
                    indices[(x * (gridSizeZ - 1) + z) * 4 + 0] = x * gridSizeZ + z;
                    indices[(x * (gridSizeZ - 1) + z) * 4 + 1] = x * gridSizeZ + z + 1;
                    indices[(x * (gridSizeZ - 1) + z) * 4 + 2] = (x + 1) * gridSizeZ + z + 1;
                    indices[(x * (gridSizeZ - 1) + z) * 4 + 3] = (x + 1) * gridSizeZ + z;
                }
            }

            long time = millis();
            float pos = (time % 500) / 500.0f;
            fMat4 mat;
            mat.setTranslate({ -pos * cellSize, 0, 0 });
            renderer->setModelMatrix(mat);
            renderer->setShaders(TGX_SHADER_FLAT);
            renderer->setMaterialColor(RGBf(1.0f, 0.0f, 1.0f));
            renderer->drawWireFrameQuads((gridSizeX-1) * (gridSizeZ-1), indices, vertices, 0.5f);
        }
};

class PortalScene: public MatrixScene {
    public:
        void setup() {
            renderer->setPerspective(90, ((float)MATRIX_WIDTH) / MATRIX_HEIGHT, 1.0f, 100.0f); 
            fMat4 view;
            view.setLookAt({ 0, 0, 5 }, { 0, 0, 0 }, { 0, 1, 0 });
            renderer->setViewMatrix(view);
            renderer->setMaterial(RGBf(1.0f, 1.0f, 1.0f), 1.0f, 0.7f, 0.8f, 64);
            renderer->setCulling(0);
        }

        void render() {
            framebuffer->fillScreen(RGB24(0.0f, 0.0f, 0.0f));
            renderer->clearZbuffer();

            long time = millis();
            int duration = 1000;
            int period = time / duration;
            int pos = time % duration;
            uint hash = pcg_hash(period);
            uint hashNext = pcg_hash(period+1);

            float top = -3.0f;
            float bottom = 8.0f;
            float y = top + (bottom - top) * ((float)pos / (float)duration);

            HSV color = HSV(hash / 4294967296.0f, 1.0f, 1.0f);
            HSV colorNext = HSV(hashNext / 4294967296.0f, 1.0f, 1.0f);

            framebuffer->fillEllipse({7, 1}, {6, 2}, RGB24(0.2f, 0.05f, 0.0f), RGB24(1.0f, 0.6f, 0.0f));
            framebuffer->fillEllipse({8, 1}, {6, 2}, RGB24(0.2f, 0.05f, 0.0f), RGB24(1.0f, 0.6f, 0.0f));
            framebuffer->fillEllipse({7, 30}, {6, 2}, RGB24(0.0f, 0.05f, 0.2f), RGB24(0.0f, 0.6f, 1.0f));
            framebuffer->fillEllipse({8, 30}, {6, 2}, RGB24(0.0f, 0.05f, 0.2f), RGB24(0.0f, 0.6f, 1.0f));

            fMat4 mat;
            mat.setScale(1.0f, 1.0f, 1.0f);
            mat.multRotate(time / 10.0f, {0, 1, 0});
            mat.multTranslate({0, y, 0});
            renderer->setModelMatrix(mat);
            renderer->setShaders(TGX_SHADER_FLAT);
            renderer->setMaterialColor(RGBf(color));
            renderer->drawWireFrameCube(0.5f);
            
            mat.setScale(1.0f, 1.0f, 1.0f);
            mat.multRotate(time / 10.0f, {0, 1, 0});
            mat.multTranslate({0, y - (bottom - top), 0});
            renderer->setModelMatrix(mat);
            renderer->setMaterialColor(RGBf(colorNext));
            renderer->drawWireFrameCube(0.5f);
        }
};

struct Ball {
    fVec2 pos;
    fVec2 vel;
    HSV color;
};

class GravityBouncyBallScene: public MatrixScene {

#define BALL_COUNT 15

    public:
        Ball balls[BALL_COUNT];

        void setup() {
            for (int i = 0; i < BALL_COUNT; i++) {
                balls[i].pos = { random(0, MATRIX_WIDTH), random(5, MATRIX_HEIGHT) };
                balls[i].vel = { random(-100, 100) / 100.0f, random(-100, 100) / 100.0f };
                balls[i].vel.normalize();
                balls[i].vel *= 20.0f;
                balls[i].color = HSV(random(0, 1000) / 1000.0f, 1.0f, 1.0f);
            }
        }

        void render() {
            framebuffer->fillScreen(RGB24(0.0f, 0.0f, 0.0f));

            // update ball positions
            for (int i = 0; i < BALL_COUNT; i++) {
                balls[i].pos += balls[i].vel * FRAMERATE / 1000.0f;
                if (balls[i].pos.x < 0) {
                    balls[i].pos.x *= -1;
                    balls[i].vel.x = -balls[i].vel.x;
                }
                if (balls[i].pos.x >= MATRIX_WIDTH) {
                    balls[i].pos.x = MATRIX_WIDTH - 1;
                    balls[i].vel.x = -balls[i].vel.x;
                }
                if (balls[i].pos.y < 0) {
                    balls[i].pos.y *= -1;
                    balls[i].vel.y = -balls[i].vel.y;
                }
                if (balls[i].pos.y >= MATRIX_HEIGHT) {
                    balls[i].pos.y = MATRIX_HEIGHT - 1;
                    balls[i].vel.y = -balls[i].vel.y;
                }
            }

            // add gravity force
            for (int i = 0; i < BALL_COUNT; i++) {
                balls[i].vel.y -= 200.0f * FRAMERATE / 1000.0f;
            }
            
            // draw all balls
            for (int i = 0; i < BALL_COUNT; i++) {
                framebuffer->fillCircle({balls[i].pos.x, balls[i].pos.y}, 1, RGB24(balls[i].color), RGB24(balls[i].color));
            }
        }
};

class BigBouncyBallScene: public MatrixScene {

#define BALL_COUNT 8

    public:
        Ball balls[BALL_COUNT];

        void setup() {
            for (int i = 0; i < BALL_COUNT; i++) {
                balls[i].pos = { random(0, MATRIX_WIDTH), random(0, MATRIX_HEIGHT) };
                balls[i].vel = { random(-100, 100) / 100.0f, random(-100, 100) / 100.0f };
                balls[i].vel.normalize();
                balls[i].vel *= 35.0f;
                //assign hsv color based on index
                balls[i].color = HSV(i / (float)BALL_COUNT, 1.0f, 1.0f);
            }
        }

        void render() {
            framebuffer->fillScreen(RGB24(0.0f, 0.0f, 0.0f));

            // let balls collide with each other without getting stuck
            for (int i = 0; i < BALL_COUNT; i++) {
                for (int j = i + 1; j < BALL_COUNT; j++) {
                    fVec2 diff = balls[i].pos - balls[j].pos;
                    float dist = diff.norm();
                    if (dist < 5.0f) {
                        fVec2 normal = diff;
                        normal.normalize();
                        fVec2 velDiff = balls[i].vel - balls[j].vel;
                        float velDiffNormal = dotProduct(velDiff, normal);
                        if (velDiffNormal < 0.0f) {
                            balls[i].vel -= normal * velDiffNormal;
                            balls[j].vel += normal * velDiffNormal;
                        }
                    }
                }
            }

            // animate ball colors cycling through hsv hues
            for (int i = 0; i < BALL_COUNT; i++) {
                balls[i].color.H += 0.01f;
                if (balls[i].color.H > 1.0f) {
                    balls[i].color.H -= 1.0f;
                }
            }

            // update ball positions
            for (int i = 0; i < BALL_COUNT; i++) {
                balls[i].pos += balls[i].vel * FRAMERATE / 1000.0f;
                if (balls[i].pos.x < 0) {
                    balls[i].pos.x = 0;
                    balls[i].vel.x = -balls[i].vel.x;
                }
                if (balls[i].pos.x >= MATRIX_WIDTH) {
                    balls[i].pos.x = MATRIX_WIDTH - 1;
                    balls[i].vel.x = -balls[i].vel.x;
                }
                if (balls[i].pos.y < 0) {
                    balls[i].pos.y = 0;
                    balls[i].vel.y = -balls[i].vel.y;
                }
                if (balls[i].pos.y >= MATRIX_HEIGHT) {
                    balls[i].pos.y = MATRIX_HEIGHT - 1;
                    balls[i].vel.y = -balls[i].vel.y;
                }
            }
            
            // draw all balls
            for (int i = 0; i < BALL_COUNT; i++) {
                framebuffer->fillCircle({balls[i].pos.x, balls[i].pos.y}, 2, RGB24(balls[i].color), RGB24(balls[i].color));
            }
        }
};


class SandScene: public MatrixScene {
    public:
        void setup() {
            
        }

        void render() {
            framebuffer->fillScreen(RGB24(0.0f, 0.0f, 0.0f));
            
            long time = millis();
        }
};


template<uint8_t LED_PIN>
class Matrix {
    // Framebuffer
    cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;
    RGB24* fb;
    Image<RGB24> imfb;
    // Z-Buffer
    uint16_t* zbuffer;

    CLEDController* controller;
    Renderer3D<RGB24, LOADED_SHADERS, uint16_t> renderer;

    MatrixScene* currentScene;

    public:

        Matrix() {
            this->setup();
        };

        void loop(){
            currentScene->render();
            fixAlignment();
            controller->showLeds(1);
            delay(1000/FRAMERATE);
        }

        void fixAlignment() {
            for (int x = 0; x < MATRIX_WIDTH; x++) {
                for (int y = 0; y < MATRIX_HEIGHT; y++) {
                    leds(x, y) = *reinterpret_cast<CRGB*>(&imfb(x, y));
                }
            }
        }

        void clear(){
            for (int16_t x=0; x<leds.Width(); x++)
            {
                for (int16_t y=0; y<leds.Height(); y++)
                {
                    leds(x, y) = CRGB::Black;
                }
            }
            controller->showLeds(5);
            delay(10);
        }


    private:
        void setup() {
            controller = &FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
            zbuffer = (uint16_t*)malloc(MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(uint16_t));
            fb = (RGB24*)malloc(MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(RGB24));
            imfb = Image<RGB24>(fb, MATRIX_WIDTH, MATRIX_HEIGHT);

            //FastLED.setDither(0);

            // setup the 3D renderer.
            renderer.setViewportSize(MATRIX_WIDTH, MATRIX_HEIGHT);
            renderer.setOffset(0, 0);    
            renderer.setImage(&imfb); // set the image to draw onto (ie the screen framebuffer)
            renderer.setZbuffer(zbuffer); // set the z buffer for depth testing

            currentScene = new SynthwaveScene();
            currentScene->renderer = &renderer;
            currentScene->framebuffer = &imfb;
            currentScene->setup();
        }
};