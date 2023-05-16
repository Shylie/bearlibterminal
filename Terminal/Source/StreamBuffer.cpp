#include "StreamBuffer.hpp"
#include "OpenGL.hpp"

namespace BearLibTerminal
{
	std::map<uint32_t, uint32_t> StreamBuffer::m_currently_bound_buffers;

	StreamBuffer::StreamBuffer(size_t size, uint32_t type):
		m_handle(0),
		m_type(type),
		m_buffer_size(size),
		m_gpu_read_offset(0),
		m_data(new uint8_t[m_buffer_size])
	{
		glGenBuffers(1, &m_handle);
		glBindBuffer(m_type, m_handle);
		glBufferData(m_type, m_buffer_size, nullptr, GL_STREAM_DRAW);
	}

	StreamBuffer::~StreamBuffer()
	{
		if (m_handle > 0)
		{
			glBindBuffer(m_type, 0);
			glDeleteBuffers(1, &m_handle);
		}

		if (m_data)
		{
			delete[] m_data;
		}
	}

	StreamBuffer::StreamBuffer(StreamBuffer&& sb):
		m_handle(sb.m_handle),
		m_type(sb.m_type),
		m_buffer_size(sb.m_buffer_size),
		m_gpu_read_offset(sb.m_gpu_read_offset),
		m_data(sb.m_data)
	{
		sb.m_handle = 0;
		sb.m_data = nullptr;
	}

	StreamBuffer& StreamBuffer::operator=(StreamBuffer&& sb)
	{
		std::swap(m_handle, sb.m_handle);
		std::swap(m_data, sb.m_data);

		m_type = sb.m_type;
		m_buffer_size = sb.m_buffer_size;
		m_gpu_read_offset = sb.m_gpu_read_offset;

		return *this;
	}

	size_t StreamBuffer::GetUsableSize() const
	{
		return m_buffer_size - m_gpu_read_offset;
	}

	StreamBuffer::MapInfo StreamBuffer::Map()
	{
		return { m_data + m_gpu_read_offset, GetUsableSize() };
	}

	void StreamBuffer::Unmap(size_t written)
	{
		glBindBuffer(m_type, m_handle);
		glBufferSubData(m_type, m_gpu_read_offset, written, m_data + m_gpu_read_offset);

		m_gpu_read_offset += written;
	}

	void StreamBuffer::Reset()
	{
		m_gpu_read_offset = 0;
	}

	void StreamBuffer::Bind()
	{
		if (m_currently_bound_buffers[m_type] != m_handle)
		{
			glBindBuffer(m_type, m_handle);
		}
	}
}