#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "rigid_body.hpp"
#include <iostream>

class ship : public rigid_body {
    private:
        float m_movespeed, m_anglespeed;
    public:
        ~ship() {}
};

#endif