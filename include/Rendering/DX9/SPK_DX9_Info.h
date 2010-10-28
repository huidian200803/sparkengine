//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2009-2010 - foulon matthieu - stardeath@wanadoo.fr				//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////

#ifndef H_SPK_DX9INFO
#define H_SPK_DX9INFO

//#include "Rendering/DX9/SPK_DX9_DEF.h"

namespace SPK
{
	namespace DX9
	{
		// vertex element for DX9QuadRenderer
		const D3DVERTEXELEMENT9 Decl[4][4] =
		{
			{
				{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				D3DDECL_END()
			},
			{},
			{
				{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				{2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				D3DDECL_END()
			},
			{
				{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{1, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				{2, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				D3DDECL_END()
			}
		};

		class DX9Renderer;

		class SPK_DX9_PREFIX DX9Info
		{
		private:
			static LPDIRECT3DDEVICE9 device;
			static LPDIRECT3DVERTEXDECLARATION9 decl[4];

		public:
			inline static LPDIRECT3DDEVICE9 getDevice();

			inline static LPDIRECT3DVERTEXDECLARATION9 getDecl(unsigned int nbTexCoords);

			inline static void setDevice(LPDIRECT3DDEVICE9 device);

			inline static void ReleaseResourcesOnDeviceFailure();
		};

		inline LPDIRECT3DDEVICE9 DX9Info::getDevice()
		{
			return DX9Info::device;
		}

		inline LPDIRECT3DVERTEXDECLARATION9 DX9Info::getDecl(unsigned int nbTexCoords)
		{
			return decl[nbTexCoords];
		}

		inline void DX9Info::setDevice(LPDIRECT3DDEVICE9 device)
		{
			DX9Info::device = device;

			for( unsigned int i = 0; i < 4; ++i )
				device->CreateVertexDeclaration(Decl[i], &decl[i]);
		}

		inline void DX9Info::ReleaseResourcesOnDeviceFailure()
		{
			for( unsigned int i = 0; i < 4; ++i )
				SAFE_RELEASE( decl[i] );
		}
	}
}

#endif
