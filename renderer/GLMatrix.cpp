//
// Created by HB on 5/20/2015.
//
#include "stdafx.h"
#include "GLMatrix.h"

#include <math.h>
#include <memory.h>

#include "../Basic.h"

const float identity[] = {1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1};

//void gl_multiplyMM(float *r, const float *lhs, const float *rhs);
//
//void gl_setRotateM(float *rm, int rmOffset, float a, float x, float y, float z);
//
//void gl_transposeM(float *mTrans, int mTransOffset, float *m, int mOffset);
//
//void gl_matrix4_proj(float *mat, float *vec);
//
//void gl_matrix4_proj2D(float *mat, float *vec, float *out);


/*
 * Copyright 2007, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// from android/platform_frameworks_base/blob/master/core/jni/android/opengl/util.cpp
#define I(_i, _j) ((_j)+ 4*(_i))

static inline void gl_multiplyMM(float *r, const float *lhs, const float *rhs) {
    for (int i = 0; i < 4; i++) {
        register const float rhs_i0 = rhs[I(i, 0)];
        register float ri0 = lhs[I(0, 0)] * rhs_i0;
        register float ri1 = lhs[I(0, 1)] * rhs_i0;
        register float ri2 = lhs[I(0, 2)] * rhs_i0;
        register float ri3 = lhs[I(0, 3)] * rhs_i0;
        for (int j = 1; j < 4; j++) {
            register const float rhs_ij = rhs[I(i, j)];
            ri0 += lhs[I(j, 0)] * rhs_ij;
            ri1 += lhs[I(j, 1)] * rhs_ij;
            ri2 += lhs[I(j, 2)] * rhs_ij;
            ri3 += lhs[I(j, 3)] * rhs_ij;
        }
        r[I(i, 0)] = ri0;
        r[I(i, 1)] = ri1;
        r[I(i, 2)] = ri2;
        r[I(i, 3)] = ri3;
    }
}

/**
 * Computes the length of a vector
 *
 * @param x x coordinate of a vector
 * @param y y coordinate of a vector
 * @param z z coordinate of a vector
 * @return the length of a vector
 */
static inline float gl_length(float x, float y, float z) {
    return (float) sqrt(x * x + y * y + z * z);
}

/**
 * Rotates matrix m by angle a (in degrees) around the axis (x, y, z)
 * @param rm returns the result
 * @param rmOffset index into rm where the result matrix starts
 * @param a angle to rotate in degrees
 * @param x scale factor x
 * @param y scale factor y
 * @param z scale factor z
 */

static inline void gl_setRotateM(float *rm, int rmOffset, float a, float x, float y, float z) {
    rm[rmOffset + 3] = 0;
    rm[rmOffset + 7] = 0;
    rm[rmOffset + 11] = 0;
    rm[rmOffset + 12] = 0;
    rm[rmOffset + 13] = 0;
    rm[rmOffset + 14] = 0;
    rm[rmOffset + 15] = 1;
    a *= (float) (PI / 180.0f);
    float s = (float) sin(a);
    float c = (float) cos(a);
    if (1.0f == x && 0.0f == y && 0.0f == z) {
        rm[rmOffset + 5] = c;
        rm[rmOffset + 10] = c;
        rm[rmOffset + 6] = s;
        rm[rmOffset + 9] = -s;
        rm[rmOffset + 1] = 0;
        rm[rmOffset + 2] = 0;
        rm[rmOffset + 4] = 0;
        rm[rmOffset + 8] = 0;
        rm[rmOffset + 0] = 1;
    }
    else if (0.0f == x && 1.0f == y && 0.0f == z) {
        rm[rmOffset + 0] = c;
        rm[rmOffset + 10] = c;
        rm[rmOffset + 8] = s;
        rm[rmOffset + 2] = -s;
        rm[rmOffset + 1] = 0;
        rm[rmOffset + 4] = 0;
        rm[rmOffset + 6] = 0;
        rm[rmOffset + 9] = 0;
        rm[rmOffset + 5] = 1;
    }
    else if (0.0f == x && 0.0f == y && 1.0f == z) {
        rm[rmOffset + 0] = c;
        rm[rmOffset + 5] = c;
        rm[rmOffset + 1] = s;
        rm[rmOffset + 4] = -s;
        rm[rmOffset + 2] = 0;
        rm[rmOffset + 6] = 0;
        rm[rmOffset + 8] = 0;
        rm[rmOffset + 9] = 0;
        rm[rmOffset + 10] = 1;
    }
    else {
        float len = gl_length(x, y, z);
        if (1.0f != len) {
            float recipLen = 1.0f / len;
            x *= recipLen;
            y *= recipLen;
            z *= recipLen;
        }
        float nc = 1.0f - c;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * s;
        float ys = y * s;
        float zs = z * s;
        rm[rmOffset + 0] = x * x * nc + c;
        rm[rmOffset + 4] = xy * nc - zs;
        rm[rmOffset + 8] = zx * nc + ys;
        rm[rmOffset + 1] = xy * nc + zs;
        rm[rmOffset + 5] = y * y * nc + c;
        rm[rmOffset + 9] = yz * nc - xs;
        rm[rmOffset + 2] = zx * nc - ys;
        rm[rmOffset + 6] = yz * nc + xs;
        rm[rmOffset + 10] = z * z * nc + c;
    }
}

/**
 * Transposes a 4 x 4 matrix.
 *
 * @param mTrans the array that holds the output inverted matrix
 * @param mTransOffset an offset into mInv where the inverted matrix is
 *        stored.
 * @param m the input array
 * @param mOffset an offset into m where the matrix is stored.
 */
static inline void gl_transposeM(float *mTrans, int mTransOffset, float *m, int mOffset) {
    for (int i = 0; i < 4; i++) {
        int mBase = i * 4 + mOffset;
        mTrans[i + mTransOffset] = m[mBase];
        mTrans[i + 4 + mTransOffset] = m[mBase + 1];
        mTrans[i + 8 + mTransOffset] = m[mBase + 2];
        mTrans[i + 12 + mTransOffset] = m[mBase + 3];
    }
}
/*******************************************************************************
 * Copyright 2011 See libgdx AUTHORS file.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
// from /gdx/src/com/badlogic/gdx/math/Matrix4.java
#define M00 0
#define M01 4
#define M02 8
#define M03 12
#define M10 1
#define M11 5
#define M12 9
#define M13 13
#define M20 2
#define M21 6
#define M22 10
#define M23 14
#define M30 3
#define M31 7
#define M32 11
#define M33 15

static inline void gl_matrix4_proj(float *mat, float *vec) {
    float inv_w = 1.0f / (vec[0] * mat[M30] + vec[1] * mat[M31] + vec[2] * mat[M32] + mat[M33]);
    float x = (vec[0] * mat[M00] + vec[1] * mat[M01] + vec[2] * mat[M02] + mat[M03]) * inv_w;
    float y = (vec[0] * mat[M10] + vec[1] * mat[M11] + vec[2] * mat[M12] + mat[M13]) * inv_w;
    float z = (vec[0] * mat[M20] + vec[1] * mat[M21] + vec[2] * mat[M22] + mat[M23]) * inv_w;
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

static inline void gl_matrix4_proj2D(float *mat, float *vec, float *out) {

    float inv_w = 1.0f / (vec[0] * mat[M30] + vec[1] * mat[M31] + mat[M33]);
    float x = (vec[0] * mat[M00] + vec[1] * mat[M01] + mat[M03]) * inv_w;
    float y = (vec[0] * mat[M10] + vec[1] * mat[M11] + mat[M13]) * inv_w;

    out[0] = x;
    out[1] = y;
}



GLMatrix::GLMatrix() {
    m = new float[16];
	memset(m, 0, 16 * sizeof(float));
}

GLMatrix::~GLMatrix() {
    delete[]m;
}

void GLMatrix::set(float mat[]) {
    memcpy(m, mat, MAT_SIZE);
}

void GLMatrix::get(float mat[]) {
    memcpy(mat, m, MAT_SIZE);
}

void GLMatrix::copy(const GLMatrix &mat) {
    memcpy(m, mat.m, MAT_SIZE);
}

void GLMatrix::prj(float vec3[]) {
    gl_matrix4_proj(m, vec3);
}

void GLMatrix::prj3D(float vec3[], int offset, int length) {
    int size = length * 3;
    for (int i = offset * 3; i < size; i += 3)
        gl_matrix4_proj(m, (vec3 + i));
}

void GLMatrix::prj2D(float vec2[], int offset, int length) {
    offset *= 2;

    for (int end = offset + length * 2; offset < end; offset += 2)
        gl_matrix4_proj2D(m, (vec2 + offset), (vec2 + offset));
}

void GLMatrix::prj2D(float src[], int src_offset, float dst[], int dst_offset, int length) {

    int off_src = src_offset * 2;
    int off_dst = dst_offset * 2;

    for (int end = off_src + length * 2; off_src < end; off_src += 2, off_dst += 2) {
        gl_matrix4_proj2D(m, (src + off_src), (dst + off_dst));
    }
}

void GLMatrix::multiplyRhs(const GLMatrix &rhs) {
    float* mata = new float[16];
    memcpy(mata, m, 16 * sizeof(float));

    gl_multiplyMM(m, mata, rhs.m);
}

void GLMatrix::multiplyLhs(const GLMatrix &lhs) {
    float* matb = new float[16];
    memcpy(matb, m, 16 * sizeof(float));

    gl_multiplyMM(m, lhs.m, matb);
}

void GLMatrix::multiplyMM(const GLMatrix &lhs, const GLMatrix &rhs) {
    gl_multiplyMM(m, lhs.m, rhs.m);
}

void GLMatrix::transposeM(const GLMatrix &mat) {
    gl_transposeM(m, 0, mat.m, 0);
}

void GLMatrix::setRotation(float a, float x, float y, float z) {
    gl_setRotateM(m, 0, a, x, y, z);
}

void GLMatrix::setTranslation(float x, float y, float z) {
    memcpy(m, identity, MAT_SIZE);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

void GLMatrix::setScale(float sx, float sy, float sz) {
    memcpy(m, identity, MAT_SIZE);
    m[0] = sx;
    m[5] = sy;
    m[10] = sz;
}

void GLMatrix::setTransScale(float tx, float ty, float scale) {
    memcpy(m, identity, MAT_SIZE);
    m[0] = scale;
    m[5] = scale;
    m[12] = tx;
    m[13] = ty;
}

void GLMatrix::setAsUniform(int location) {
	throw std::exception("The method or operation is not implemented.");

    glUniformMatrix4fv(location, 1, GL_FALSE, m);
	
}

void GLMatrix::setValue(int pos, float value) {
    if (pos > -1 && pos < 16)
        m[pos] = value;
}

float someRandomEpsilon = 1.0f / (1 << 11);

void GLMatrix::addDepthOffset(int delta) {
    m[10] *= 1.0f + someRandomEpsilon * delta;
}

void GLMatrix::setIdentity() {
    memcpy(m, identity, MAT_SIZE);
}

void GLMatrix::frustumM(float m[], int offset, float left, float right, float bottom, float top, float near,
                        float far) {
    if (left == right) {
//        throw new IllegalArgumentException("left == right");
        return;
    }
    if (top == bottom) {
//        throw new IllegalArgumentException("top == bottom");
        return;
    }
    if (near == far) {
//        throw new IllegalArgumentException("near == far");
        return;
    }
    if (near <= 0.0f) {
//        throw new IllegalArgumentException("near <= 0.0f");
        return;
    }
    if (far <= 0.0f) {
//        throw new IllegalArgumentException("far <= 0.0f");
        return;
    }

    const float r_width = 1.0f / (right - left);
    const float r_height = 1.0f / (top - bottom);
    const float r_depth = 1.0f / (near - far);
    const float x = 2.0f * (near * r_width);
    const float y = 2.0f * (near * r_height);
    const float A = (right + left) * r_width;
    const float B = (top + bottom) * r_height;
    const float C = (far + near) * r_depth;
    const float D = 2.0f * (far * near * r_depth);
    m[offset + 0] = x;
    m[offset + 5] = y;
    m[offset + 8] = A;
    m[offset + 9] = B;
    m[offset + 10] = C;
    m[offset + 14] = D;
    m[offset + 11] = -1.0f;
    m[offset + 1] = 0.0f;
    m[offset + 2] = 0.0f;
    m[offset + 3] = 0.0f;
    m[offset + 4] = 0.0f;
    m[offset + 6] = 0.0f;
    m[offset + 7] = 0.0f;
    m[offset + 12] = 0.0f;
    m[offset + 13] = 0.0f;
    m[offset + 15] = 0.0f;

}

bool GLMatrix::invertM(float mInv[], int mInvOffset, float m[], int mOffset) {
    // Invert a 4 x 4 matrix using Cramer's Rule

    // transpose matrix
    const float src0 = m[mOffset + 0];
    const float src4 = m[mOffset + 1];
    const float src8 = m[mOffset + 2];
    const float src12 = m[mOffset + 3];

    const float src1 = m[mOffset + 4];
    const float src5 = m[mOffset + 5];
    const float src9 = m[mOffset + 6];
    const float src13 = m[mOffset + 7];

    const float src2 = m[mOffset + 8];
    const float src6 = m[mOffset + 9];
    const float src10 = m[mOffset + 10];
    const float src14 = m[mOffset + 11];

    const float src3 = m[mOffset + 12];
    const float src7 = m[mOffset + 13];
    const float src11 = m[mOffset + 14];
    const float src15 = m[mOffset + 15];

    // calculate pairs for first 8 elements (cofactors)
    const float atmp0 = src10 * src15;
    const float atmp1 = src11 * src14;
    const float atmp2 = src9 * src15;
    const float atmp3 = src11 * src13;
    const float atmp4 = src9 * src14;
    const float atmp5 = src10 * src13;
    const float atmp6 = src8 * src15;
    const float atmp7 = src11 * src12;
    const float atmp8 = src8 * src14;
    const float atmp9 = src10 * src12;
    const float atmp10 = src8 * src13;
    const float atmp11 = src9 * src12;

    // calculate first 8 elements (cofactors)
    const float dst0 = (atmp0 * src5 + atmp3 * src6 + atmp4 * src7)
                       - (atmp1 * src5 + atmp2 * src6 + atmp5 * src7);
    const float dst1 = (atmp1 * src4 + atmp6 * src6 + atmp9 * src7)
                       - (atmp0 * src4 + atmp7 * src6 + atmp8 * src7);
    const float dst2 = (atmp2 * src4 + atmp7 * src5 + atmp10 * src7)
                       - (atmp3 * src4 + atmp6 * src5 + atmp11 * src7);
    const float dst3 = (atmp5 * src4 + atmp8 * src5 + atmp11 * src6)
                       - (atmp4 * src4 + atmp9 * src5 + atmp10 * src6);
    const float dst4 = (atmp1 * src1 + atmp2 * src2 + atmp5 * src3)
                       - (atmp0 * src1 + atmp3 * src2 + atmp4 * src3);
    const float dst5 = (atmp0 * src0 + atmp7 * src2 + atmp8 * src3)
                       - (atmp1 * src0 + atmp6 * src2 + atmp9 * src3);
    const float dst6 = (atmp3 * src0 + atmp6 * src1 + atmp11 * src3)
                       - (atmp2 * src0 + atmp7 * src1 + atmp10 * src3);
    const float dst7 = (atmp4 * src0 + atmp9 * src1 + atmp10 * src2)
                       - (atmp5 * src0 + atmp8 * src1 + atmp11 * src2);

    // calculate pairs for second 8 elements (cofactors)
    const float btmp0 = src2 * src7;
    const float btmp1 = src3 * src6;
    const float btmp2 = src1 * src7;
    const float btmp3 = src3 * src5;
    const float btmp4 = src1 * src6;
    const float btmp5 = src2 * src5;
    const float btmp6 = src0 * src7;
    const float btmp7 = src3 * src4;
    const float btmp8 = src0 * src6;
    const float btmp9 = src2 * src4;
    const float btmp10 = src0 * src5;
    const float btmp11 = src1 * src4;

    // calculate second 8 elements (cofactors)
    const float dst8 = (btmp0 * src13 + btmp3 * src14 + btmp4 * src15)
                       - (btmp1 * src13 + btmp2 * src14 + btmp5 * src15);
    const float dst9 = (btmp1 * src12 + btmp6 * src14 + btmp9 * src15)
                       - (btmp0 * src12 + btmp7 * src14 + btmp8 * src15);
    const float dst10 = (btmp2 * src12 + btmp7 * src13 + btmp10 * src15)
                        - (btmp3 * src12 + btmp6 * src13 + btmp11 * src15);
    const float dst11 = (btmp5 * src12 + btmp8 * src13 + btmp11 * src14)
                        - (btmp4 * src12 + btmp9 * src13 + btmp10 * src14);
    const float dst12 = (btmp2 * src10 + btmp5 * src11 + btmp1 * src9)
                        - (btmp4 * src11 + btmp0 * src9 + btmp3 * src10);
    const float dst13 = (btmp8 * src11 + btmp0 * src8 + btmp7 * src10)
                        - (btmp6 * src10 + btmp9 * src11 + btmp1 * src8);
    const float dst14 = (btmp6 * src9 + btmp11 * src11 + btmp3 * src8)
                        - (btmp10 * src11 + btmp2 * src8 + btmp7 * src9);
    const float dst15 = (btmp10 * src10 + btmp4 * src8 + btmp9 * src9)
                        - (btmp8 * src9 + btmp11 * src10 + btmp5 * src8);

    // calculate determinant
    const float det =
            src0 * dst0 + src1 * dst1 + src2 * dst2 + src3 * dst3;

    if (det == 0.0f) {
        return false;
    }

    // calculate matrix inverse
    const float invdet = 1.0f / det;
    mInv[mInvOffset] = dst0 * invdet;
    mInv[1 + mInvOffset] = dst1 * invdet;
    mInv[2 + mInvOffset] = dst2 * invdet;
    mInv[3 + mInvOffset] = dst3 * invdet;

    mInv[4 + mInvOffset] = dst4 * invdet;
    mInv[5 + mInvOffset] = dst5 * invdet;
    mInv[6 + mInvOffset] = dst6 * invdet;
    mInv[7 + mInvOffset] = dst7 * invdet;

    mInv[8 + mInvOffset] = dst8 * invdet;
    mInv[9 + mInvOffset] = dst9 * invdet;
    mInv[10 + mInvOffset] = dst10 * invdet;
    mInv[11 + mInvOffset] = dst11 * invdet;

    mInv[12 + mInvOffset] = dst12 * invdet;
    mInv[13 + mInvOffset] = dst13 * invdet;
    mInv[14 + mInvOffset] = dst14 * invdet;
    mInv[15 + mInvOffset] = dst15 * invdet;

    return true;
}
