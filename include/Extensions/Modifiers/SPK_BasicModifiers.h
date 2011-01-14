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

#ifndef H_SPK_BASICMODIFIERS
#define H_SPK_BASICMODIFIERS

namespace SPK
{
	class SPK_PREFIX Gravity : public Modifier
	{
	SPK_IMPLEMENT_REFERENCEABLE(Gravity)
	SPK_IMPLEMENT_SERIALIZABLE(Gravity)

	SPK_START_DESCRIPTION
	SPK_PARENT_ATTRIBUTES(Modifier)
	SPK_ATTRIBUTE("value",ATTRIBUTE_TYPE_VECTOR)
	SPK_END_DESCRIPTION

	public :

		static inline Ref<Gravity> create(const Vector3D& value = Vector3D());

		inline void setValue(const Vector3D& value);
		inline const Vector3D& getValue() const;
		inline const Vector3D& getTransformedValue() const;

	protected :

		virtual inline void innerUpdateTransform();

		virtual void innerImport(const IO::Descriptor& descriptor);
		virtual void innerExport(IO::Descriptor& descriptor) const;

	private :

		Vector3D value;
		Vector3D tValue;

		inline Gravity(const Vector3D& value = Vector3D());
		inline Gravity(const Gravity& gravity);

		virtual void modify(Group& group,DataSet* dataSet,float deltaTime) const;
	};

	class SPK_PREFIX Friction : public Modifier
	{
	SPK_IMPLEMENT_REFERENCEABLE(Friction)
	SPK_IMPLEMENT_SERIALIZABLE(Friction)

	SPK_START_DESCRIPTION
	SPK_PARENT_ATTRIBUTES(Modifier)
	SPK_ATTRIBUTE("value",ATTRIBUTE_TYPE_FLOAT)
	SPK_END_DESCRIPTION

	public :

		static inline Ref<Friction> create(float value = 0.0f);

		float value;

	protected :

		virtual void innerImport(const IO::Descriptor& descriptor);
		virtual void innerExport(IO::Descriptor& descriptor) const;

	private :

		inline Friction(float value = 0.0f);
		inline Friction(const Friction& friction);

		virtual void modify(Group& group,DataSet* dataSet,float deltaTime) const;
	};

	inline Gravity::Gravity(const Vector3D& value) :
		Modifier(MODIFIER_PRIORITY_FORCE,false,false)
	{
		setValue(value);	
	}

	inline Gravity::Gravity(const Gravity& gravity) :
		Modifier(gravity)
	{
		setValue(gravity.value);
	}

	inline Ref<Gravity> Gravity::create(const Vector3D& value)
	{
		return SPK_NEW(Gravity,value);
	}

	inline void Gravity::setValue(const Vector3D& value)
	{
		this->value = value;
		transformDir(tValue,value);
	}
		
	inline const Vector3D& Gravity::getValue() const
	{
		return value;
	}
		
	inline const Vector3D& Gravity::getTransformedValue() const
	{
		return tValue;
	}

	inline void Gravity::innerUpdateTransform()
	{
		transformDir(tValue,value);
	}

	inline Friction::Friction(float value) :
		Modifier(MODIFIER_PRIORITY_FRICTION,false,false),
		value(value)
	{}

	inline Friction::Friction(const Friction& friction) :
		Modifier(friction),
		value(friction.value)
	{}

	inline Ref<Friction> Friction::create(float value)
	{
		return SPK_NEW(Friction,value);
	}
}

#endif
