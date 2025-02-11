//
// Created on 11.02.2025.
//

#ifndef WINDOW_HPP
#define WINDOW_HPP

template<typename parent>
class window {
public:
    explicit window(parent& p) : parent_(p) { }
    virtual ~window() = default;

    // empty and not = 0, for polymorphic typeid usage
    virtual void prepareEnvironmentOnce() { };
    virtual void prepareEnvironment() { };

    virtual void ui() { };
    virtual void handleInput() { };
    virtual void update(float dt) { };
    virtual void render() { };

    virtual void cleanEnvironment() { };
    virtual void cleanEnvironmentOnce() { };

protected:
    parent& parent_;
};



#endif //WINDOW_HPP
