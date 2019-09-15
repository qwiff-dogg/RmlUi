/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "precompiled.h"
#include "FontFace.h"
#include "FontFaceHandle.h"
#include "../../../Include/RmlUi/Core/Log.h"

namespace Rml {
namespace Core {

FontFace_FreeType::FontFace_FreeType(FT_Face _face, Style::FontStyle _style, Style::FontWeight _weight, bool _release_stream) : Rml::Core::FontFace(_style, _weight, _release_stream)
{
	face = _face;
}

FontFace_FreeType::~FontFace_FreeType()
{
	ReleaseFace();
}

// Returns a handle for positioning and rendering this face at the given size.
SharedPtr<Rml::Core::FontFaceHandle> FontFace_FreeType::GetHandle(int size)
{
	UnicodeRangeList charset;

	auto it = handles.find(size);
	if (it != handles.end())
		return it->second;

	// See if this face has been released.
	if (!face)
	{
		Log::Message(Log::LT_WARNING, "Font face has been released, unable to generate new handle.");
		return nullptr;
	}

	// Construct and initialise the new handle.
	auto handle = std::make_shared<FontFaceHandle_FreeType>();
	if (!handle->Initialise(face, size))
	{
		return nullptr;
	}

	// Save the new handle to the font face
	handles[size] = handle;

	return handle;
}

// Releases the face's FreeType face structure.
void FontFace_FreeType::ReleaseFace()
{
	if (face != nullptr)
	{
		FT_Byte* face_memory = face->stream->base;
		FT_Done_Face(face);

		if (release_stream)
			delete[] face_memory;

		face = nullptr;
	}
}

}
}
