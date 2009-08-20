//---------------------------------------------
//                Initial Engine
//             Color  source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/ILogger.h"
#include "Initial/3D/Loader/I3DLoad3DS.h"
#include <lib3ds.h>

using namespace Initial;
using namespace Initial::Core;
using namespace Initial::Video;

IMesh* I3DLoad::Load(IString filename, IRenderDriver* device)
{
	Lib3dsFile *f;

    f = lib3ds_file_open(filename.c_str());
	if (!f)
		f = lib3ds_file_open(("Models/"+filename).c_str());

	if (f)
	{
		ILogger::LogMessage("3DS file found\n");
		ILogger::LogMessage("meshes count %d\n",f->nmeshes);

		IMesh *obj = new IMesh(device);
		for (unsigned int i=0;i<f->nmeshes;i++)
		{
			ITriangle tri;
			IMesh *child = new IMesh(device,obj);
			
			//f->meshes[i]->faces[0].material
			if (f->meshes[i]->nfaces && f->nmaterials)
			{
				float *col = f->materials[f->meshes[i]->faces[0].material]->diffuse;

				child->SetColor(IColor(col[0],col[1],col[2]));
				/*printf("tex1 %s\n",f->materials[f->meshes[i]->faces[0].material]->texture1_map.name);
				printf("tex2 %s\n",f->materials[f->meshes[i]->faces[0].material]->texture2_map.name);
				printf("tex3 %s\n",f->materials[f->meshes[i]->faces[0].material]->texture1_mask.name);
				printf("tex4 %s\n",f->materials[f->meshes[i]->faces[0].material]->texture2_mask.name);
				printf("tex4 %s\n",f->materials[f->meshes[i]->faces[0].material]->bump_map.name);*/

				/*child->SetMaterial(IMaterial::LoadMaterialFromTexture(device,
						f->materials[f->meshes[i]->faces[0].material]->texture1_map.name));*/
			}

			//Force le lissage
			for (unsigned int j=0;j<f->meshes[i]->nfaces;j++)
				f->meshes[i]->faces[j].smoothing_group=1;

			//float (*normals)[3] = (float(*)[3])malloc(sizeof(float) * 9 * f->meshes[i]->nfaces);
			//lib3ds_mesh_calculate_face_normals(f->meshes[i], normals);
			float (*normalsV)[3] = (float(*)[3])malloc(3 * sizeof(float) * 9 * f->meshes[i]->nfaces);
			lib3ds_mesh_calculate_vertex_normals(f->meshes[i], normalsV);

			for (unsigned int j=0;j<f->meshes[i]->nfaces;j++)
			{
				/*tri.SetNormal(0,IVector3D(normals[j][0],normals[j][1],normals[j][2]));
				tri.SetNormal(1,IVector3D(normals[j][0],normals[j][1],normals[j][2]));
				tri.SetNormal(2,IVector3D(normals[j][0],normals[j][1],normals[j][2]));*/

				tri.SetNormal(0,IVector3D(normalsV[3*j][0],normalsV[3*j][1],normalsV[3*j][2]));
				tri.SetNormal(1,IVector3D(normalsV[3*j+1][0],normalsV[3*j+1][1],normalsV[3*j+1][2]));
				tri.SetNormal(2,IVector3D(normalsV[3*j+2][0],normalsV[3*j+2][1],normalsV[3*j+2][2]));

				float *pos;
				float *tex=NULL;
				pos = f->meshes[i]->vertices[f->meshes[i]->faces[j].index[0]];
				if (f->meshes[i]->texcos)
					tex = f->meshes[i]->texcos[f->meshes[i]->faces[j].index[0]];
				tri.SetVertex(0,IVector3D(pos[0],pos[1],pos[2]));
				if (tex)
					tri.SetCoord(0,IVector2D(tex[0],tex[1]));

				tex=NULL;
				pos = f->meshes[i]->vertices[f->meshes[i]->faces[j].index[1]];
				if (f->meshes[i]->texcos)
					tex = f->meshes[i]->texcos[f->meshes[i]->faces[j].index[1]];
				tri.SetVertex(1,IVector3D(pos[0],pos[1],pos[2]));
				if (tex)
					tri.SetCoord(1,IVector2D(tex[0],tex[1]));

				tex=NULL;
				pos = f->meshes[i]->vertices[f->meshes[i]->faces[j].index[2]];
				if (f->meshes[i]->texcos)
					tex = f->meshes[i]->texcos[f->meshes[i]->faces[j].index[2]];
				tri.SetVertex(2,IVector3D(pos[0],pos[1],pos[2]));
				if (tex)
					tri.SetCoord(2,IVector2D(tex[0],tex[1]));

				child->AddPolygon(tri);
			}	
			//free(normals);
			free(normalsV);
			obj->AddChild(child);
		}
		ILogger::LogMessage("Calculing bounding box\n");
		obj->CalculateBBox(true);
		ILogger::LogMessage("3DS loading finish\n");
		return obj;
	}
	ILogger::LogError("3DS file not found\n");
	return NULL;
}
