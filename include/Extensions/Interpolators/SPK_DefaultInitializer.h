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

#ifndef H_SPK_DEFAULTINITIALIZER
#define H_SPK_DEFAULTINITIALIZER

namespace SPK
{
	template<typename T>
	class DefaultInitializer : public Interpolator<T>
	{
	SPK_IMPLEMENT_OBJECT(DefaultInitializer<T>)
	SPK_DEFINE_DESCRIPTION_TEMPLATE

	public :

		static inline Ref<DefaultInitializer<T>> create(const T& value);

		inline void setDefaultValue(const T& value);
		inline const T& getDefaultValue() const;

	protected :

		inline virtual void innerImport(const IO::Descriptor& descriptor);
		inline virtual void innerExport(IO::Descriptor& descriptor) const;

	private :

		T defaultValue;

		DefaultInitializer<T>(const T& value);
		DefaultInitializer<T>(const DefaultInitializer<T>& interpolator);

		virtual inline void interpolate(T* data,Group& group,DataSet* dataSet) const {}
		virtual inline void init(T& data,Particle& particle,DataSet* dataSet) const;
	};

	typedef DefaultInitializer<Color> ColorDefaultInitializer;
	typedef DefaultInitializer<float> FloatDefaultInitializer;

	SPK_START_DESCRIPTION_TEMPLATE(ColorDefaultInitializer)
	SPK_PARENT_ATTRIBUTES(ColorInterpolator)
	SPK_ATTRIBUTE("default value",ATTRIBUTE_TYPE_COLOR)
	SPK_END_DESCRIPTION

	SPK_START_DESCRIPTION_TEMPLATE(FloatDefaultInitializer)
	SPK_PARENT_ATTRIBUTES(FloatInterpolator)
	SPK_ATTRIBUTE("default value",ATTRIBUTE_TYPE_FLOAT)
	SPK_END_DESCRIPTION

	template<typename T>
	DefaultInitializer<T>::DefaultInitializer(const T& value) :
		Interpolator(false),
		defaultValue(value)
	{}

	template<typename T>
	DefaultInitializer<T>::DefaultInitializer(const DefaultInitializer<T>& interpolator) :
		Interpolator(interpolator),
		defaultValue(interpolator.defaultValue)
	{}

	template<typename T>
	inline Ref<DefaultInitializer<T>> DefaultInitializer<T>::create(const T& value)
	{
		return SPK_NEW(DefaultInitializer<T>,value);
	}

	template<typename T>
	inline void DefaultInitializer<T>::setDefaultValue(const T& value)
	{
		defaultValue = value;
	}

	template<typename T>
	inline const T& DefaultInitializer<T>::getDefaultValue() const
	{
		return defaultValue;
	}

	template<typename T>
	inline void DefaultInitializer<T>::init(T& data,Particle& particle,DataSet* dataSet) const
	{
		data = defaultValue;
	}

	template<>
	inline void DefaultInitializer<float>::innerImport(const IO::Descriptor& descriptor)
	{
		Interpolator<float>::innerImport(descriptor);

		const IO::Attribute* attrib = NULL;
		if (attrib = descriptor.getAttributeWithValue("default value"))
			setDefaultValue(attrib->getValueFloat());
	}

	template<>
	inline void DefaultInitializer<float>::innerExport(IO::Descriptor& descriptor) const
	{
		Interpolator<float>::innerExport(descriptor);
		descriptor.getAttribute("default value")->setValueFloat(getDefaultValue());
	}

	template<>
	inline void DefaultInitializer<Color>::innerImport(const IO::Descriptor& descriptor)
	{
		Interpolator<Color>::innerImport(descriptor);

		const IO::Attribute* attrib = NULL;
		if (attrib = descriptor.getAttributeWithValue("default value"))
			setDefaultValue(attrib->getValueColor());
	}

	template<>
	inline void DefaultInitializer<Color>::innerExport(IO::Descriptor& descriptor) const
	{
		Interpolator<Color>::innerExport(descriptor);
		descriptor.getAttribute("default value")->setValueColor(getDefaultValue());
	}	
}

#endif
