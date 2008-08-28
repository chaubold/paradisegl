#include "ObjModel.h"


ObjModel::ObjModel(const std::string& _file)
{
	mp_vbo = NULL;
	loadObjModel(_file);
}


ObjModel::~ObjModel(){
	delete mp_vbo;
}


ObjModel::Material* ObjModel::getMaterial(const std::string& name)
{
    if( mc_materials.find(name) != mc_materials.end())
    {
        return &mc_materials[name];
    }
    else
        return NULL;
}

void ObjModel::DrawModel(bool withTexture, Shader* shader){
    if(withTexture)
    {
        for(unsigned int i = 0; i < mc_groups.size(); i++)
        {
            if(mc_materials[mc_groups.at(i).material].texID >= 0)
            {
                //apply material
                glActiveTexture(GL_TEXTURE0);
                glEnable(GL_TEXTURE_2D);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glBindTexture(GL_TEXTURE_2D,mc_materials[mc_groups.at(i).material].texID);
                shader->sendUniform("tex0", 0);
            }
            else
                std::cout << "Texture ID < 0" << std::endl;
            mp_vbo->Draw(mc_groups.at(i).start*3, mc_groups.at(i).end*3, withTexture);
        }
    }
    else mp_vbo->Draw(withTexture);
}

bool ObjModel::loadMtlLib(const std::string& _file)
{
	try{
		std::ifstream mtl_file(_file.c_str());

		if (mtl_file.fail()){
			std::cerr << "ObjModel: wasn't able to load mtl_lib file " << _file << std::endl;
			return false;
		}
		else
        {
            std::cout << "ObjModel: Loading " << _file << std::endl;
        }
		//Initial values for default material
		Material currMat;
		kmVec3 d;
		kmVec3Fill(&d, 1.0f, 1.0f, 1.0f);
		currMat.diffuse = d;
		currMat.ambient = d;
		currMat.specular = d;
		currMat.texID = -1;

		std::string mat_name = "default";

		enum states{start = 1, read_newmtl, have_K, read_Kd, read_Ka, read_Ks, read_map_Kd};
		enum states currState = start;

		while (mtl_file)
		{
			switch (currState)
			{
				case start:
					{
						char c;
						mtl_file >> c;
						switch (c)
						{
							case 'K': currState = have_K; break;
							case 'n': currState = read_newmtl; break;
							case 'm': currState = read_map_Kd; break;
							default: std::string temp; getline(mtl_file, temp); break;
						}
					} break;

				case have_K:
					{
						char c;
						mtl_file >> c;
						switch (c)
						{
							case 'a': currState = read_Ka; break;
							case 'd': currState = read_Kd; break;
							case 's': currState = read_Ks; break;
							default: std::string temp; getline(mtl_file, temp); break;
						}
					} break;
				case read_Ka:
					{
						kmVec3 vec;
						mtl_file >> vec.x;
						mtl_file >> vec.y;
						mtl_file >> vec.z;
						currMat.ambient = vec;
						currState = start;
					} break;
				case read_Kd:
					{
						kmVec3 vec;
						mtl_file >> vec.x;
						mtl_file >> vec.y;
						mtl_file >> vec.z;
						currMat.diffuse = vec;
						currState = start;
					} break;
				case read_Ks:
					{
						kmVec3 vec;
						mtl_file >> vec.x;
						mtl_file >> vec.y;
						mtl_file >> vec.z;
						currMat.specular = vec;
						currState = start;
					} break;
				case read_map_Kd:
					{
						std::string line;
						getline(mtl_file, line);
						std::string::size_type pos = line.find("ap_Kd", 0);
						if (pos == std::string::npos)
						{
							std::cerr << "pos = npos" << std::endl;
						}
						else
						{
							currMat.texture = line.substr(pos + 6);
							currMat.texture.erase(currMat.texture.end() - 1);
							std::cout << "Found texture " << currMat.texture << std::endl;
						}
						currState = start;
					} break;
				case read_newmtl:
					{
						std::string line;
						getline(mtl_file, line);
						std::string::size_type pos = line.find("ewmtl", 0);
						if (pos == std::string::npos)
						{
							std::cerr << "pos = npos" << std::endl;
						}
						else
						{
							//push back old material
							mc_materials[mat_name] = currMat;

							mat_name = line.substr(pos + 6);
							mat_name.erase(mat_name.end() - 1);
							std::cout << "Found new Material " << mat_name << std::endl;
						}
						currState = start;
					} break;
                default:
                    currState = start;
                break;
			}
		}
		mc_materials[mat_name] = currMat;


		//Output all materials here for debugging
		for(std::map<std::string, Material>::iterator i = mc_materials.begin(); i != mc_materials.end(); ++i)
		{
		    std::cout << "Material: " << i->first << std::endl;
		    std::cout << "\tTexture: " << i->second.texture << std::endl;
		    //load texture:
            if(!i->second.texture.empty())
		    {
		        sf::Image texImage;
                texImage.LoadFromFile(i->second.texture);
                GLuint id;
                glGenTextures(1, &id);
                glBindTexture(GL_TEXTURE_2D, id);

                //flip image data;
                const sf::Uint8* imageData = texImage.GetPixelsPtr();
                sf::Uint8* flippedImageData = new sf::Uint8[texImage.GetHeight()*texImage.GetWidth()*4];

                for(unsigned int j=0; j < texImage.GetHeight(); j++)
                {
                    memcpy(&flippedImageData[j*texImage.GetWidth()*4], &imageData[(texImage.GetHeight() - j - 1)*texImage.GetWidth()*4], texImage.GetWidth()*4*sizeof(sf::Uint8));
                }

                gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texImage.GetWidth(), texImage.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, flippedImageData);
                delete flippedImageData;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                i->second.texID = static_cast<int>(id);
		    }
		    std::cout << "\tTexID: " << i->second.texID << std::endl;
		    std::cout << "\tDiffuse: " << i->second.diffuse.x << " " << i->second.diffuse.y << " " << i->second.diffuse.z << std::endl;
		    std::cout << "\tAmbient: " << i->second.ambient.x << " " << i->second.ambient.y << " " << i->second.ambient.z << std::endl;
		    std::cout << "\tSpecular: " << i->second.specular.x << " " << i->second.specular.y << " " << i->second.specular.z << std::endl;
		}

		mtl_file.close();
		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in ObjModel::loadMtlLib =" << e.what() << std::endl;
		return false;
	}
}

bool ObjModel::loadObjModel(const std::string& _file)
{
	std::ifstream modelfile(_file.c_str());

	if (modelfile.fail()){
		std::cerr << "ObjModel: wasn't able to load file " << _file << std::endl;
		return false;
	}
	else
	{
	    std::cout << "ObjModel: Loading " << _file << std::endl;
	}

	enum states{start = 1, have_v, read_vt, read_vn, read_f, read_usemtl, read_mtllib};
	enum states currState = start;
	while (modelfile){
		switch (currState){
			case start:
				{
					char c;
					string temp;
					modelfile >> c;
					switch (c){
						case 'v': currState = have_v; break;//go to differenciation between v, vt and vn
						case 'f': currState = read_f; break;//read face
						case 'm': currState = read_mtllib; break;//read in a materiallib
						case 'u': currState = read_usemtl; break;//use the specified material for this group
						default: getline(modelfile, temp); break;//line which doesnt interest ous
					}
				}
				break;

			case have_v:
				{
					char c;
					modelfile.get(c);
					switch (c)
					{
						case 't': currState = read_vt; break;
						case 'n': currState = read_vn; break;
						default: //Read Vertex
							kmVec3 vec;
							modelfile >> vec.x;
							modelfile >> vec.y;
							modelfile >> vec.z;
							map_vertices[map_vertices.size()] = vec;
							//std::cout << "Found Vertex " << vec.x << " " << vec.y << " " << vec.z << std::endl;
							currState = start;
							break;
					}
				}
				break;

			case read_vt: //Read Texture Coordinate
				{
					kmVec2 vec;
					modelfile >> vec.x;
					modelfile >> vec.y;
					map_texCoords[map_texCoords.size()] = vec;
					currState = start;
				}
				break;

			case read_vn: //Read Vertex Normal
				{
					kmVec3 vec;
					modelfile >> vec.x;
					modelfile >> vec.y;
					modelfile >> vec.z;
					map_normals[map_normals.size()] = vec;
					currState = start;
				}
				break;

			case read_f: //Read triangle
				{
					char c = '/';
					ModelFace f;

					modelfile >> f.vertices[0];
					modelfile.get(c);// a slash
					modelfile >> f.texCoords[0];
					modelfile.get(c);// a slash
					modelfile >> f.normals[0];

					modelfile.get(c);// is a space

					modelfile >> f.vertices[1];
					modelfile.get(c);// a slash
					modelfile >> f.texCoords[1];
					modelfile.get(c);// a slash
					modelfile >> f.normals[1];

					modelfile.get(c);// is a space

					modelfile >> f.vertices[2];
					modelfile.get(c);// a slash
					modelfile >> f.texCoords[2];
					modelfile.get(c);// a slash
					modelfile >> f.normals[2];
					list_faces.push_back(f);
					currState = start;
				}
				break;

			case read_mtllib:
				{
					std::string line;
					getline(modelfile, line);
					std::string::size_type pos = line.find("tllib", 0);
					if (pos == std::string::npos)
					{
						std::cerr << "pos = npos" << std::endl;
					}
					else
					{
						//push back old material
						std::string mtl_lib = line.substr(pos + 6);
						mtl_lib.erase(mtl_lib.end() - 1);
						std::cout << "Found MaterialLib " << mtl_lib << std::endl;
						if(!loadMtlLib(mtl_lib))
                            return false;
					}
					currState = start;
				}; break;
            case read_usemtl:
				{
					std::string line;
					getline(modelfile, line);
					std::string::size_type pos = line.find("semtl", 0);
					if (pos == std::string::npos)
					{
						std::cerr << "Read Usemtl: pos = npos" << std::endl;
						currState = start;
					}
					else
					{
					    if (!mc_groups.empty())
                        {
                            mc_groups.back().end = list_faces.size();
                        }
                        Group g;
                        g.start = list_faces.size();
                        std::string usemtl = line.substr(pos + 6);
                        usemtl.erase(usemtl.end() - 1);
						std::cout << "Found UseMtl " << usemtl << std::endl;
						g.material = usemtl;
                        mc_groups.push_back(g);
                        string temp;
                        getline(modelfile, temp);
                        currState = start;
					}
				}; break;
			default:
                currState = start;
				break;
		}
	}
	if (!mc_groups.empty())
	{
		mc_groups.back().end = list_faces.size();
	}
	std::cout << "MOModel: finished loading mesh data" << std::endl;

	mp_vbo = new VBO();
	for (std::vector<ModelFace>::iterator i = list_faces.begin();i != list_faces.end();++i)
		mp_vbo->AddTriangle(&map_vertices[(*i).vertices[0] - 1],
		                    &map_normals[(*i).normals[0] - 1],
		                    &map_texCoords[(*i).texCoords[0] - 1],

		                    &map_vertices[(*i).vertices[1] - 1],
		                    &map_normals[(*i).normals[1] - 1],
		                    &map_texCoords[(*i).texCoords[1] - 1],

		                    &map_vertices[(*i).vertices[2] - 1],
		                    &map_normals[(*i).normals[2] - 1],
		                    &map_texCoords[(*i).texCoords[2] - 1]
		                   );
	mp_vbo->Upload();

	return true;
}
