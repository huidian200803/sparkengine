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

#ifndef H_SPK_ITERATOR
#define H_SPK_ITERATOR

#include "Core/SPK_DEF.h"
#include "Core/SPK_Group.h"
#include "Core/SPK_Particle.h"
#include "Core/SPK_Logger.h"
#include "Core/SPK_System.h"

namespace SPK
{
	/**
	* @brief An object to iterate on a collection of particles
	*/
	template<typename T>
	class Iterator
	{
	public :

		Iterator(T& t);

		inline Particle& operator*();
		inline Particle* operator->();

		inline Iterator& operator++();
		inline Iterator operator++(int);

		inline bool end() const;

	private :

		Particle particle;
	};

	template<typename T>
	class ConstIterator
	{
	public :

		ConstIterator(const T& t);

		inline const Particle& operator*();
		inline const Particle* operator->();

		inline ConstIterator& operator++();
		inline ConstIterator operator++(int);

		inline bool end() const;

	private :

		const Particle particle;
	};

	typedef Iterator<Group> GroupIterator;
	typedef ConstIterator<Group> ConstGroupIterator;

	inline Iterator<Group>::Iterator(Group& group) :
		particle(group,0)
	{
		SPK_ASSERT(group.getSystem().isInitialized(),"Iterator::Iterator(Group&) - An iterator from an uninitialized group cannot be retrieved");
	}

	inline Particle& Iterator<Group>::operator*()
	{ 
		return particle;
	}
		
	inline Particle* Iterator<Group>::operator->() 
	{
		return &particle;
	}

	inline Iterator<Group>& Iterator<Group>::operator++()
	{
		++particle.index;
		return *this;
	}

	inline Iterator<Group> Iterator<Group>::operator++(int)
	{
		Iterator tmp(*this);
		return ++tmp;
	}

	inline bool Iterator<Group>::end() const
	{ 
		return particle.index >= particle.group.getNbParticles();
	}

	inline ConstIterator<Group>::ConstIterator(const Group& group) :
		particle(const_cast<Group&>(group),0)
	{
		SPK_ASSERT(group.getSystem().isInitialized(),"ConstIterator::ConstIterator(Group&) - An const iterator from a uninitialized group cannot be retrieved");	
	}

	inline const Particle& ConstIterator<Group>::operator*()
	{ 
		return particle;
	}
		
	inline const Particle* ConstIterator<Group>::operator->() 
	{
		return &particle;
	}

	inline ConstIterator<Group>& ConstIterator<Group>::operator++()
	{
		++particle.index;
		return *this;
	}

	inline ConstIterator<Group> ConstIterator<Group>::operator++(int)
	{
		ConstIterator tmp(*this);
		return ++tmp;
	}

	inline bool ConstIterator<Group>::end() const
	{ 
		return particle.index >= particle.group.getNbParticles();
	}
}

#endif
