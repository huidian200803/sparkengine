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

#ifndef H_SPK_GL_RENDERER
#define H_SPK_GL_RENDERER

#include "Rendering/OpenGL/SPK_GL_DEF.h"
#include "Core/SPK_Renderer.h"

namespace SPK
{
namespace GL
{
	/**
	* @class GLRenderer
	* @brief An abstract Renderer for the OpenGL renderers
	*/
	class SPK_GL_PREFIX GLRenderer : public Renderer
	{
	public :

		////////////////
		// Destructor //
		////////////////

		/** @brief Destructor of GLRenderer */
		virtual inline ~GLRenderer() {}

		/////////////
		// Setters //
		/////////////

		/**
		* @brief Enables or disables the blending of this GLRenderer
		* @param blendingEnabled true to enable the blending, false to disable it
		*/
		virtual inline void enableBlending(bool blendingEnabled);

		/**
		* @brief Sets the blending functions of this GLRenderer
		*
		* the blending functions are one of the OpenGL blending functions.
		*
		* @param src : the source blending function of this GLRenderer
		* @param dest : the destination blending function of this GLRenderer
		*/
		inline void setBlendingFunctions(GLuint src,GLuint dest);
		virtual void setBlending(Blending blendMode);

		/////////////
		// Getters //
		/////////////

		/**
		* @brief Tells whether blending is enabled for this GLRenderer
		* @return true if blending is enabled, false if it is disabled
		*/
		inline bool isBlendingEnabled() const;

		/**
		* @brief Gets the source blending function of this GLRenderer
		* @return the source blending function of this GLRenderer
		*/
		inline GLuint getSrcBlendingFunction() const;

		/**
		* @brief Gets the destination blending function of this GLRenderer
		* @return the source destination function of this GLRenderer
		*/
		inline GLuint getDestBlendingFunction() const;

		///////////////
		// Interface //
		///////////////

		/**
		* @brief Saves the current OpenGL states
		*
		* This method saves all the states that are likely to be modified by a GLRenderer.<br>
		* Use restoreGLStates() to restore the states.<br>
		* <br>
		* Note that for one saveGLStates call, a call to restoreGLStates must occur.
		* In case of several saveGLStates with no restoreGLStates, the restoreGLStates is called priorly in an implicit way.
		*/
		static void saveGLStates();

		/**
		* @brief Restores the OpenGL states
		*
		* This method restores the OpenGL states at the values they were at the last call of saveGLStates().
		*/
		static void restoreGLStates();

	protected :

		inline GLRenderer(bool NEEDS_DATASET);

		/** @brief Inits the blending of this GLRenderer */
		inline void initBlending() const;

		/** @brief Inits the rendering hints of this GLRenderer */
		inline void initRenderingOptions() const;

	private :

		bool blendingEnabled;
		GLuint srcBlending;
		GLuint destBlending;
	};

	inline GLRenderer::GLRenderer(bool NEEDS_DATASET) :
		Renderer(NEEDS_DATASET),
		blendingEnabled(false),
		srcBlending(GL_SRC_ALPHA),
		destBlending(GL_ONE_MINUS_SRC_ALPHA)
	{}

	inline void GLRenderer::enableBlending(bool blendingEnabled)
	{
		this->blendingEnabled = blendingEnabled;
	}

	inline void GLRenderer::setBlendingFunctions(GLuint src,GLuint dest)
	{
		srcBlending = src;
		destBlending = dest;
	}

	inline bool GLRenderer::isBlendingEnabled() const
	{
		return blendingEnabled;
	}

	inline GLuint GLRenderer::getSrcBlendingFunction() const
	{
		return srcBlending;
	}

	inline GLuint GLRenderer::getDestBlendingFunction() const
	{
		return destBlending;
	}

	inline void GLRenderer::initBlending() const
	{
		if (blendingEnabled)
		{
			glBlendFunc(srcBlending,destBlending);
			glEnable(GL_BLEND);
		}
		else
			glDisable(GL_BLEND);
	}

	inline void GLRenderer::initRenderingOptions() const
	{
		// alpha test
		if (isRenderingOptionEnabled(RENDERING_OPTION_ALPHA_TEST))
		{
			glAlphaFunc(GL_GEQUAL,getAlphaTestThreshold());
			glEnable(GL_ALPHA_TEST);
		}
		else
			glDisable(GL_ALPHA_TEST);

		// depth write
		glDepthMask(isRenderingOptionEnabled(RENDERING_OPTION_DEPTH_WRITE));
	}
}}

#endif
