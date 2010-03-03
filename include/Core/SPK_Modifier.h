//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2010 - Julien Fryer - julienfryer@gmail.com				//
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

#ifndef H_SPK_MODIFIER
#define H_SPK_MODIFIER

#include "Core/SPK_DEF.h"
#include "Core/SPK_Registerable.h"
#include "Core/SPK_Transformable.h"
#include "Core/SPK_DataHandler.h"

namespace SPK
{
	class Particle;
	class Group;
	class DataSet;

	enum ModifierPriority
	{
		MODIFIER_PRIORITY_POSITION = 10,
		MODIFIER_PRIORITY_FORCE = 20,
		MODIFIER_PRIORITY_COLLISION = 30,
		MODIFIER_PRIORITY_FRICTION = 40,
		MODIFIER_PRIORITY_CHECK = 50,
	};

	class Modifier :	public Registerable,
						public Transformable, 
						public DataHandler
	{
	friend class Group;

	public :

		virtual inline ~Modifier() {}

		inline void setActive(bool active);
		inline bool isActive() const;

		/**
		* @brief Sets whether to consider this modifier local to a system or not
		*
		* A local modifier is transformed when its system is transformed, a non local one will not.
		*
		* @param local : true to consider the modifier local, false not to
		*/
		inline void setLocalToSystem(bool local);

		/**
		* @brief Tells whether this modifier is considered as being local to a system or not
		*
		* A local modifier is transformed when its system is transformed, a non local one will not.
		*
		* @return true if it is local, false if not
		*/
		inline bool isLocalToSystem() const;

		inline unsigned int getPriority() const;

	protected :

		inline Modifier(unsigned int PRIORITY,bool NEEDS_DATASET,bool CALL_INIT);

	private :

		const unsigned int PRIORITY;
		const bool CALL_INIT;
		
		bool active;
		bool local;

		virtual void init(Particle& particle,DataSet* dataSet) const {};
		virtual void modify(Group& group,DataSet* dataSet,float deltaTime) const = 0;
	};

	inline Modifier::Modifier(unsigned int PRIORITY,bool NEEDS_DATASET,bool CALL_INIT) :
		DataHandler(NEEDS_DATASET),
		PRIORITY(PRIORITY),
		CALL_INIT(CALL_INIT),
		active(true),
		local(false)
	{}

	inline void Modifier::setActive(bool active)
	{
		this->active = active;
	}
	
	inline bool Modifier::isActive() const
	{
		return active;
	}

	inline void Modifier::setLocalToSystem(bool local)
	{
		this->local = local;
	}

	inline bool Modifier::isLocalToSystem() const
	{
		return local;
	}

	inline unsigned int Modifier::getPriority() const
	{
		return PRIORITY;
	}
}

#endif
