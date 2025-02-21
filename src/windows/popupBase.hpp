//
// Created by rdkgs on 2/20/2025.
//

#ifndef POPUPBASE_HPP
#define POPUPBASE_HPP

/* CRTP */
template<typename T>
class popupBase {
public:
    void show() {
        static_cast<T*>(this)->show(); /* run implementation from derived class */
    }
};

#endif //POPUPBASE_HPP
