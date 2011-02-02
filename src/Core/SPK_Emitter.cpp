//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2011 - Julien Fryer - julienfryer@gmail.com				//
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

#include <SPARK_Core.h>

namespace SPK
{
	Emitter::Emitter(const Ref<Zone>& zone,bool full,int tank,float flow,float forceMin,float forceMax) :
		SPKObject(),
		active(true),
		zone(zone == NULL ? SPK_DEFAULT_ZONE : zone),
		flow(1.0f),
		fraction(SPK_RANDOM(0.0f,1.0f))
	{
		setTank(tank);
		setFlow(flow);
		setForce(forceMin,forceMax);
	}

	Emitter::Emitter(const Emitter& emitter) :
		SPKObject(emitter),
		active(emitter.active),
		flow(emitter.flow),
		minTank(emitter.minTank),
		maxTank(emitter.maxTank),
		forceMin(emitter.forceMin),
		forceMax(emitter.forceMax),
		fraction(SPK_RANDOM(0.0f,1.0f))
	{
		zone = copyChild(emitter.zone);
		resetTank();
	}

	Emitter::~Emitter() {}

	void Emitter::setTank(int minTank,int maxTank)
	{
		SPK_ASSERT(minTank >= 0 == maxTank >= 0,"Emitter::setTank(int,int) : min and max tank values must be of the same sign");
		if (minTank < 0 || maxTank < 0) minTank = maxTank = -1;
		if (minTank > maxTank)
		{
			SPK_LOG_WARNING("Emitter::setTank(int,int) : min tank is greater than max tank. Values are swapped");
			std::swap(minTank,maxTank);
		}
		this->minTank = minTank;
		this->maxTank = maxTank;
		resetTank();
		SPK_ASSERT(flow >= 0.0f || currentTank >= 0,"Emitter::setTank(int,int) : the flow and tank of an emitter cannot be both negative");
	}

	void Emitter::setFlow(float flow)
	{
		SPK_ASSERT(flow >= 0.0f || currentTank >= 0,"Emitter::setFlow(float) : the flow and tank of an emitter cannot be both negative");
		this->flow = flow;
	}

	void Emitter::setForce(float min,float max)
	{
		if (forceMin <= forceMax)
		{
			forceMin = min;
			forceMax = max;
		}
		else
		{
			SPK_LOG_WARNING("Emitter::setForce(float,float) - min is higher than max - Values are swapped");
			forceMin = max;
			forceMax = min;
		}
	}

	void Emitter::setZone(const Ref<Zone>& zone,bool full)
	{
		this->zone = (zone == NULL ? SPK_DEFAULT_ZONE : zone);
		this->full = full;
	}

	void Emitter::propagateUpdateTransform()
	{
		if (!zone->isShared())
			zone->updateTransform(this);
	}

	void Emitter::emit(Particle& particle) const
	{
		zone->generatePosition(particle.position(),full);
		generateVelocity(particle,SPK_RANDOM(forceMin,forceMax) / particle.getParam(PARAM_MASS));
	}

	Ref<SPKObject> Emitter::findByName(const std::string& name)
	{
		Ref<SPKObject>& object = SPKObject::findByName(name);
		if (object != NULL) return object;

		return zone->findByName(name);
	}

	void Emitter::innerImport(const IO::Descriptor& descriptor)
	{
		SPKObject::innerImport(descriptor);

		const IO::Attribute* attrib = NULL;

		if (attrib = descriptor.getAttributeWithValue("active"))
			setActive(attrib->getValueBool());

		if (attrib = descriptor.getAttributeWithValue("tank"))
		{
			std::vector<long int> tmpTanks = attrib->getValuesInt32();
			switch(tmpTanks.size())
			{
				case 1 : setTank(tmpTanks[0]); break;
				case 2 : setTank(tmpTanks[0],tmpTanks[1]); break;
				default : SPK_LOG_ERROR("Emitter::innerImport(const IO::Descriptor&) - Wrong number of tanks : " << tmpTanks.size());
			}
		}

		if (attrib = descriptor.getAttributeWithValue("flow"))
			setFlow(attrib->getValueFloat());

		if (attrib = descriptor.getAttributeWithValue("force"))
		{
			std::vector<float> tmpForces = attrib->getValuesFloat();
			switch(tmpForces.size())
			{
				case 1 : setForce(tmpForces[0],tmpForces[0]); break;
				case 2 : setForce(tmpForces[0],tmpForces[1]); break;
				default : SPK_LOG_ERROR("Emitter::innerImport(const IO::Descriptor&) - Wrong number of forces : " << tmpForces.size());
			}
		}

		if (attrib = descriptor.getAttributeWithValue("zone"))
			setZone(attrib->getValueRef().cast<Zone>(),isFullZone());
		if (attrib = descriptor.getAttributeWithValue("full"))
			setZone(getZone(),attrib->getValueBool());
	}

	void Emitter::innerExport(IO::Descriptor& descriptor) const
	{
		SPKObject::innerExport(descriptor);

		descriptor.getAttribute("active")->setValueBool(isActive(),isActive());

		long int tmpTanks[2] = {minTank,maxTank};
		descriptor.getAttribute("tank")->setValuesInt32(tmpTanks,tmpTanks[0] == tmpTanks[1] ? 1 : 2);

		descriptor.getAttribute("flow")->setValueFloat(getFlow());

		float tmpForces[2] = {forceMin,forceMax};
		descriptor.getAttribute("force")->setValuesFloat(tmpForces,tmpForces[0] == tmpForces[1] ? 1 : 2);

		descriptor.getAttribute("zone")->setValueRef(getZone(),getZone() == SPK_DEFAULT_ZONE);
		descriptor.getAttribute("full")->setValueBool(isFullZone(),isFullZone());
	}
}
