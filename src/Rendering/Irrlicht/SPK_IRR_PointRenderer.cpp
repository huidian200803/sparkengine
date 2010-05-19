//////////////////////////////////////////////////////////////////////////////////
// SPARK Irrlicht Rendering library												//
// Copyright (C) 2009															//
// Thibault Lescoat -  info-tibo <at> orange <dot> fr							//
// Julien Fryer - julienfryer@gmail.com											//
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

#include "Rendering/Irrlicht/SPK_IRR_PointRenderer.h"
#include "Rendering/Irrlicht/SPK_IRR_Buffer.h"
#include "Core/SPK_Group.h"
#include "Core/SPK_Vector3D.h"
#include "Core/SPK_Iterator.h"

namespace SPK
{
namespace IRR
{
	IRRPointRenderer::IRRPointRenderer(irr::IrrlichtDevice* d,float screenSize) :
		IRRRenderer(d),
		PointRendererInterface(POINT_TYPE_SQUARE,screenSize)
	{
		material.Thickness = screenSize;
	}

	bool IRRPointRenderer::setType(PointType type)
	{
		switch(type)
		{
		case POINT_TYPE_SQUARE :
		case POINT_TYPE_SPRITE :
			this->type = type;
			return true;

		default :
			SPK_LOG_WARNING("IRRPointRenderer::setType(PointType) - this point type is not supported by Irrlicht");
			return false;
		}
	}

	RenderBuffer* IRRPointRenderer::attachRenderBuffer(const Group& group) const
	{
		// Creates the render buffer
		IRRBuffer* buffer = new IRRBuffer(device,group.getCapacity(),NB_VERTICES_PER_PARTICLE,NB_INDICES_PER_PARTICLE);
	
		// Initializes the index array
		buffer->positionAtStart();
		for (size_t i = 0; i < group.getCapacity(); ++i)
			buffer->setNextIndex(i);
		buffer->getMeshBuffer().setDirty(irr::scene::EBT_INDEX);

		return buffer;
	}

	void IRRPointRenderer::render(const Group& group,const DataSet* dataSet,RenderBuffer* renderBuffer) const
	{
		SPK_ASSERT(renderBuffer != NULL,"IRRPointRenderer::render(const Group&,const DataSet*,RenderBuffer*) - renderBuffer must not be NULL");
		IRRBuffer& buffer = dynamic_cast<IRRBuffer&>(*renderBuffer);

		buffer.positionAtStart();
		for (ConstGroupIterator particleIt(group); !particleIt.end(); ++particleIt)
		{
			buffer.setNextVertex(particleIt->position());
			buffer.setNextColor(particleIt->getColor());
		}
		buffer.getMeshBuffer().setDirty(irr::scene::EBT_VERTEX);

		material.Thickness = worldSize ? group.getRadius() : getScreenSize();

		irr::video::IVideoDriver* driver = device->getVideoDriver();
        driver->setMaterial(material);
        driver->drawVertexPrimitiveList(
			buffer.getMeshBuffer().getVertexBuffer().pointer(),
			group.getNbParticles(),
			buffer.getMeshBuffer().getIndexBuffer().pointer(),
			group.getNbParticles(),
			irr::video::EVT_STANDARD,
			type == POINT_TYPE_SPRITE ? irr::scene::EPT_POINT_SPRITES : irr::scene::EPT_POINTS,
			buffer.getMeshBuffer().getIndexBuffer().getType());
	}

	void IRRPointRenderer::computeAABB(Vector3D& AABBMin,Vector3D& AABBMax,const Group& group,const DataSet* dataSet) const
	{
		for (ConstGroupIterator particleIt(group); !particleIt.end(); ++particleIt)
		{
			AABBMin.setMin(particleIt->position());
			AABBMax.setMax(particleIt->position());
		}
	}
}}