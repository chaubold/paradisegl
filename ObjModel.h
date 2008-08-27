#ifndef _MO_MODEL
#define _MO_MODEL

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <kazmath/vec2.h>
#include <kazmath/vec3.h>

#include "VBO.h"
#include "Shader.h"

class ObjModel
{
	public:
		typedef struct ModelFace
		{
			int vertices[3];
			int texCoords[3];
			int normals[3];
		};

		typedef struct Material
		{
			std::string texture;
			int texID;
			kmVec3 diffuse;
			kmVec3 ambient;
			kmVec3 specular;
		};

		typedef struct Group
		{
			int start;
			int end;
			std::string material;
		};

	private:

		std::map<int, kmVec3> map_vertices;
		std::map<int, kmVec2> map_texCoords;
		std::map<int, kmVec3> map_normals;
		std::map<std::string, Material> mc_materials;
		std::vector<ModelFace> list_faces;
		std::vector<Group> mc_groups;

		VBO *mp_vbo;

		bool loadObjModel(const std::string& _file);
		bool loadMtlLib(const std::string& _file);

	public:
		ObjModel(const std::string& _file);
		~ObjModel();

		void DrawModel(bool withTexture, Shader* shader);
		Material* getMaterial(const std::string& name);
};


#endif

