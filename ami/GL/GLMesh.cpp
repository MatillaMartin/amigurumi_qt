#include "ami/GL/GLMesh.h"
#include "ami/GL/GLCommon.h"

#include <QtOpenGL/qgl.h>
#include <QOpenGLVertexArrayObject>

namespace ami
{
	GLMesh::GLMesh()
	{}

	GLMesh::~GLMesh()
	{
		delete m_verticesVao;
	}

	void GLMesh::draw()
	{
		syncGL();
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_verticesVbo));
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVbo));
		GL_CHECK(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));
	}
	
	void GLMesh::drawVertices()
	{
		syncGL();
	}
	
	void GLMesh::drawWireframe()
	{
		syncGL();
	}
	
	void GLMesh::clear()
	{
		clearVertices();
		clearIndices();
	}

	void GLMesh::addVertex(const glm::vec3 & vertex)
	{
		m_verticesDirty = true;
		m_vertices.push_back(vertex);
	}

	void GLMesh::addVertices(const std::vector<glm::vec3>& vertices)
	{
		m_verticesDirty = true;
		m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	}

	void GLMesh::setVertices(const std::vector<glm::vec3>& vertices)
	{
		m_verticesDirty = true;
		m_vertices = vertices;
	}

	void GLMesh::clearVertices()
	{
		m_verticesDirty = true;
		m_vertices.clear();
	}

	void GLMesh::addIndex(unsigned int index)
	{
		m_indicesDirty = true;
		m_indices.push_back(index);
	}

	void GLMesh::addIndices(const std::vector<unsigned int> & indices)
	{
		m_indicesDirty = true;
		m_indices.insert(m_indices.end(), indices.begin(), indices.end());
	}

	void GLMesh::setIndices(const std::vector<unsigned int> & indices)
	{
		m_indicesDirty = true;
		m_indices = indices;
	}

	void GLMesh::clearIndices()
	{
		m_indicesDirty = true;
		m_indices.clear();
	}

	void GLMesh::syncGL()
	{
		if (!m_verticesVao->isCreated())
		{
			// create VAO
			bool bVao = m_verticesVao->create();
			assert(bVao);
		}
		if (m_verticesDirty)
		{
			// upload vertices
			if (!m_verticesVbo)
			{
				GL_CHECK(glGenBuffers(1, &m_verticesVbo));
			}
			
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_verticesVbo));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_STATIC_DRAW));

			m_verticesDirty = false;
		}
		if (m_indicesDirty)
		{
			// upload indices
			if (!m_indicesVbo)
			{
				GL_CHECK(glGenBuffers(1, &m_indicesVbo));
			}

			GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesVbo));
			GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW));

			m_indicesDirty = false;
		}
	}
}
