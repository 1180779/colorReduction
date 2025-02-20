//
// Created by rdkgs on 2/20/2025.
//

#ifndef MAT_HPP
#define MAT_HPP

#include <cstring>
#include <vector>

class mat {
public:
    mat() = default;
    void set(int n, int m, float *data);

    int getN() const { return n; }
    int getM() const { return m; }
    const float* getData() const { return data.data(); }

private:
    int n = 0, m = 0; /* matrix dimensions */
    std::vector<float> data; /* matrix data */
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

inline void mat::set(int n, int m, float *data) {
    this->n = n;
    this->m = m;
    this->data.resize(n * m);
    memcpy(this->data.data(), data, n * m * sizeof(float));
}

#endif //MAT_HPP
