//---------------------------------------------
//                Initial Engine
//               3DS Loader Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _I3DLOAD3DS_HEADER_
#define _I3DLOAD3DS_HEADER_

#include "Core/IString.h"
#include "3D/I3DObject.h"
#include "Video/IRenderDriver.h"

namespace Initial
{
	class I3DLoad3DS /*: public I3DObject*/
	{
		typedef struct _3DSVertex
		{
			float x,y,z;
		}_3DSVertex;

		typedef struct _3DSFace
		{
			unsigned short a,b,c;
		}_3DSFace;

		typedef struct _3DSUV
		{
			float u,v;
		}_3DSUV;

		typedef struct _3DSMesh
		{
			char name[20];
			unsigned short num_vectrices;
			_3DSVertex *vectrices;
			unsigned short num_faces;
			_3DSFace *faces;
			unsigned short num_mapping_face;
			_3DSUV *mapping_faces;
			char material_name[20];
			unsigned short num_material_face;
			unsigned short *material_face;
		}_3DSMesh;

		typedef struct _3DSObject
		{
			int num_mesh;
			_3DSMesh *mesh;
		}_3DSObject;

		static int MainChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght);
		static int EditorChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght);
		static int ObjetBlockChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght);
		static int TriangularMeshChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght);
		static int FaceDescriptionChunk(_3DSObject *objet3d, FILE *fichier, unsigned int lenght);

	public:
		static I3DObject* Load3DS(IString filename,IRenderDriver *device);	
	};

}

#endif