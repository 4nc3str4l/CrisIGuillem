
#include "Common.h"

namespace Common{

    bool wireframeView = false;

    void changeViewMode(){
        wireframeView = !wireframeView;
    }

    unsigned int getWireframeView(){
        return wireframeView  ? GL_LINE : GL_FILL;
    }
}

