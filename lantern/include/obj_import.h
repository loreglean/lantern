#ifndef LANTERN_OBJ_IMPORT_H
#define LANTERN_OBJ_IMPORT_H

#include <string>
#include <memory>
#include "mesh.h"

namespace lantern
{
	std::shared_ptr<mesh const> import_obj_mesh(std::string path);
}

#endif // LANTERN_OBJ_IMPORT_H
