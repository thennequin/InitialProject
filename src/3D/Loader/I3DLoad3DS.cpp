//---------------------------------------------
//                Initial Engine
//             Color  source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "3D/Loader/I3DLoad3DS.h"
#include <lib3ds.h>

using namespace Initial;

I3DObject* I3DLoad3DS::Load3DS(IString filename, IRenderDriver* device)
{
	/*_3DSObject *objet3d = new _3DSObject;
	objet3d->num_mesh=0;
	//objet3d->mesh = (_3DSMesh*) malloc(sizeof(_3DSMesh)*100);
	objet3d->mesh=NULL;

	unsigned short chunk_id;
	unsigned int chunk_lenght;

	FILE *fichier = fopen(filename,"rb");
	if (fichier==NULL)
	{
		printf("Erreur de lecture du fichier\n");
		return NULL;
	}

	fread (&chunk_id, 2, 1, fichier);
	fread (&chunk_lenght, 4, 1, fichier);

	switch(chunk_id)
	{
	case 0x4d4d:
	  MainChunk(objet3d,fichier,chunk_lenght-6);
	  break;
	default:
		printf("Le fichier n'est pas un 3ds\n");
		fclose(fichier);
		return NULL;
	}

	//Creation de la class I3DObject
	I3DObject *obj = new I3DObject(device);
	if (obj)
	{
		I3DObject *child;
		//IArray<ITriangle> Polys;

		printf("Nombre de mesh %d\n",objet3d->num_mesh);
		for (int i=0;i<objet3d->num_mesh;i++)
		{		
			IMaterial *mat;
			if (device)
			{
				mat = IMaterial::LoadMaterial(device,objet3d->mesh[i].material_name);
				if (mat==NULL)
					mat = IMaterial::LoadMaterialFromTexture(device,objet3d->mesh[i].material_name);
			}else mat = NULL;
			//Polys.Clear();
			child = new I3DObject(device,obj);
			printf("Mapping face %d\n",objet3d->mesh[i].num_mapping_face);
			printf("Face %d\n",objet3d->mesh[i].num_faces);
			for (int j=0;j<objet3d->mesh[i].num_faces;j++)
			{
				ITriangle *pol = new ITriangle;

				ICoord coord;
				IVertex vert;
				//printf("Mesh %d\n",objet3d->mesh[i]);
				for (int k=0;k<3;k++)
				{
					int id;
					if (k==0)
						id=objet3d->mesh[i].faces[j].a;
					else if (k==1)
						id=objet3d->mesh[i].faces[j].b;
					else
						id=objet3d->mesh[i].faces[j].c;
					
					coord.SetU(objet3d->mesh[i].mapping_faces[id].u);
					coord.SetV(objet3d->mesh[i].mapping_faces[id].v);
					pol->SetCoord(k,coord);

					vert.SetX(objet3d->mesh[i].vectrices[id].x);
					vert.SetY(objet3d->mesh[i].vectrices[id].y);
					vert.SetZ(objet3d->mesh[i].vectrices[id].z);
					pol->SetVertex(k,vert);
				}

				//objet3d->mesh[i].material_name[objet3d->mesh[i].material_face];
				//pol->SetMaterial(mat);
				child->SetMaterial(mat);
				//Polys.Add(pol);
				child->AddPolygon(pol);
			}
		

			obj->AddChild(child);
		}

		obj->CalculateBBox(true);
		return obj;
	}
	return NULL;*/

	//----------------------------------------------------------------------------------

	Lib3dsFile *f;

    f = lib3ds_file_open(filename);
	if (f)
	{
		printf("3DS file found\n");
		printf("meshes count %d\n",f->nmeshes);

		I3DObject *obj = new I3DObject(device);
		for (unsigned int i=0;i<f->nmeshes;i++)
		{
			ITriangle tri;
			I3DObject *child = new I3DObject(device,obj);
			
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

			for (unsigned int j=0;j<f->meshes[i]->nfaces;j++)
			{
				float *pos;
				float *tex=NULL;
				pos = f->meshes[i]->vertices[f->meshes[i]->faces[j].index[0]];
				if (f->meshes[i]->texcos)
					tex = f->meshes[i]->texcos[f->meshes[i]->faces[j].index[0]];
				tri.SetVertex(0,IVertex(pos[0],pos[1],pos[2]));
				if (tex)
					tri.SetCoord(0,ICoord(tex[0],tex[1]));

				pos = f->meshes[i]->vertices[f->meshes[i]->faces[j].index[1]];
				if (f->meshes[i]->texcos)
					tex = f->meshes[i]->texcos[f->meshes[i]->faces[j].index[1]];
				tri.SetVertex(1,IVertex(pos[0],pos[1],pos[2]));
				if (tex)
					tri.SetCoord(1,ICoord(tex[0],tex[1]));

				pos = f->meshes[i]->vertices[f->meshes[i]->faces[j].index[2]];
				if (f->meshes[i]->texcos)
					tex = f->meshes[i]->texcos[f->meshes[i]->faces[j].index[2]];
				tri.SetVertex(2,IVertex(pos[0],pos[1],pos[2]));
				if (tex)
					tri.SetCoord(2,ICoord(tex[0],tex[1]));
				child->AddPolygon(tri);
			}	
			obj->AddChild(child);
		}
		printf("Calculing bounding box\n");
		obj->CalculateBBox(true);
		printf("3DS loading finish\n");
		return obj;
	}

	return NULL;
}

int I3DLoad3DS::MainChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght)
{
	int start = ftell(fichier);

	unsigned short chunk_id;
	unsigned int chunk_lenght;

	while (ftell(fichier) < start + lenght)
	{
		fread (&chunk_id, 2, 1, fichier);
		fread (&chunk_lenght, 4, 1, fichier);

		switch(chunk_id)
		{
		case 0x3d3d:
			EditorChunk(objet3d, fichier, chunk_lenght-6);
			break;
		case 0xb000:
			//KeyframerChunk(objet3d,fichier, chunk_lenght-6);
			fseek(fichier, chunk_lenght-6, SEEK_CUR);
			break;
		default:
			fseek(fichier, chunk_lenght-6, SEEK_CUR);
		}
	}
	fseek(fichier, start+lenght, SEEK_SET);
	return 1;
}


int I3DLoad3DS::EditorChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght)
{
	int start = ftell(fichier);
	unsigned short chunk_id;
	unsigned int chunk_lenght;
	while (ftell(fichier) < start + lenght)
	{
		fread (&chunk_id, 2, 1, fichier);
		fread (&chunk_lenght, 4, 1, fichier); 
		switch(chunk_id)
		{
		case 0x4000:
			int num,i;
			objet3d->num_mesh++;
			objet3d->mesh=(_3DSMesh*)realloc(objet3d->mesh,sizeof(_3DSMesh)*objet3d->num_mesh);
			num=objet3d->num_mesh-1;
			char temp;
			i = 0;
			do
			{
				fread (&temp, 1, 1, fichier);
				objet3d->mesh[num].name[i] = temp;
				i++;
			}while(temp != '\0');
			ObjetBlockChunk(objet3d,fichier, chunk_lenght-6-i);
			break;
		case 0xAFFF:
			//MaterialBlocChunk(objet3d, fichier, chunk_lenght-6);
			fseek(fichier, chunk_lenght-6, SEEK_CUR);
			break;
		default:
			fseek(fichier, chunk_lenght-6, SEEK_CUR);
		}
	}
	fseek(fichier, start+lenght, SEEK_SET);
	return 1;
}

int I3DLoad3DS::ObjetBlockChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght)
{
	int start = ftell(fichier);
	unsigned short chunk_id;
	unsigned int chunk_lenght;
	while (ftell(fichier) < start + lenght)
	{
		fread (&chunk_id, 2, 1, fichier);
		fread (&chunk_lenght, 4, 1, fichier); 
		switch(chunk_id)
		{
		case 0x4100:
			TriangularMeshChunk(objet3d, fichier, chunk_lenght-6);
			break;
		default:
			fseek(fichier, chunk_lenght-6, SEEK_CUR);
		}
	}
	fseek(fichier, start+lenght, SEEK_SET);
	return 1;
}

int I3DLoad3DS::TriangularMeshChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght)
{
	int start = ftell(fichier);
	unsigned short chunk_id;
	unsigned int chunk_lenght;
	while (ftell(fichier) < start + lenght)
	{
		fread (&chunk_id, 2, 1, fichier);
		fread (&chunk_lenght, 4, 1, fichier); 
		int i;
		int num;
		switch(chunk_id)
		{
		case 0x4110:			
			num=objet3d->num_mesh-1;
			fread(&objet3d->mesh[num].num_vectrices, sizeof (unsigned short), 1, fichier);
			objet3d->mesh[num].vectrices = (_3DSVertex*)malloc(sizeof(_3DSVertex)*objet3d->mesh[num].num_vectrices);
			for (i=0; i<objet3d->mesh[num].num_vectrices; i++)
			{
				fread (&objet3d->mesh[num].vectrices[i].x, sizeof(float), 1, fichier);
				fread (&objet3d->mesh[num].vectrices[i].y, sizeof(float), 1, fichier);
				fread (&objet3d->mesh[num].vectrices[i].z, sizeof(float), 1, fichier);
			}
			break;
		case 0x4120:
			num=objet3d->num_mesh-1;
			fread(&objet3d->mesh[num].num_faces, sizeof (unsigned short), 1, fichier);
			printf("Faces %d\n",objet3d->mesh[num].num_faces);
			objet3d->mesh[num].faces = (_3DSFace*)malloc(sizeof(_3DSFace)*objet3d->mesh[num].num_faces);
			unsigned short flag;
			for (i=0; i<objet3d->mesh[num].num_faces; i++)
			{
				fread (&objet3d->mesh[num].faces[i].a, sizeof (unsigned short), 1, fichier);
				fread (&objet3d->mesh[num].faces[i].b, sizeof (unsigned short), 1, fichier);
				fread (&objet3d->mesh[num].faces[i].c, sizeof (unsigned short), 1, fichier);
				fread (&flag, sizeof (unsigned short), 1, fichier);
			}
			FaceDescriptionChunk(objet3d,fichier,chunk_lenght-6-8*objet3d->mesh[num].num_faces-2);
			break;
		case 0x4140:
			num=objet3d->num_mesh-1;
			fread(&objet3d->mesh[num].num_mapping_face, sizeof (unsigned short), 1, fichier);
			printf("Mapping faces %d\n",objet3d->mesh[num].num_mapping_face);
			objet3d->mesh[num].mapping_faces = (_3DSUV*)malloc(sizeof(_3DSUV)*objet3d->mesh[num].num_mapping_face);
			for (i=0; i<objet3d->mesh[num].num_mapping_face; i++)
			{
				fread (&objet3d->mesh[num].mapping_faces[i].u, sizeof (float), 1, fichier);
				fread (&objet3d->mesh[num].mapping_faces[i].v, sizeof (float), 1, fichier);
			}
			break;
		default:  
          fseek(fichier, chunk_lenght-6, SEEK_CUR);
		}
	}
	fseek(fichier, start+lenght, SEEK_SET);
	return 1;
}

int I3DLoad3DS::FaceDescriptionChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght)
{
	int start = ftell(fichier);
	unsigned short chunk_id;
	unsigned int chunk_lenght;
	while (ftell(fichier) < start + lenght)
	{
		fread (&chunk_id, 2, 1, fichier);
		fread (&chunk_lenght, 4, 1, fichier); 
		int num,i;
		switch(chunk_id)
		{
		case 0x4130 : 
			num=objet3d->num_mesh-1;
			char temp;
			i = 0;
			do
			{
				fread (&temp, 1, 1, fichier);
				objet3d->mesh[num].material_name[i] = temp;
				i++;
			}while(temp != '\0');
			fread (&objet3d->mesh[num].num_material_face, 2, 1, fichier);
			objet3d->mesh[num].material_face = (unsigned short*)malloc(sizeof(unsigned short)*objet3d->mesh[num].num_material_face);
			for(i=0;i<objet3d->mesh[num].num_material_face;i++)
				fread (&objet3d->mesh[num].material_face[i], sizeof (unsigned short), 1, fichier);
			break;
		default:
			fseek(fichier, chunk_lenght-6, SEEK_CUR);
		}
	}
	fseek(fichier, start+lenght, SEEK_SET);
	return 1;

}