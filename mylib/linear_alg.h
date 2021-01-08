// +----------------------------------------------------------------------+
// |                                                                      |
// |                              VEC4                                    |
// |                                                                      |
// +----------------------------------------------------------------------+

// +---------------------+
// |   TYPE DEFINITION   |
// +---------------------+

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4;


// +---------------+
// |   FUNCTIONS   |
// +---------------+

// print vector to console
void vec_print(vec4 v); 

// scalar vector multiplication
vec4 float_vec_mult(float s, vec4 v);
// vector scalar multiplication, backup for different argument order
vec4 vec_float_mult(vec4 v, float s);

// vector vector addition
vec4 vec_add(vec4 v1, vec4 v2);
// vector vector subtraction
vec4 vec_sub(vec4 v1, vec4 v2);

// vector magnitude
float vec_mag(vec4 v);
// vector normalize
vec4 vec_norm(vec4 v);

// vector dot product
float vec_dot(vec4 v1, vec4 v2);
// vector cross product
vec4 vec_cross(vec4 v1, vec4 v2);


// +----------------------------------------------------------------------+
// |                                                                      |
// |                              MAT4                                    |
// |                                                                      |
// +----------------------------------------------------------------------+

// +---------------------+
// |   TYPE DEFINITION   |
// +---------------------+

typedef struct {
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;

// +---------------+
// |   FUNCTIONS   |
// +---------------+

// print
void mat_print(mat4 m);

// scalar matrix multiplication
mat4 float_mat_mult(float s, mat4 m);

// matrix matrix addition
mat4 mat_add(mat4 m1, mat4 m2);
// matrix matrix subtraction
mat4 mat_sub(mat4 m1, mat4 m2);

// matrix matrix multiplication
mat4 mat_mult(mat4 m1, mat4 m2);

// matrix inverse
mat4 mat_inv(mat4 m);
// matrix transpose
mat4 mat_trans(mat4 m);

// matrix vector multiplication
vec4 mat_vec_mult(mat4 m, vec4 v);



// +----------------------------------------------------------------------+
// |                                                                      |
// |                            TRANSFORM                                 |
// |                                                                      |
// +----------------------------------------------------------------------+

// translation
mat4 xform_trans_mat(float x, float y, float z);

// scaling
mat4 xform_scale_mat(float x, float y, float z);

// rotation
mat4 xform_rot_mat(char axis, float a);


mat4 look_at(vec4 eye, vec4 at, vec4 up);
