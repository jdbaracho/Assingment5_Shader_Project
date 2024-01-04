////////////////////////////////////////////////////////////////////////////////
//
// Key Buffer Class
//
// by Jo�o Baracho
//
////////////////////////////////////////////////////////////////////////////////

#include "mglKeyBuffer.hpp"

namespace mgl {

	///////////////////////////////////////////////////////////////////// Key Buffer

    KeyBuffer::KeyBuffer() {}

    KeyBuffer::~KeyBuffer() {}

    KeyBuffer& KeyBuffer::getInstance() {
        static KeyBuffer instance;
        return instance;
    }

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl