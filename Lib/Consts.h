#ifndef TRACKINGSHOT_CONSTS_H
#define TRACKINGSHOT_CONSTS_H



struct Consts {
public:

	struct SCREEN {
	public:
		static const unsigned int WIDTH = 1200;
		static const unsigned int HEIGHT = 800;
	};

	struct PATHS {
	public:
		inline static const char* DEPTH_MAP_VERT_SHADER = "Shaders/depthMap.vert";
		inline static const char* DEPTH_MAP_FRAG_SHADER = "Shaders/depthMap.frag";
		inline static const char* DEPTH_MAP_VISUAL_VERT = "Shaders/depthMapVisual.vert";
		inline static const char* DEPTH_MAP_VISUAL_FRAG = "Shaders/depthMapVisual.frag";
		inline static const char* SHADOW_VERT_SHADER = "Shaders/shadowShader.vert";
		inline static const char* SHADOW_FRAG_SHADER = "Shaders/shadowShader.frag";

		inline static const char* TEXTURE_CRATE = "Art/Textures/woodenCrate.jpg";
		inline static const char* TEXTURE_BRICK = "Art/Textures/brickWall.jpg";
		inline static const char* TEXTURE_GRID = "Art/Textures/unityGrid.jpg";
		inline static const char* TEXTURE_WHITE = "Art/Textures/white.jpg";

		inline static const char* NORMAL_MAP_EMPTY = "Art/Textures/emptyNormalMap.jpg";
		inline static const char* NORMAL_MAP_BRICK = "Art/Textures/brickWall_normal.jpg";
	};

	struct RENDERING {
	public:
		static const bool USE_WIREFRAME_MODE = false;
		static const unsigned int SHADOW_WITH = 2048; 
		static const unsigned int SHADOW_HEIGHT = 2048;
	};
};


#endif //TRACKINGSHOT_CONSTS_H
