#include <stdio.h>
#include <math.h>
#include "linear_alg.h"

// +---------------+
// |   FUNCTIONS   |
// +---------------+

// print vector to console
void vec_print(vec4 v) {
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v.x, v.y, v.z, v.w);
}

// scalar vector multiplication
vec4 float_vec_mult(float s, vec4 v) {
	vec4 result = {v.x * s,
			      v.y * s,
			      v.z * s, 
			      v.w * s};
	return result;
}
// vector scalar multiplication, backup for different argument order
vec4 vec_float_mult(vec4 v, float s) {
	vec4 result = {v.x * s,
			      v.y * s,
			      v.z * s, 
			      v.w * s};
	return result;
}

// vector vector addition
vec4 vec_add(vec4 v1, vec4 v2) {
	vec4 result = {v1.x + v2.x,
		    v1.y + v2.y,
		    v1.z + v2.z,
		    v1.w + v2.w};
	return result;
}

// vector vector subtraction
vec4 vec_sub(vec4 v1, vec4 v2) {
	vec4 result = {v1.x - v2.x,
		    v1.y - v2.y,
		    v1.z - v2.z,
		    v1.w - v2.w};
	return result;
}

// vector magnitude
float vec_mag(vec4 v) {
	float result = sqrtf(v.x * v.x +
		     v.y * v.y +
		     v.z * v.z +
		     v.w * v.w);
	return result;
}

// vector normalize
vec4 vec_norm(vec4 v) {
	return float_vec_mult((1 / vec_mag(v)), v);
}

// vector dot product
float vec_dot(vec4 v1, vec4 v2) {
	return v1.x * v2.x +
	       v1.y * v2.y +
	       v1.z * v2.z +
	       v1.w * v2.w;
}

// vector cross product
vec4 vec_cross(vec4 v1, vec4 v2) {
	vec4 result = {v1.y * v2.z - v1.z * v2.y,
		    v1.z * v2.x - v1.x * v2.z,
		    v1.x * v2.y - v1.y * v2.x,
		    0.0f};
	return result;
}


// +----------------------------------------------------------------------+
// |                                                                      |
// |                              MAT4                                    |
// |                                                                      |
// +----------------------------------------------------------------------+

// +---------------+
// |   FUNCTIONS   |
// +---------------+

// print
void mat_print(mat4 m) {
	printf("[%.5f, %.5f, %.5f, %.5f]\n", m.x.x, m.y.x, m.z.x, m.w.x);
	printf("[%.5f, %.5f, %.5f, %.5f]\n", m.x.y, m.y.y, m.z.y, m.w.y);
	printf("[%.5f, %.5f, %.5f, %.5f]\n", m.x.z, m.y.z, m.z.z, m.w.z);
	printf("[%.5f, %.5f, %.5f, %.5f]\n", m.x.w, m.y.w, m.z.w, m.w.w);
}

// scalar matrix multiplication
mat4 float_mat_mult(float s, mat4 m) {
	mat4 result = {float_vec_mult(s, m.x),
		       float_vec_mult(s, m.y),
		       float_vec_mult(s, m.z),
		       float_vec_mult(s, m.w)};
	return result;
}

// matrix matrix addition
mat4 mat_add(mat4 m1, mat4 m2) {

	mat4 result = {
	{ m1.x.x + m2.x.x, m1.x.y + m2.x.y, m1.x.z + m2.x.z, m1.x.w + m2.x.w },
	{ m1.y.x + m2.y.x, m1.y.y + m2.y.y, m1.y.z + m2.y.z, m1.y.w + m2.y.w },
	{ m1.z.x + m2.z.x, m1.z.y + m2.z.y, m1.z.z + m2.z.z, m1.z.w + m2.z.w },
	{ m1.w.x + m2.w.x, m1.w.y + m2.w.y, m1.w.z + m2.w.z, m1.w.w + m2.w.w }};

	return result;
}

// matrix matrix subtraction
mat4 mat_sub(mat4 m1, mat4 m2) {
	mat4 result = {
	{ m1.x.x - m2.x.x, m1.x.y - m2.x.y, m1.x.z - m2.x.z, m1.x.w - m2.x.w },
	{ m1.y.x - m2.y.x, m1.y.y - m2.y.y, m1.y.z - m2.y.z, m1.y.w - m2.y.w },
	{ m1.z.x - m2.z.x, m1.z.y - m2.z.y, m1.z.z - m2.z.z, m1.z.w - m2.z.w },
	{ m1.w.x - m2.w.x, m1.w.y - m2.w.y, m1.w.z - m2.w.z, m1.w.w - m2.w.w }};
	return result;
}

// matrix matrix multiplication
mat4 mat_mult(mat4 m1, mat4 m2) {
	mat4 result = {
		{
			m1.x.x * m2.x.x + m1.y.x * m2.x.y + m1.z.x * m2.x.z + m1.w.x * m2.x.w,
			m1.x.y * m2.x.x + m1.y.y * m2.x.y + m1.z.y * m2.x.z + m1.w.y * m2.x.w,
			m1.x.z * m2.x.x + m1.y.z * m2.x.y + m1.z.z * m2.x.z + m1.w.z * m2.x.w,
			m1.x.w * m2.x.x + m1.y.w * m2.x.y + m1.z.w * m2.x.z + m1.w.w * m2.x.w
		},

		{
			m1.x.x * m2.y.x + m1.y.x * m2.y.y + m1.z.x * m2.y.z + m1.w.x * m2.y.w,
			m1.x.y * m2.y.x + m1.y.y * m2.y.y + m1.z.y * m2.y.z + m1.w.y * m2.y.w,
			m1.x.z * m2.y.x + m1.y.z * m2.y.y + m1.z.z * m2.y.z + m1.w.z * m2.y.w,
			m1.x.w * m2.y.x + m1.y.w * m2.y.y + m1.z.w * m2.y.z + m1.w.w * m2.y.w
		},

		{
			m1.x.x * m2.z.x + m1.y.x * m2.z.y + m1.z.x * m2.z.z + m1.w.x * m2.z.w,
			m1.x.y * m2.z.x + m1.y.y * m2.z.y + m1.z.y * m2.z.z + m1.w.y * m2.z.w,
			m1.x.z * m2.z.x + m1.y.z * m2.z.y + m1.z.z * m2.z.z + m1.w.z * m2.z.w,
			m1.x.w * m2.z.x + m1.y.w * m2.z.y + m1.z.w * m2.z.z + m1.w.w * m2.z.w
		},

		{
			m1.x.x * m2.w.x + m1.y.x * m2.w.y + m1.z.x * m2.w.z + m1.w.x * m2.w.w,
			m1.x.y * m2.w.x + m1.y.y * m2.w.y + m1.z.y * m2.w.z + m1.w.y * m2.w.w,
			m1.x.z * m2.w.x + m1.y.z * m2.w.y + m1.z.z * m2.w.z + m1.w.z * m2.w.w,
			m1.x.w * m2.w.x + m1.y.w * m2.w.y + m1.z.w * m2.w.z + m1.w.w * m2.w.w
		}
	};
	return result;
}

// matrix inverse
mat4 mat_inv(mat4 m) {
		
		// MATRIX OF MINOR
		// a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h
		mat4 minor = {
		{
		m.y.y*m.z.z*m.w.w + m.z.y*m.w.z*m.y.w + m.w.y*m.y.z*m.z.w - m.w.y*m.z.z*m.y.w - m.z.y*m.y.z*m.w.w - m.y.y*m.w.z*m.z.w,
		m.y.x*m.z.z*m.w.w + m.z.x*m.w.z*m.y.w + m.w.x*m.y.z*m.z.w - m.w.x*m.z.z*m.y.w - m.z.x*m.y.z*m.w.w - m.y.x*m.w.z*m.z.w,
		m.y.x*m.z.y*m.w.w + m.z.x*m.w.y*m.y.w + m.w.x*m.y.y*m.z.w - m.w.x*m.z.y*m.y.w - m.z.x*m.y.y*m.w.w - m.y.x*m.w.y*m.z.w,
		m.y.x*m.z.y*m.w.z + m.z.x*m.w.y*m.y.z + m.w.x*m.y.y*m.z.z - m.w.x*m.z.y*m.y.z - m.z.x*m.y.y*m.w.z - m.y.x*m.w.y*m.z.z
		},

		{
		m.x.y*m.z.z*m.w.w + m.z.y*m.w.z*m.x.w + m.w.y*m.x.z*m.z.w - m.w.y*m.z.z*m.x.w - m.z.y*m.x.z*m.w.w - m.x.y*m.w.z*m.z.w,
		m.x.x*m.z.z*m.w.w + m.z.x*m.w.z*m.x.w + m.w.x*m.x.z*m.z.w - m.w.x*m.z.z*m.x.w - m.z.x*m.x.z*m.w.w - m.x.x*m.w.z*m.z.w,
		m.x.x*m.z.y*m.w.w + m.z.x*m.w.y*m.x.w + m.w.x*m.x.y*m.z.w - m.w.x*m.z.y*m.x.w - m.z.x*m.x.y*m.w.w - m.x.x*m.w.y*m.z.w,
		m.x.x*m.z.y*m.w.z + m.z.x*m.w.y*m.x.z + m.w.x*m.x.y*m.z.z - m.w.x*m.z.y*m.x.z - m.z.x*m.x.y*m.w.z - m.x.x*m.w.y*m.z.z
		},

		{
		m.x.y*m.y.z*m.w.w + m.y.y*m.w.z*m.x.w + m.w.y*m.x.z*m.y.w - m.w.y*m.y.z*m.x.w - m.y.y*m.x.z*m.w.w - m.x.y*m.w.z*m.y.w,
		m.x.x*m.y.z*m.w.w + m.y.x*m.w.z*m.x.w + m.w.x*m.x.z*m.y.w - m.w.x*m.y.z*m.x.w - m.y.x*m.x.z*m.w.w - m.x.x*m.w.z*m.y.w,
		m.x.x*m.y.y*m.w.w + m.y.x*m.w.y*m.x.w + m.w.x*m.x.y*m.y.w - m.w.x*m.y.y*m.x.w - m.y.x*m.x.y*m.w.w - m.x.x*m.w.y*m.y.w,
		m.x.x*m.y.y*m.w.z + m.y.x*m.w.y*m.x.z + m.w.x*m.x.y*m.y.z - m.w.x*m.y.y*m.x.z - m.y.x*m.x.y*m.w.z - m.x.x*m.w.y*m.y.z
		},

		{
		m.x.y*m.y.z*m.z.w + m.y.y*m.z.z*m.x.w + m.z.y*m.x.z*m.y.w - m.z.y*m.y.z*m.x.w - m.y.y*m.x.z*m.z.w - m.x.y*m.z.z*m.y.w,
		m.x.x*m.y.z*m.z.w + m.y.x*m.z.z*m.x.w + m.z.x*m.x.z*m.y.w - m.z.x*m.y.z*m.x.w - m.y.x*m.x.z*m.z.w - m.x.x*m.z.z*m.y.w,
		m.x.x*m.y.y*m.z.w + m.y.x*m.z.y*m.x.w + m.z.x*m.x.y*m.y.w - m.z.x*m.y.y*m.x.w - m.y.x*m.x.y*m.z.w - m.x.x*m.z.y*m.y.w,
		m.x.x*m.y.y*m.z.z + m.y.x*m.z.y*m.x.z + m.z.x*m.x.y*m.y.z - m.z.x*m.y.y*m.x.z - m.y.x*m.x.y*m.z.z - m.x.x*m.z.y*m.y.z
		}
		};
		
		// MATRIX OF COFACTOR
		mat4 cofactor = {{minor.x.x, -minor.x.y, minor.x.z, -minor.x.w},
						 {-minor.y.x, minor.y.y, -minor.y.z, minor.y.w},
						 {minor.z.x, -minor.z.y, minor.z.z, -minor.z.w},
						 {-minor.w.x, minor.w.y, -minor.w.z, minor.w.w}};


		// TRANSPOSE MATRIX
		mat4 trans = mat_trans(cofactor);

		// GET DETERMINANT
		// a11 * m11 - a12 * m12 + a13 * m13 - a14 * m14
		float deter = m.x.x * minor.x.x - m.y.x * minor.y.x + m.z.x * minor.z.x - m.w.x * minor.w.x;

		// MULTIPLY BY 1/DETERMINANT
		return float_mat_mult(1/deter, trans);

};

// matrix transpose
mat4 mat_trans(mat4 m) {
		mat4 result = {{m.x.x, m.y.x, m.z.x, m.w.x},
						{m.x.y, m.y.y, m.z.y, m.w.y},
						{m.x.z, m.y.z, m.z.z, m.w.z},
						{m.x.w, m.y.w, m.z.w, m.w.w}};
		return result;
}

// matrix vector multiplication
vec4 mat_vec_mult(mat4 m, vec4 v) {
		vec4 result = {m.x.x * v.x + m.y.x * v.y + m.z.x * v.z + m.w.x * v.w,
				       m.x.y * v.x + m.y.y * v.y + m.z.y * v.z + m.w.y * v.w,
				       m.x.z * v.x + m.y.z * v.y + m.z.z * v.z + m.w.z * v.w,
				       m.x.w * v.x + m.y.w * v.y + m.z.w * v.z + m.w.w * v.w};
		return result;
}


// +----------------------------------------------------------------------+
// |                                                                      |
// |                            TRANSFORM                                 |
// |                                                                      |
// +----------------------------------------------------------------------+

// translation
mat4 xform_trans_mat(float x, float y, float z) {
	
	mat4 result = {{1, 0, 0, 0},
		       {0, 1, 0, 0},
		       {0, 0, 1, 0},
		       {x, y, z, 1}};
	return result;
}

// scale
mat4 xform_scale_mat(float x, float y, float z) {
	
	mat4 result = {{x, 0, 0, 0},
		       {0, y, 0, 0},
		       {0, 0, z, 0},
		       {0, 0, 0, 1}};
	return result;
}

// rotation
mat4 xform_rot_mat(char axis, float a) {
	
	mat4 result;
	
	if (axis == 'x') {
	
		mat4 tempres = {{1,	0,	0,	0},
			       {0,	cos(a),	sin(a),	0},
			       {0,	-sin(a),cos(a),	0},
			       {0,	0,	0,	1}};
		result = tempres;

	}
	else if (axis == 'y') {

		mat4 tempres = {{cos(a),	0,	-sin(a),0},
			       {0,	1,	0,	0},
			       {sin(a),	0,	cos(a),	0},
			       {0,	0,	0,	1}};

		result = tempres;
	}
	else if (axis == 'z') {

		mat4 tempres = {{cos(a),	sin(a),	0,	0},
			       {-sin(a),cos(a),	0,	0},
			       {0,	0,	1,	0},
			       {0,	0,	0,	1}};

		result = tempres;
	}
	else {

	printf("ERROR: Invalid axis for rotation. Returned matrix will not have any rotation.");
	mat4 tempres = {{1, 0, 0, 0},
		       {0, 1, 0, 0},
		       {0, 0, 1, 0},
		       {0, 0, 0, 1}};
		result = tempres;
	}

	return result;
}

// +----------------------------------------------------------------------+
// |                                                                      |
// |                            TRANSFORM                                 |
// |                                                                      |
// +----------------------------------------------------------------------+

mat4 look_at(vec4 eye, vec4 at, vec4 up) {

	vec4 forward = vec_norm(vec_sub(eye, at));
	vec4 left = vec_norm(vec_cross(forward, up));
	vec4 new_up = vec_cross(forward, left);

	// turn around forward axis due to camera z- being the front
	forward = float_vec_mult(-1, forward);

	mat4 result = {
		{left.x, new_up.x, forward.x, 0},
		{left.y, new_up.y, forward.y, 0},
		{left.z, new_up.z, forward.z, 0},
		{-vec_dot(left, eye), -vec_dot(up, eye), -vec_dot(forward, eye), 1}
	};
	return result;
}

mat4 ortho(float left, float right, float bottom, float top, float near, float far) {}

mat4 frustrum(float left, float right, float bottom, float top, float near, float far) {}


