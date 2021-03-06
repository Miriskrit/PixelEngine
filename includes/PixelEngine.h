#pragma once
#include "../shaders/shader.h"

#include "../includes/glm/glm.hpp"
#include "../includes/glm/gtc/matrix_transform.hpp"
#include "../includes/glm/gtc/type_ptr.hpp"

namespace Px
{
	class ComponentBase;

	typedef struct
	{
		float x;
		float y;
	} FloatCoord;

	extern bool MouseLeftClick;
	extern bool MouseRightClick;
	extern int WindowSizeX;
	extern int WindowSizeY;
	extern int MousePosX;
	extern int MousePosY;
	extern float ScrollX;
	extern float ScrollY;
	extern bool Keys[1024];
	extern float deltaTime;
	extern float lastFrame;


	class ComponentEvents
	{
	public:
		void setMoveSpeed(float value);

		virtual void eventProcessing(float deltaTime) {};

		float getMoveSpeed();

	protected:
		float moveSpeed;

		ComponentEvents();
	};


	class EventsDefaultManager
	{
	public:
		ComponentEvents** objects;
		int objectsCounter;

		EventsDefaultManager();
		~EventsDefaultManager();
		void updateEvents(float deltaTime);
		void appendObj(ComponentEvents& obj);
		void appendObjects(int n, ComponentEvents* objects[]);
	};

	class DrawManager
	{
	public:
		ComponentBase** objects;
		int objectsCounter;

		DrawManager();
		~DrawManager();
		void drawAll();
		void appendObj(ComponentBase& obj);
		void appendObjects(int n, ComponentBase* objects[]);
	};

	class VAOComponent
	{
	public:
		void setOpacity(float value);
		void setBaseColor(float r, float g, float b);
	protected:
		float TextureOpacity;
		unsigned int VAO;
		unsigned int TEXTURE;
		int indicesSize;
		Shader* shader;

		VAOComponent();
		~VAOComponent();
		virtual void drawVAO() = 0;
		void genBuffers(GLfloat* vertices, GLuint sizeV, GLuint* indices, GLuint sizeI);
		void genTexture();
		void setTexture(float* pixels, int ROWS, int COLS);
	};

	class VAOrectangle : public VAOComponent
	{
	protected:
		GLuint indices2triangles[6] = {
			0, 1, 3,
			1, 2, 3
		};

		GLuint indices4triangles[12] = {
			0, 4, 3,
			0, 4, 1,
			3, 4, 2,
			1, 4, 2,
		};

		void genVAO(glm::mat4x3 color, float WIDTH, float HEIGHT, int indicesSize);
		void genVAO(glm::mat2x3 color, float WIDTH, float HEIGHT, int indicesSize);
		void drawVAO() override;
	};

	class ComponentBase
	{
	public:
		float getWIDTH();
		float getHEIGHT();
		FloatCoord getSIZE();
		virtual void draw() {};

	protected:
		float WIDTH;
		float HEIGHT;

		ComponentBase(float WIDTH, float HEIGHT);
		ComponentBase();
	};


	class ComponentMovable : public ComponentBase
	{
	public:
		void setScale(float scale);
		float getScale();
		void increaseScale(float value);
		void setTranslate(float x, float y);
		void increaseTranslate(float moveX, float moveY);
		FloatCoord getNullPos();
		FloatCoord getRealPos();
		FloatCoord getScaledSIZE();
		FloatCoord getTranslatePos();
		bool IsLocatedOnObject(int x, int y);
		bool ComponentMovable::isRectCollisionWith(ComponentMovable& obj);
		bool ComponentMovable::isRoundCollisionWith(ComponentMovable& obj);

	protected:
		float scale;
		FloatCoord translatePos;
		FloatCoord nullPos;
		FloatCoord absoluteMousePos;
		FloatCoord realPos;
		glm::mat4 transform;

		ComponentMovable(float WIDTH, float HEIGHT);
		void normilizeNullCoords();
		void updateRealPos();
	};

	class AbstractRectangle : public VAOrectangle, public ComponentMovable
	{
	public:
		void changeBackground(glm::mat4x3 color);
		void changeBackground(glm::mat2x3 color);
		void draw() override;
	protected:
		AbstractRectangle(float WIDTH, float HEIGHT, glm::mat4x3 bgcolor, Shader* shader, float scale, float posX, float posY);
		AbstractRectangle(float WIDTH, float HEIGHT, glm::mat2x3 bgcolor, Shader* shader, float scale, float posX, float posY);

	private:
		AbstractRectangle(float WIDTH, float HEIGHT, Shader* shader, float scale, float posX, float posY);
	};
	

	class PxRectangle : public AbstractRectangle, public ComponentEvents
	{
	public:
		PxRectangle(float WIDTH, float HEIGHT, glm::mat4x3 bgcolor, Shader* shader, float scale, float posX, float posY,
			void (*event_click)(PxRectangle& self), void (*event_scrool)(PxRectangle& self), void (*event_keyboard)(PxRectangle& self, float deltaTime));
		PxRectangle(float WIDTH, float HEIGHT, glm::mat2x3 bgcolor, Shader* shader, float scale, float posX, float posY,
			void (*event_click)(PxRectangle& self), void (*event_scrool)(PxRectangle& self), void (*event_keyboard)(PxRectangle& self, float deltaTime));
		void eventProcessing(float deltaTime) override;
	private:
		void init(void (*event_click)(PxRectangle& self), void (*event_scrool)(PxRectangle& self), void (*event_keyboard)(PxRectangle& self, float deltaTime));

		void (*event_click)(PxRectangle& self);
		void (*event_scrool)(PxRectangle& self);
		void (*event_keyboard)(PxRectangle& self, float deltaTime);
	};


	class PxCanvas : public AbstractRectangle, public ComponentEvents
	{
	public:
		PxCanvas(int ROWS, int COLS, float WIDTH, float HEIGHT, glm::mat4x3 bgcolor, Shader* shader, float scale, float posX, float posY,
			void (*event_click)(PxCanvas& self), void (*event_scrool)(PxCanvas& self), void (*event_keyboard)(PxCanvas& self, float deltaTime));
		PxCanvas(int ROWS, int COLS, float WIDTH, float HEIGHT, glm::mat2x3 bgcolor, Shader* shader, float scale, float posX, float posY,
			void (*event_click)(PxCanvas& self), void (*event_scrool)(PxCanvas& self), void (*event_keyboard)(PxCanvas& self, float deltaTime));
		~PxCanvas();
		void setPixel(int i, int j, float r, float g, float b);
		void setPixel(float r, float g, float b);
		void setLine(int i, int j, GLfloat r, GLfloat g, GLfloat b, int end_i, int end_j, int width);
		void clear();
		void draw() override;
		void eventProcessing(float deltaTime) override;
		int getROWS();
		int getCOLS();

		int getMouseCol();
		int getMouseRow();

	private:
		int ROWS;
		int COLS;
		int SIZE;
		float* pixelCanvas;
		int MousePosCol;
		int MousePosRow;

		void (*event_click)(PxCanvas& self);
		void (*event_scrool)(PxCanvas& self);
		void (*event_keyboard)(PxCanvas& self, float deltaTime);

		void init(int ROWS, int COLS, void (*event_click)(PxCanvas& self), void (*event_scrool)(PxCanvas& self), void (*event_keyboard)(PxCanvas& self, float deltaTime));
		void setPixelTexture();
	};


	class PxStaticBackground : public ComponentBase, public VAOrectangle
	{
	public:
		PxStaticBackground(glm::mat4x3 color, Shader* shader);
		PxStaticBackground(glm::mat2x3 color, Shader* shader);
		void changeBackground(glm::mat4x3 color);
		void changeBackground(glm::mat2x3 color);
		void draw() override;
	};


	template <typename T>
	class PxButtonForObj : public AbstractRectangle, public ComponentEvents
	{
	public:
		PxButtonForObj(float WIDTH, float HEIGHT, glm::mat4x3 bgcolor, Shader* shader, float scale, float posX, float posY, void (*btncallback)(T& obj), T& obj);
		PxButtonForObj(float WIDTH, float HEIGHT, glm::mat2x3 bgcolor, Shader* shader, float scale, float posX, float posY, void (*btncallback)(T& obj), T& obj);
		void eventProcessing(float deltaTime) override;

	private:
		void init(void (*btncallback)(T& obj), T& obj);
		void (*btncallback)(T& obj);
		T* bindObj;
	};

#include "../includes/PxButtonForObj.ini";

	class PxButton : public AbstractRectangle, public ComponentEvents
	{
	public:
		PxButton(float WIDTH, float HEIGHT, glm::mat4x3 bgcolor, Shader* shader, float scale, float posX, float posY, void (*btncallback)());
		PxButton(float WIDTH, float HEIGHT, glm::mat2x3 bgcolor, Shader* shader, float scale, float posX, float posY, void (*btncallback)());
		void eventProcessing(float deltaTime) override;

	private:
		void init(void (*btncallback)());
		void (*btncallback)();
	};
}