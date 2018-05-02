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

private:
	SDL_Window *window;
	SDL_GLContext glContext;

	class Pane;

	std::vector<Vertex> LVertex, RVertex;
	std::vector<float> LEdgesPlain, REdgesPlain;

	void renderSetup ();
	void drawAxes ();
	void drawVertex ();
	void drawEdges ();
	void renderFinish ();

	//@formatter:off
	// General
	const GLfloat scale            = 0.2;
	const GLfloat invsc            = 1 / scale;
	const Vertex LPane_center     = {-invsc,  0},
	              RPane_center     = { invsc,  0};
	const GLfloat separatorLine[4] = { 0, -invsc,  0,  invsc};
	const GLfloat axis[8]          = {-invsc, 0,  invsc, 0,  // x
	                                  0, -invsc, 0,  invsc}; // y
	// Dots
	const GLfloat dotSizeHalf = 0.01f * invsc;
	const GLfloat dot[8]      = {-dotSizeHalf,  dotSizeHalf,
							     -dotSizeHalf, -dotSizeHalf,
	                              dotSizeHalf, -dotSizeHalf,
	                              dotSizeHalf,  dotSizeHalf};
	// Axis notches
	const GLfloat notchInterval = 0.8f;
	const int notchCountHalf    = (int) (invsc / notchInterval);
	const GLfloat notchSize     = 0.2f;
	const GLfloat H_notch[4]    = {-notchSize / 2, 0, notchSize / 2, 0};
	const GLfloat V_notch[4]    = {0, -notchSize / 2, 0, notchSize / 2};
	// Thiccness
	const GLfloat separatorLineWidth = 2;
	const GLfloat axisLineWidth      = 1;
	const GLfloat notchLineWidth     = 1;
	const GLfloat edgeLineWidth      = 1;
	// Colours
	const GLfloat clAxis[3]    = {0.0, 0.0, 0.0};
	const GLfloat clLVertex[3] = {0.0, 0.0, 1.0};
	const GLfloat clRVertex[3] = {0.0, 0.0, 1.0};
	const GLfloat clLEdges[3]  = {1.0, 0.0, 0.0};
	const GLfloat clREdges[3]  = {0.0, 0.6, 0.0};
	//@formatter:on

	void moveVertexArray (const float *in, float *out, int count, float dx, float dy);

public:
	enum class pane_id {
		Left, Right
	};

	int createWindow (int w, int h);
	~Kurisu ();

	void set (pane_id pane, const std::vector<Vertex> &vertex_in, const std::vector<Edge> &edges_in);
	bool update ();
	void render ();
};


class Kurisu::Pane {

};
