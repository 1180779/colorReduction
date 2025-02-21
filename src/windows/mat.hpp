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

    [[nodiscard]] int getN() const { return n; }
    [[nodiscard]] int getM() const { return m; }
    [[nodiscard]] const float* getData() const { return data.data(); }
    float* getData() { return data.data(); }

    void clear();
    float getSum() const;
    void normalize();

    void addEmptyRow();
    void addEmptyColumn();

    void removeLastRow();
    void removeLastColumn();

    bool emptyRow(int k);
    bool emptyColumn(int k);
    bool emptyLastRow(int k) { return emptyRow(getN() - 1); }
    bool emptyLastColumn(int k) { return emptyColumn(getM() - 1); }

    void resize(int n, int m);

private:
    int n = 0, m = 0; /* matrix dimensions */
    std::vector<float> data; /* matrix data */
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

inline void mat::set(int n, int m, float *data)
{
    this->n = n;
    this->m = m;
    this->data.resize(n * m);
    memcpy(this->data.data(), data, n * m * sizeof(float));
}

inline bool mat::emptyColumn(int k)
{
    if (k < 0 || k >= m)
        return false;
    for (int i = k; i < n * m; i += m) {
        if (data[i] != 0.0f)
            return false;
    }
    return true;
}

inline void mat::resize(int n, int m)
{
    if (this->n <= n && this->m <= m) {
        this->n = n;
        this->m = m;
        this->data.resize(n * m);
    }

    this->n = n;
    this->m = m;
    this->data.resize(n * m);
    memset(this->data.data(), 0, n * m * sizeof(float));
}

inline void mat::clear()
{
    memset(data.data(), 0, n * m * sizeof(float));
}

inline float mat::getSum() const
{
    float sum = 0.0f;
    for (auto item : data)
        sum += item;
    return sum;
}

inline void mat::normalize()
{
    float sum = getSum();
    if (sum == 0.0f)
        return;
    for (auto& item : data)
        item /= sum;
}

inline void mat::addEmptyRow()
{
    data.resize((n + 1) * m);
    memset(data.data() + n * m, 0, m * sizeof(float));
    ++n;
}

inline void mat::addEmptyColumn()
{
    data.resize(n * (m + 1));
    memset(data.data() + n * m, 0, n * sizeof(float));
    for (int i = n - 1; i >= 0; --i) {
        memmove(data.data() + i * (m + 1), data.data() + i * m, m * sizeof(float));
        data[i * (m + 1) + m] = 0.0f;
    }
    ++m;
}

inline void mat::removeLastRow()
{
    if (n == 1)
        return;
    data.resize((n - 1) * m);
    --n;
}

inline void mat::removeLastColumn()
{
    if (m == 1)
        return;
    for (int i = 0; i < n; ++i) {
        memmove(data.data() + i * (m - 1), data.data() + i * m, (m - 1) * sizeof(float));
    }
    data.resize(n * (m - 1));
    --m;
}

inline bool mat::emptyRow(int k)
{
    if (k < 0 || k >= n)
        return false;
    for (int i = k * n; i < k * (n + 1); ++i) {
        if (data[i] != 0.0f)
            return false;
    }
    return true;
}

#endif //MAT_HPP
