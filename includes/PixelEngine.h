#pragma once
#include "../shaders/shader.h"

#include "../includes/glm/glm.hpp"
#include "../includes/glm/gtc/matrix_transform.hpp"
#include "../includes/glm/gtc/type_ptr.hpp"

typedef struct
{
	float x;
	float y;
} PxCoord;

class PixelArray
{
private:
	int ROWS;
	int COLS;
	int SIZE;
	float* pixelCanvas;

public:

	PixelArray(int ROWS, int COLS);

	~PixelArray();

	/* returns number of rows (pixels per col) */
	int getROWS();

	/* returns number of cols (pixels per row) */
	int getCOLS();

	/* returns an array of pixels to be passed to the PixelCanvasObj */
	float* getCanvas();

	/* sets the desired color for the array element */
	void setPixel(int i, int j, float r, float g, float b);

	/* allows you to set a continuous line at 2 points */
	void setLine(int i, int j, GLfloat r, GLfloat g, GLfloat b, int end_i, int end_j, int width);

	void clear();
};


class PixelCanvas
{
private:
	float TextureOpacity;
	unsigned int VAO;
	unsigned int TEXTURE;
	int ROWS;
	int COLS;
	float scale;
	float WIDTH;
	float HEIGHT;
	glm::mat4 transform;
	PxCoord translatePos;
	PxCoord nullPos;
	Shader* shader;

	void normilizeNullCoords();
public:

	void initCanvas(glm::mat4x3 bgcolor, Shader* shader);

	void initCanvas(glm::mat4x3 bgcolor, Shader* shader, float scale, float posX, float posY);

	PixelCanvas(int ROWS, int COLS, float WIDTH, float HEIGHT);

	~PixelCanvas();

	void changeBackground(glm::mat4x3 color);

	/* set the transparency of the texture, the higher, the more transparent, recomended value 0.2f */
	void setOpacity(float value);

	/* rendering this object */
	void render(float* canvas);

	void setScale(float scale);

	void increaseScale(float value);

	void setTranslate(float x, float y);

	void increaseTranslate(float moveX, float moveY);

	float getScale();

	float getWIDTH();

	float getHEIGHT();

	PxCoord PixelCanvas::getNullPos();

private:
	/* indexes for drawing a rectangular canvas from triangles*/
	GLuint indices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	/* the creation of the vertices of the canvas */
	void genCanvas(glm::mat4x3 color);

	/* creating all the buffers necessary for OpenGL */
	void genBuffers(GLfloat* vertices, GLuint sizeV, GLuint* indices, GLuint sizeI);

	/* configuring the texture buffer */
	void genTexture();

	/* sets an array of pixels as a texture */
	void setPixelTexture(float* canvas);
};


class PxObj : public PixelCanvas, public PixelArray
{
public:
	static int WindowSizeX;
	static int WindowSizeY;
	static bool MouseLeftClick;
	static bool MouseRightClick;
	int MousePosCol;
	int MousePosRow;
	int COLS;
	int ROWS;

	PxObj(int ROWS, int COLS, float WIDTH, float HEIGHT, int WindowSizeX, int WindowSizeY);

	PxObj(int ROWS, int COLS, float WIDTH, float HEIGHT);

	void draw();

	void TransformMouseXtoGrid(int x, int y);

};