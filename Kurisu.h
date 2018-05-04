#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <utility>


class Kurisu {
public:
	typedef struct {
		float x, y;
	} Vertex;
	typedef std::pair<Vertex, Vertex> Edge;

	class Pane_obj;
	typedef size_t Pane;

private:
	SDL_Window *window;
	SDL_GLContext glContext;

	std::vector<Pane_obj> pane_objs;

	void renderSetup ();
	void renderFinish ();

	static void moveVertexArray (const float *in, float *out, int count, float dx, float dy);

public:
	int createWindow (int w, int h);
	~Kurisu ();

	Pane addPane (Vertex center, GLfloat axesLineWidth, GLfloat notchLineWidth, GLfloat edgesLineWidth);
	inline Pane_obj& operator[] (Pane pane) {
		return pane_objs.at(pane);
	}
	bool update ();
	void render ();
};


class Kurisu::Pane_obj {
	friend class Kurisu;

	std::vector<Vertex> vertices;
	std::vector<float> edges;

	Vertex center;
	GLfloat axesLineWidth, notchLineWidth, edgesLineWidth;

	void render ();
	void drawAxes ();
	void drawEdges ();
	void drawVerticies ();

	// General
	static constexpr GLfloat scale = 0.2;
	const GLfloat invsc = 1 / scale;
	// Dots
	const GLfloat dotSizeHalf = 0.01f * invsc;
	const GLfloat dot[8] = {-dotSizeHalf, dotSizeHalf,
	                        -dotSizeHalf, -dotSizeHalf,
	                        dotSizeHalf, -dotSizeHalf,
	                        dotSizeHalf, dotSizeHalf};
	// Axis notches
	const GLfloat notchInterval = 0.8f;
	const int notchCountHalf = (int) (invsc / notchInterval);
	const GLfloat notchSize = 0.2f;
	const GLfloat H_notch[4] = {-notchSize / 2, 0, notchSize / 2, 0};
	const GLfloat V_notch[4] = {0, -notchSize / 2, 0, notchSize / 2};
	const GLfloat axis[8] = {-invsc, 0, invsc, 0,  // x
	                         0, -invsc, 0, invsc}; // y
	// Colours
	const GLfloat clAxis[3] = {0.0, 0.0, 0.0};
	const GLfloat clLVertex[3] = {0.0, 0.0, 1.0};
	const GLfloat clLEdges[3] = {1.0, 0.0, 0.0};

public:
	Pane_obj (Vertex center, GLfloat axesLineWidth, GLfloat notchLineWidth, GLfloat edgesLineWidth);
	void setGraph (const std::vector<Vertex> &vertices_in, const std::vector<Edge> &edges_in);
	~Pane_obj() = default;
};
