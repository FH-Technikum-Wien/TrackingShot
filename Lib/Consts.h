#ifndef TRACKINGSHOT_CONSTS_H
#define TRACKINGSHOT_CONSTS_H



struct Consts {
public:

	struct SCREEN {
	public:
		static const unsigned int WIDTH = 2560;
		static const unsigned int HEIGHT = 1440;
	};

	struct PATHS {
	public:
		inline static const char* DEPTH_MAP_VERT_SHADER = "Shaders/depthMap.vert";
		inline static const char* DEPTH_MAP_FRAG_SHADER = "Shaders/depthMap.frag";
		inline static const char* DEPTH_MAP_VISUAL_VERT = "Shaders/depthMapVisual.vert";
		inline static const char* DEPTH_MAP_VISUAL_FRAG = "Shaders/depthMapVisual.frag";
		inline static const char* SHADOW_VERT_SHADER = "Shaders/shadowShader.vert";
		inline static const char* SHADOW_FRAG_SHADER = "Shaders/shadowShader.frag";

		inline static const char* TEXTURE_GRID = "Art/Textures/unityGrid.jpg";

		inline static const char* TEXTURE_BRICK = "Art/Textures/brickWall.jpg";
		inline static const char* TEXTURE_WOOD = "Art/Textures/wood.jpg";
		inline static const char* TEXTURE_ROCKS = "Art/Textures/rocks.jpg";
		inline static const char* TEXTURE_BRICK2 = "Art/Textures/bricks2.jpg";
		inline static const char* TEXTURE_WHITE = "Art/Textures/white.jpg";

		inline static const char* NORMAL_MAP_EMPTY = "Art/Textures/emptyNormalMap.jpg";
		inline static const char* NORMAL_MAP_BRICK = "Art/Textures/brickWall_normal.jpg";
		inline static const char* NORMAL_MAP_WOOD = "Art/Textures/wood_normal.jpg";
		inline static const char* NORMAL_MAP_ROCKS = "Art/Textures/rocks_normal.jpg";
		inline static const char* NORMAL_MAP_BRICK2 = "Art/Textures/bricks2_normal.jpg";
	};

	struct RENDERING {
	public:
		static const bool USE_WIREFRAME_MODE = false;
		static const unsigned int SHADOW_WITH = 4096; 
		static const unsigned int SHADOW_HEIGHT = 4096;
	};
};


#endif //TRACKINGSHOT_CONSTS_H
