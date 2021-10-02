#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Glew/glew.h>
#include <Glm/glm.hpp>

#include "shader.h"

#include <iostream>
#include <map>

namespace Minicube {
	namespace text {
		void init();
		void setProjection(glm::mat4 projection);
		void draw2DText(const std::string& string, int x, int y);
	}
}