//
// Created by HB on 5/20/2015.
//

#ifndef VECTORTILEMAP_GLMATRIX_H
#define VECTORTILEMAP_GLMATRIX_H


#define MAT_SIZE 16 * sizeof(float)



class GLMatrix {

private:
    float *m;

public:
    GLMatrix();
    ~GLMatrix();

public:
    void set(float m[]);
    void get(float m[]);
    void copy(const GLMatrix &m);
    void prj(float vec3[]);
    void prj3D(float vec3[], int offset, int length);
    void prj2D(float vec2[], int offset, int length);
    void prj2D(float src[], int src_offset, float dst[], int dst_offset, int length);
    void multiplyRhs(const GLMatrix &rhs);
    void multiplyLhs(const GLMatrix &lhs);
    void multiplyMM(const GLMatrix &lhs, const GLMatrix &rhs);
    void transposeM(const GLMatrix &mat);
    void setRotation(float a, float x, float y, float z);
    void setTranslation(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setTransScale(float tx, float ty, float scale);
    void setAsUniform(int location);
    void setValue(int pos, float value);
    void addDepthOffset(int delta);
    void setIdentity();

public:
    static void frustumM(float m[], int offset,
                  float left, float right, float bottom, float top,
                  float near, float far);

    static bool invertM(float mInv[], int mInvOffset, float m[],
                           int mOffset);



};


#endif //VECTORTILEMAP_GLMATRIX_H
