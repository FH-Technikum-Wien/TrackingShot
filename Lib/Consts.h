#ifndef TRACKINGSHOT_CONSTS_H
#define TRACKINGSHOT_CONSTS_H



struct Consts {
public:

    struct SCREEN {
    public:
        static const unsigned int WIDTH = 1920;
        static const unsigned int HEIGHT = 1080;
    };

    struct PATHS {
    public:
        inline static const char *VERTEX_SHADER = "Shaders/shader.vert";
        inline static const char *FRAGMENT_SHADER = "Shaders/shader.frag";
        inline static const char *TEXTURE_CRATE = "Art/Textures/woodenCrate.jpg";
        inline static const char *TEXTURE_BRICK = "Art/Textures/brickWall.jpg";
        inline static const char *TEXTURE_GRID = "Art/Textures/texture_09.png";
        inline static const char *TEXTURE_GRID_TEXT = "Art/Textures/texture_13.png";
        inline static const char *TEXTURE_SKY = "Art/Textures/sky.jpg";
        inline static const char *TEXTURE_RED = "Art/Textures/red.png";
        inline static const char *TEXTURE_AWESOME = "Art/Textures/awesomeface.png";
    };

    struct RENDERING{
    public:
        static const bool USE_WIREFRAME_MODE = false;
    };
};


#endif //TRACKINGSHOT_CONSTS_H
