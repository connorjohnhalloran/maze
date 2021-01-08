/*
 * template.c
 *
 * An OpenGL source code template.
 */


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../mylib/initShader.h"
#include "../mylib/linear_alg.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ------------------------------ MAZE BASE CODE START ------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// stuff to make working with booleans easier
typedef int bool;
#define true 1
#define false 0

// main struct that makes up the maze
typedef struct {
	// bools that tell whether each cell has a wall or not
	bool north_has_wall;
	bool south_has_wall;
	bool east_has_wall;
	bool west_has_wall;

	// bool that determines whether each cell has been visited during creation of maze
	bool visited;
	
} cell;

// 2d array that contains the maze itself
cell maze[8][8];

// maze printing function for debugging and display
void print_maze() {
	for (int i = 0; i < 8; ++i) {
		// first row
		for (int j = 0; j < 8; ++j) {
			printf("+");
			if (maze[i][j].north_has_wall) {
				printf("+++");
			} else {
				printf("   ");
			}
			printf("+");
		}
		printf("\n");
		// second row
		for (int j = 0; j < 8; ++j) {
			if (maze[i][j].west_has_wall) {
				printf("|");
			} else {
				printf(" ");
			}
			printf("   ");
			if (maze[i][j].east_has_wall) {
				printf("|");
			} else {
				printf(" ");
			}
		}
		printf("\n");
		// first row
		for (int j = 0; j < 8; ++j) {
			printf("+");
			if (maze[i][j].south_has_wall) {
				printf("+++");
			} else {
				printf("   ");
			}
			printf("+");
		}
		printf("\n");
	}
}

// set every cell up for maze generation
void initialize_maze() {
	// Set each cell to have walls on each side and mark as unvisited
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			maze[i][j].north_has_wall = true;
			maze[i][j].south_has_wall = true;
			maze[i][j].east_has_wall = true;
			maze[i][j].west_has_wall = true;
			maze[i][j].visited = false;
		}
	}
}

// defines to make code read more cleanly
// also used when solving the maze and determining direction
#define north 1
#define east 2
#define south 3
#define west 4

// recursive maze generation code
void generate_maze(int row, int col, int incoming_direction) {

	// mark this cell as visited
	maze[row][col].visited = true;

	// remove wall of this cell from direction where it came from
	switch (incoming_direction) {
		case north:
			maze[row][col].south_has_wall = false; break;
		case south:
			maze[row][col].north_has_wall = false; break;
		case east:
			maze[row][col].west_has_wall =  false; break;
		case west:
			maze[row][col].east_has_wall =  false; break;
	}

	// bools used for reading clarity
	bool north_visited;
	bool south_visited;
	bool east_visited;
	bool west_visited;
	
	// loop that is exited when all sides are marked as visited
	while (true) {
		// determine whether neighboring cells are valid movement spots,
		// including checking for hitting the edge of the maze
		south_visited = (row == 7) ? true : maze[row+1][col].visited;
		north_visited = (row == 0) ? true : maze[row-1][col].visited;
		east_visited  = (col == 7) ? true : maze[row][col+1].visited;
		west_visited  = (col == 0) ? true : maze[row][col-1].visited;

		// when all sides relative to cell are visited, go back one in the recursive chain
		if (north_visited && south_visited && east_visited && west_visited) { return; }

		// use random number generator to determine which direction to go
		// 1 = north, 2 = south, 3 = east, 4 = west
		int direction = (rand() % 4) + 1;

		// determine which cell to go to and update information for this cell and function call
		if (direction == north && !north_visited) {
			maze[row][col].north_has_wall = false;
			north_visited = true;
			generate_maze(row-1, col, north);
		} else if (direction == south && !south_visited) {
			maze[row][col].south_has_wall = false;
			south_visited = true;
			generate_maze(row+1, col, south);
		} else if (direction == east && !east_visited) {
			maze[row][col].east_has_wall = false;
			east_visited = true;
			generate_maze(row, col+1, east);
		} else if (direction == west && !west_visited) {
			maze[row][col].west_has_wall = false;
			west_visited = true;
			generate_maze(row, col-1, west);
		}
	}
}

// helper function to kick off recursive maze generation
void start_maze_generation() {
	srand(time(0)); // randomize seed
	initialize_maze();
	generate_maze(0, 0, south); // starting point for recursion
	maze[7][7].south_has_wall = false; // add exit point at bottom right of maze when program is done running
}

// switch between solid color and texture sampling, needed for blue line
int use_texture;
GLuint use_texture_location;

// ------------------------------------
// ----------- MAZE SOLVING -----------
// ------------------------------------

// doubly linked list used to keep track of movements for backtracking and
// easy removal of duplicate steps for finding shortest path
struct node {
	struct node* next;
	struct node* prev;
	int row;
	int col;
	int orientation;
};

struct node* head;

void solve_maze() {
	
	// starting data for entrance at the north west corner of maze going down
	int row = 0;
	int col = 0;
	int orientation = south;

	// define head of linked list
	head = malloc(sizeof(struct node));
	head->next = NULL;
	head->prev = NULL;
	head->row = row;
	head->col = col;
	head->orientation = orientation; // IMPORTANT: Notates the orientation of the camera as it enters the cell

	// reference to most recent node
	struct node* current_node = head;

	// generate raw list of directions to exit of maze
	while (row != 7 || col != 7) {

		// set orientation to look left relative to current position
		orientation = (orientation + 6) % 4 + 1;

		// find next valid position, starting with previously mentioned left turn then turning right repeatedly
		while (true) {
			if (orientation == north) {
				if (maze[row][col].north_has_wall) { orientation = orientation % 4 + 1; }
				else { row -= 1; break; }
			}
			else if (orientation == east) {
				if (maze[row][col].east_has_wall) { orientation = orientation % 4 + 1; }
				else { col += 1; break; }
			}
			else if (orientation == south) {
				if (maze[row][col].south_has_wall) { orientation = orientation % 4 + 1; }
				else { row += 1; break; }
			}
			else if (orientation == west) {
				if (maze[row][col].west_has_wall) { orientation = orientation % 4 + 1; }
				else { col -= 1; break; }
			}
		}

		// add node to linked list
		struct node* new_node = malloc(sizeof(struct node));
		current_node->next = new_node;
		new_node->next = NULL;
		new_node->prev = current_node;
		new_node->row = row;
		new_node->col = col;
		new_node->orientation = orientation;

		// update current node to prepare for next loop
		current_node = current_node->next;
	}

	// clean up list of directions for shortest path
	struct node* left_node = head;

	while (left_node != NULL) {
		// reset position of right node for next loop
		struct node* right_node = left_node->next;
		while (right_node != NULL) {
			if (left_node->row == right_node->row && left_node->col == right_node->col) {
				// get rid of nodes between left and right node, leaving the right node in the list but not the left

				// store reference to left node for memory freeing purposes
				struct node* temp1 = left_node;
				struct node* temp2 = left_node;

				// update trail of nodes by skipping first instance of cell and pointing to second instance
				left_node->prev->next = right_node;
				// update right node's prev pointer to point to the node before left_node
				right_node->prev = left_node->prev;
				right_node->orientation = left_node->orientation;
				// set left node to right node to prepare for next iterations of loop
				left_node = right_node;

				// free the memory on the heap taken up by items in the list we no longer need
				while (temp1 != right_node) {
					temp2 = temp1->next;
					free(temp1);
					temp1 = temp2;
				}
			}
			right_node = right_node->next;
		}
		left_node = left_node->next;
	}

	/*
	int x = 0;
	printf("\n\nSHORTEST PATH:\n");
	struct node* temp = head;
	while (temp != NULL) {
		printf("Row: %i, Col: %i\n", temp->row, temp->col);
		temp = temp->next;
		x += 1;
	}
	printf("Shortest path found! %i moves.\n", x);

	*/
}




GLuint model_view_matrix_location;
mat4 model_view_matrix = {
{ 1.0f, 0.0f, 0.0f, 0.0f },
{ 0.0f, 1.0f, 0.0f, 0.0f },
{ 0.0f, 0.0f, 1.0f, 0.0f },
{ 0.0f, 0.0f, 0.0f, 1.0f }};

mat4 og_view_matrix = {
{ 1.0f, 0.0f, 0.0f, 0.0f },
{ 0.0f, 1.0f, 0.0f, 0.0f },
{ 0.0f, 0.0f, 1.0f, 0.0f },
{ 0.0f, 0.0f, 0.0f, 1.0f }};


mat4 line_tranforms[66];

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ------------------------------- MAZE BASE CODE END -------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// main array of vertices, large enough to hold verts if every side of each cell contained a wall
#define MAZE_VERTS 11700
vec4 vertices[MAZE_VERTS + 36];

void create_geometry() {

	// ---------- GENERATE BASE CUBE ----------
	
	// create initial square face
	vec4 base_cube[36] = {
	{ -0.5f,  0.5f, 0.5f, 1.0f }, // top left
	{ -0.5f, -0.5f, 0.5f, 1.0f }, // bot left
	{  0.5f, -0.5f, 0.5f, 1.0f }, // bot right
	{ -0.5f,  0.5f, 0.5f, 1.0f }, // top left
	{  0.5f, -0.5f, 0.5f, 1.0f }, // bot right
	{  0.5f,  0.5f, 0.5f, 1.0f }, // top right
	};
	
	// copy and rotate inital cube to form full cube
	// SIDE COMMENTS ARE RELATIVE TO DEFAULT CAMERA VIEW

	int offset = 6;
	float pi = 3.14159;
	mat4 curr_xform = xform_rot_mat('x', pi/2);

	// bottom
	for (int i = 0; i < 6; ++i) {
		base_cube[i + offset] = mat_vec_mult(curr_xform, base_cube[i]);
	}
	// right
	offset += 6;
	curr_xform = xform_rot_mat('z', pi/2);
	for (int i = 0; i < 6; ++i) {
		base_cube[i + offset] = mat_vec_mult(curr_xform, base_cube[i + 6]);
	}
	// top
	offset += 6;
	for (int i = 0; i < 6; ++i) {
		base_cube[i + offset] = mat_vec_mult(curr_xform, base_cube[i + 12]);
	}
	// left
	offset += 6;
	for (int i = 0; i < 6; ++i) {
		base_cube[i + offset] = mat_vec_mult(curr_xform, base_cube[i + 18]);
	}
	// back
	offset += 6;
	curr_xform = xform_rot_mat('y', -pi/2);
	for (int i = 0; i < 6; ++i) {
		base_cube[i + offset] = mat_vec_mult(curr_xform, base_cube[i + 24]);
	}

	// ------------------------------------------------------------------------
	// ---------- CUSTOMIZE CUBES FOR WALLS, FLOOR, POLES, AND LINES ----------
	// ------------------------------------------------------------------------

	// arrays to hold each base cube
	vec4 floor_cube[36];
	vec4 pole_cube[36];
	vec4 wall_cube[36];
	vec4 line_cube[36];
	
	// -------------------- FLOOR --------------------
	// create floor cube base by translating cube down .5 z units to align top with xy axis
	curr_xform = xform_trans_mat(0.0f, 0.0f, -0.5f);
	for (int i = 0; i < 36; ++i) {
		floor_cube[i] = mat_vec_mult(curr_xform, base_cube[i]);
	}

	// -------------------- POLE --------------------
	// create pole cube by moving up so bottom lies on xy axis, then scale down on x and y axis
	curr_xform = mat_mult(xform_scale_mat(0.25f, 0.25f, 1.0f), xform_trans_mat(0.0f, 0.0f, 0.5f));
	for (int i = 0; i < 36; ++i) {
		pole_cube[i] = mat_vec_mult(curr_xform, base_cube[i]);
	}

	// -------------------- WALL --------------------
	// create wall cube by moving up bottom slightly less thatn .5 units, then scaling to be thin on one axis
	// NOTE: WALL IS THIN ON THE Y AXIS
	curr_xform = mat_mult(xform_scale_mat(1.0f, 0.1f, 1.0f), xform_trans_mat(0.0f, 0.0f, 0.4f));
	for (int i = 0; i < 36; ++i) {
		wall_cube[i] = mat_vec_mult(curr_xform, base_cube[i]);
	}

	// -------------------- LINE --------------------
	// create line cube by aligning with __________ axis and scaling to proper length
	//curr_xform = mat_mult(xform_scale_mat(1.0f, 0.5f, 1.0f), mat_mult(xform_trans_mat(0.0f, -0.5f, 0.4f), xform_scale_mat(0.25f, 1.0f, 0.25f)));
	curr_xform = mat_mult(xform_trans_mat(0.0f, -0.5f, 0.8f), xform_scale_mat(0.25f, 1.0f, 0.25f));
	for (int i = 0; i < 36; ++i) {
		line_cube[i] = mat_vec_mult(curr_xform, base_cube[i]);
	}


	// -----------------------------------------------------------
	// ---------- COPY AND TRANSFORM CUBES TO FORM MAZE ----------
	// -----------------------------------------------------------

	// NOTE: maze is fomed in the +x -y quadrant of the xy plane to make construction easier by being able to directly
	// take maze cell coordinates and place things

	// GROUND
	int index = 0;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			for (int k = 0; k < 36; ++k) {
				vertices[index] = floor_cube[k];
				// move cube in one unit increments to form a floor
				vertices[index].x += i;
				vertices[index].y += j;
				++index;
			}
		}
	}

	// POLES
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			for (int k = 0; k < 36; ++k) {
				vertices[index] = pole_cube[k];
				vertices[index].x += i + 0.5f;
				vertices[index].y += j + 0.5f;
				++index;
			}
		}
	}

	// VERTICAL WALLS
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			for (int k = 0; k < 36; ++k) {
				if (maze[i][j].west_has_wall) {
					vertices[index] = wall_cube[k];
					vertices[index].x += i + 1.0f;
					vertices[index].y += j + 0.5f;
					++index;
				}
			}
		}
	}
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 36; ++j) {
			if (maze[i][7].east_has_wall) {
				vertices[index] = wall_cube[j];
				vertices[index].x += i + 1.0f;
				vertices[index].y += 8.5f;
				++index;
			}
		}
	}
	// HORIZONTAL WALLS
	curr_xform = xform_rot_mat('z', 3.14159f/2.0f);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			for (int k = 0; k < 36; ++k) {
				if (maze[i][j].north_has_wall) {
					vertices[index] = mat_vec_mult(curr_xform, wall_cube[k]);
					vertices[index].x += i + 0.5f;
					vertices[index].y += j + 1.0f;
					++index;
				}
			}
		}
	}
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 36; ++j) {
			if (maze[7][i].south_has_wall) {
				vertices[index] = mat_vec_mult(curr_xform, wall_cube[j]);
				vertices[index].x += 8.5f;
				vertices[index].y += i + 1.0f;
				++index;
			}
		}
	}


	index = MAZE_VERTS;
	// TEST CUBE
	for (int i = 0; i < 36; ++i) {
		vertices[index] = mat_vec_mult(curr_xform, line_cube[i]);
		index++;
	}

	// center on screen
	curr_xform = xform_trans_mat(-4.5f, -4.5f, 0.0f);
	for (int i = 0; i < MAZE_VERTS; ++i) {
		vertices[i] = mat_vec_mult(curr_xform, vertices[i]);
	}
	// scale to fit screen
	curr_xform = xform_scale_mat(0.2f, 0.2f, 0.2f);
	for (int i = 0; i < MAZE_VERTS; ++i) {
		vertices[i] = mat_vec_mult(curr_xform, vertices[i]);
	}
	// rotate to make north up
	curr_xform = xform_rot_mat('z', -3.14159f/2.0f);
	for (int i = 0; i < MAZE_VERTS; ++i) {
		vertices[i] = mat_vec_mult(curr_xform, vertices[i]);
	}
}


int num_vertices = MAZE_VERTS + 36;

// CAMERA CONTROL VARIABLES
float scale = 0.8f;
float up_down_rot = -0.5f;
float left_right_rot = 0.5f;

bool first_click = true;
int prev_x = 0;
int prev_y = 0;

#define up 0
#define down 1
bool lmb_state = up;


// bool to determine when to start the animation
bool is_animating = false;

// current state of animation

int current_step = 0;
int max_steps = 100;

mat4 anim_tranforms[66];
float tick = 0.0f;

void idle() {

	// used for rotation logic to get initial click position
	if (lmb_state == up) {
		first_click = true;
	}

	// camera view computations
	model_view_matrix = mat_mult(
					xform_rot_mat('x', up_down_rot),
					mat_mult(
						xform_rot_mat('z', left_right_rot),
						xform_scale_mat(scale, scale, scale)
						)
				   );

	// logic for line animation
	anim_tranforms[(int)tick] = mat_mult(
					xform_trans_mat(1.0f, 0.0f, 0.0f),
					mat_mult(
					xform_scale_mat(-1.0f, 1.0f, 1.0f),
					mat_mult(
						xform_trans_mat(0.13f, 0.0f, 0.0f),
						xform_scale_mat(tick - (int)tick, 1.0f, 1.0f)
					    )));

	tick += .025;

	// thin out relative to camera view to avoid camera plane clipping
	model_view_matrix = mat_mult(
					xform_scale_mat(1.0f, 1.0f, 0.01f),
					model_view_matrix
				    );

	glutPostRedisplay();
}

void init(void)
{
	// OPEN IMAGE FILE AND CREATE NECESSARY DATA
	GLubyte my_texels[800][800][3];

	FILE *fp;

	fp = fopen("p2texture04.raw", "r");
	if (fp == NULL) {
		printf("ERROR: UNABLE TO OPEN FILE\n");
		exit(0);
	}

	fread(my_texels, 800 * 800 * 3, 1, fp);

	fclose(fp);

	// GENERATE TEXTURE COORDINATES
	GLfloat tex_coords[MAZE_VERTS][2];
	
	// ground
	for (int i = 0; i < 3600; /**/) {
		tex_coords[i][0] = 0.0f + 0.0f;
		tex_coords[i][1] = 0.0f + 0.5f;
		++i;
		tex_coords[i][0] = 0.0f + 0.0f;
		tex_coords[i][1] = 0.5f + 0.5f;
		++i;
		tex_coords[i][0] = 0.5f + 0.0f;
		tex_coords[i][1] = 0.5f + 0.5f;
		++i;
		tex_coords[i][0] = 0.0f + 0.0f;
		tex_coords[i][1] = 0.0f + 0.5f;
		++i;
		tex_coords[i][0] = 0.5f + 0.0f;
		tex_coords[i][1] = 0.5f + 0.5f;
		++i;
		tex_coords[i][0] = 0.5f + 0.0f;
		tex_coords[i][1] = 0.0f + 0.5f;
		++i;
	}
	
	// poles
	for (int i = 3600; i < 3600+2916; /**/) {
		tex_coords[i][0] = 0.0f + 0.5f;
		tex_coords[i][1] = 0.0f + 0.0f;
		++i;
		tex_coords[i][0] = 0.0f + 0.5f;
		tex_coords[i][1] = 0.5f + 0.0f;
		++i;
		tex_coords[i][0] = 0.5f + 0.5f;
		tex_coords[i][1] = 0.5f + 0.0f;
		++i;
		tex_coords[i][0] = 0.0f + 0.5f;
		tex_coords[i][1] = 0.0f + 0.0f;
		++i;
		tex_coords[i][0] = 0.5f + 0.5f;
		tex_coords[i][1] = 0.5f + 0.0f;
		++i;
		tex_coords[i][0] = 0.5f + 0.5f;
		tex_coords[i][1] = 0.0f + 0.0f;
		++i;
	}
	 
	// walls
	for (int i = 2916+3600; i < 3600+2916+5184; /**/) {
		tex_coords[i][0] = 0.0f + 0.0f;
		tex_coords[i][1] = 0.0f + 0.0f;
		++i;
		tex_coords[i][0] = 0.0f + 0.0f;
		tex_coords[i][1] = 0.5f + 0.0f;
		++i;
		tex_coords[i][0] = 0.5f + 0.0f;
		tex_coords[i][1] = 0.5f + 0.0f;
		++i;
		tex_coords[i][0] = 0.0f + 0.0f;
		tex_coords[i][1] = 0.0f + 0.0f;
		++i;
		tex_coords[i][0] = 0.5f + 0.0f;
		tex_coords[i][1] = 0.5f + 0.0f;
		++i;
		tex_coords[i][0] = 0.5f + 0.0f;
		tex_coords[i][1] = 0.0f + 0.0f;
		++i;
	}

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    //
    GLuint mytex;
    glGenTextures(1, &mytex);
    glBindTexture(GL_TEXTURE_2D, mytex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    //

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(tex_coords), tex_coords);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + sizeof(vertices));

    model_view_matrix_location = glGetUniformLocation(program, "model_view_matrix");
    use_texture_location = glGetUniformLocation(program, "use_texture");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

mat4 orig_model_view_matrix;

void display(void)
{
    // save model view matrix
    orig_model_view_matrix = model_view_matrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);

    // draw maze itself
    glUniform1i(use_texture_location, 1); // switch to using textures
    glDrawArrays(GL_TRIANGLES, 0, MAZE_VERTS);

    // save model view matrix

    glUniform1i(use_texture_location, 0); // switch to using solid color
    // draw animated solve lines
    for (int i = 0; i < 66; ++i) {
	model_view_matrix = mat_mult(orig_model_view_matrix, mat_mult(line_tranforms[i], anim_tranforms[i]));
	//model_view_matrix = line_tranforms[i];
        glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);

	//mat_print(model_view_matrix);
    	glDrawArrays(GL_TRIANGLES, MAZE_VERTS, 36);
    }
    glUniform1i(use_texture_location, 1); // switch to using textures

    model_view_matrix = og_view_matrix;
    glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();

    //glutPostRedisplay();
}



void mouse(int button, int state, int x, int y) {

	// set variables for mouse rotation
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		lmb_state = down;

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		lmb_state = up;
	}

	// mw zoom in
	if (button == 3) {
		scale += .03f;
	}
	// mw zoom out
	if (button == 4) {
		scale -= .03f;
	}
}

void passive(int x, int y) {

	if (lmb_state == down) {
		if (first_click) {
			// no previous click data, don't move yet
			prev_x = x;
			prev_y = y;
			first_click = false;
		}
		else {
			up_down_rot -= 0.003f * (float)(prev_y - y);
			left_right_rot -= 0.003f * (float)(prev_x - x);

			// reset values for next frame
			prev_x = x;
			prev_y = y;
		}
	}
}

void reshape(int width, int height)
{
    glViewport(0, 0, 800, 800);
}

int steps = 0;

int main(int argc, char **argv)
{
	
	start_maze_generation();
	print_maze();
	solve_maze();
	create_geometry();
	

	// matrix that matches transforms applied to the maze itself
	mat4 maze_match_xform = mat_mult(xform_rot_mat('z', -3.14159f/2.0f), mat_mult(xform_scale_mat(0.2f, 0.2f, 0.2f), xform_trans_mat(-4.5f, -4.5f, 0.0f)));
	
	struct node* temp_head = head;
	float line_x_trans = 1.0f;
	float line_y_trans = 0.0f;
	float line_rot = 0.0f;

	int i = 0;
	// initialize all line transforms to proper values
	for (   ; i < 66; ++i) {
		
		if (temp_head->next == NULL) { break; }

		switch (temp_head->orientation) {
			case north:
				line_rot = 0.0f; break;
			case south:
				line_rot = 3.14159f; break;
			case east:
				line_rot = -3.14159f/2.0f; break;
			case west:
				line_rot = 3.14159f/2.0f; break;
		}

		line_tranforms[i] = mat_mult(
					maze_match_xform,
					mat_mult(
						xform_trans_mat((float)temp_head->row + 1, (float)temp_head->col + 1, 0.0f),
						xform_rot_mat('z', line_rot)
						)
					    );

		switch (temp_head->orientation) {
			case north:
				line_x_trans += 0.0f; line_y_trans -= 1.0f; break;
			case south:
				line_x_trans += 0.0f; line_y_trans += 1.0f; break;
			case east:
				line_x_trans += 1.0f; line_y_trans += 0.0f; break;
			case west:
				line_x_trans -= 1.0f; line_y_trans += 0.0f; break;
		}
		
		temp_head = temp_head->next;
	}

	// Add final two lines to maze exit
	if (temp_head->orientation == south) {
		line_tranforms[i] = mat_mult(maze_match_xform, mat_mult(xform_trans_mat((float)8, (float)8, 0.0f), xform_rot_mat('z', 3.14159f)));
	} else {
		line_tranforms[i] = mat_mult(maze_match_xform, mat_mult(xform_trans_mat((float)8, (float)8, 0.0f), xform_rot_mat('z', -3.14159f/2.0f)));
	}
	i += 1;
	line_tranforms[i] = mat_mult(maze_match_xform, mat_mult(xform_trans_mat((float)9, (float)8, 0.0f), xform_rot_mat('z', 3.14159f)));
	

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Maze Program");
    glEnable(GL_BLEND);
    glewInit();
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(passive);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
