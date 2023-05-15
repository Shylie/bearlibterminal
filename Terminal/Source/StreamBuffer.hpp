/*
* BearLibTerminal
* Copyright (C) 2023 Shylie
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BEARLIBTERMINAL_STREAMBUFFER_HPP
#define BEARLIBTERMINAL_STREAMBUFFER_HPP

#include "Color.hpp"
#include "Atlas.hpp"

namespace BearLibTerminal
{
	struct Vertex
	{
		uint32_t color;
		int position[2];
		float texture_coords[2];
	};

	struct Element
	{
		Vertex top_left;
		Vertex top_right;
		Vertex bottom_left;
		Vertex bottom_right;
		Vertex center;

		bool has_center = false;
	};

	class StreamBuffer
	{
	public:
		struct MapInfo
		{
			uint8_t* data;
			size_t size;
		};

		StreamBuffer(size_t size, uint32_t type);
		~StreamBuffer();

		StreamBuffer(StreamBuffer&& sb);
		StreamBuffer& operator=(StreamBuffer&& sb);

		size_t GetUsableSize() const;

		void Bind();
		MapInfo Map();
		void Unmap(size_t written);
		
		void Orphan();

		template <typename T>
		int Count() const
		{
			return m_gpu_read_offset / sizeof(T);
		}

	private:
		uint32_t m_handle;
		uint32_t m_type;
		size_t m_buffer_size;
		size_t m_gpu_read_offset;
		uint8_t* m_data;
		bool m_orphan;
	};
}

#endif /* BEARLIBTERMINAL_STREAMBUFFER_HPP */