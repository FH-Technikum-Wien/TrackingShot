//
// Created by David on 10/5/2020.
//

#ifndef TRACKINGSHOT_CONSTS_H
#define TRACKINGSHOT_CONSTS_H



struct Consts {
public:

    struct PLAYER {
        inline static float MOVEMENT_SPEED = 1.0f;
        constexpr static const float MOUSE_SENSITIVITY = 0.001f;
    };

    struct SCREEN {
    public:
        static const unsigned int WIDTH = 1920;
        static const unsigned int HEIGHT = 1080;
    };

    struct PATHS {
    public:
        inline static const char *VERTEX_SHADER = "Shaders/shader.vert";
        inline static const char *FRAGMENT_SHADER = "Shaders/shader.frag";
        inline static const char *TEXTURE_CRATE = "art/textures/woodenCrate.jpg";
        inline static const char *TEXTURE_BRICK = "art/textures/brickWall.jpg";
        inline static const char *TEXTURE_GRID = "art/textures/texture_09.png";
        inline static const char *TEXTURE_GRID_TEXT = "art/textures/texture_13.png";
        inline static const char *TEXTURE_SKY = "art/textures/sky.jpg";
        inline static const char *TEXTURE_RED = "art/textures/red.png";
        inline static const char *TEXTURE_AWESOME = "art/textures/awesomeface.png";
    };

    struct RENDERING{
    public:
        static const bool USE_WIREFRAME_MODE = false;
    };

    struct PLAY_MODE{
        inline static bool FREE_FLY = true;
    };
};


#endif //TRACKINGSHOT_CONSTS_H
